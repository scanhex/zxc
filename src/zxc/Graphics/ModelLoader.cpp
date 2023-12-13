#include "ModelLoader.h"

#include "Drawables.h"
#include "ShaderLibrary.h"

#include <Magnum/GL/Texture.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/ImageView.h>
#include <Magnum/Mesh.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/PixelFormat.h>
#include <Magnum/Trade/AbstractImporter.h>
#include <Magnum/Trade/AbstractMaterialData.h>
#include <Magnum/Trade/ImageData.h>
#include <Magnum/Trade/MeshData3D.h>
#include <Magnum/Trade/MeshObjectData3D.h>
#include <Magnum/Trade/ObjectData3D.h>
#include <Magnum/Trade/PhongMaterialData.h>
#include <Magnum/Trade/SceneData.h>
#include <Magnum/Trade/TextureData.h>

#include <cassert>

Pointer<Object3D>
ModelLoader::loadModel(std::string filename, Scene3D &scene, SceneGraph::DrawableGroup3D &drawables, bool wtf) {
    /* Load a scene importer plugin */
    Corrade::PluginManager::Manager<Magnum::Trade::AbstractImporter> manager;
    auto loadState = manager.load("AssimpImporter");
    assert(
        loadState == Corrade::PluginManager::LoadState::Loaded || loadState == Corrade::PluginManager::LoadState::Static
    );
    Pointer<Magnum::Trade::AbstractImporter> importer = manager.instantiate("AssimpImporter");

    Pointer<Object3D> manipulator(new Object3D(&scene));

    Debug{} << "Opening file" << filename;

    /* Load file */
    if (!importer->openFile(filename)) {
        std::exit(4);
    }

    /* Load all textures. Textures that fail to load will be NullOpt. */
    /**/
    //    if (textures_.empty()) { // TODO apply correct memoization
    textures_ = Containers::Array<Containers::Optional<GL::Texture2D>>{importer->textureCount()};
    for (UnsignedInt i = 0; i != importer->textureCount(); ++i) {
        Debug{} << "Importing texture" << i << importer->textureName(i);

        Containers::Optional<Trade::TextureData> textureData = importer->texture(i);
        if (!textureData || textureData->type() != Trade::TextureType::Texture2D) {
            Warning{} << "Cannot load texture properties, skipping";
            continue;
        }

        Debug{} << "Importing image" << textureData->image() << importer->image2DName(textureData->image());

        Containers::Optional<Trade::ImageData2D> imageData = importer->image2D(textureData->image());
        GL::TextureFormat format;
        if (imageData && imageData->format() == PixelFormat::RGB8Unorm) {
            format = GL::TextureFormat::RGB8;
        } else if (imageData && imageData->format() == PixelFormat::RGBA8Unorm) {
            format = GL::TextureFormat::RGBA8;
        } else {
            Warning{} << "Cannot load texture image, skipping";
            continue;
        }

        /* Configure the texture */
        GL::Texture2D texture;
        texture.setMagnificationFilter(textureData->magnificationFilter())
            .setMinificationFilter(textureData->minificationFilter(), textureData->mipmapFilter())
            .setWrapping(textureData->wrapping().xy())
            .setStorage(static_cast<int>(Math::log2(imageData->size().max())) + 1, format, imageData->size())
            .setSubImage(0, {}, *imageData)
            .generateMipmap();

        textures_[i] = std::move(texture);
    }

    /* Load all meshes. Meshes that fail to load will be NullOpt. */
    meshes_ = Containers::Array<Containers::Optional<GL::Mesh>>{importer->meshCount()};
    for (UnsignedInt i = 0; i != importer->meshCount(); ++i) {
        Debug{} << "Importing mesh" << i << importer->meshName(i);

        Containers::Optional<Trade::MeshData> meshData = importer->mesh(i);

        if (!meshData || meshData->normalsAsArray().isEmpty() || meshData->primitive() != MeshPrimitive::Triangles) {
            Warning{} << "Cannot load the mesh, skipping";
            continue;
        }

        /* Compile the mesh */
        meshes_[i] = MeshTools::compile(*meshData);
    }
    //    }

    /* Load all materials. Materials that fail to load will be NullOpt. The
       data will be stored directly in objects later, so save them only
       temporarily. */
    Containers::Array<Containers::Optional<Trade::PhongMaterialData>> materials{importer->materialCount()};
    for (UnsignedInt i = 0; i != importer->materialCount(); ++i) {
        Debug{} << "Importing material" << i << importer->materialName(i);

        Containers::Optional<Trade::MaterialData> materialData = importer->material(i);
        if (!materialData || materialData->types() != Trade::MaterialType::Phong) {
            Warning{} << "Cannot load material, skipping";
            continue;
        }

        materials[i] = std::move(*materialData).as<Trade::PhongMaterialData>();
    }

    /* Load the scene */
    if (importer->defaultScene() != -1) {
        Debug{} << "Adding default scene" << importer->sceneName(importer->defaultScene());

        Containers::Optional<Trade::SceneData> sceneData = importer->scene(importer->defaultScene());
        if (!sceneData) {
            Error{} << "Cannot load scene, exiting";
            return manipulator;
        }

        /* Recursively add all children */
        for (UnsignedInt objectId : sceneData->children3D()) {
            addObject(*importer, materials, *manipulator, objectId, drawables, wtf);
        }

        /* The format has no scene support, display just the first loaded mesh with
           a default material and be done with it */
    } else if (!meshes_.isEmpty() && meshes_[0]) {
        new ColoredDrawable{*manipulator, ShaderLibrary::coloredShader(), *meshes_[0], 0xffffff_rgbf, drawables};
    }
    return manipulator;
}

void ModelLoader::addObject(
    Trade::AbstractImporter &importer,
    Containers::ArrayView<const Containers::Optional<Trade::PhongMaterialData>> materials,
    Object3D &parent,
    UnsignedInt i,
    SceneGraph::DrawableGroup3D &drawables,
    bool wtf
) {
    Debug{} << "Importing object" << i << importer.object3DName(i);
    Containers::Pointer<Trade::ObjectData3D> objectData = importer.object3D(i);
    if (!objectData) {
        Error{} << "Cannot import object, skipping";
        return;
    }

    /* Add the object to the scene and set its transformation */
    auto *object = new Object3D{&parent};
    /*
    object->setTransformation(//Matrix4::rotationX(Magnum::Rad(Math::Constants<float>::piHalf())) *
                objectData->transformation() * Matrix4::scaling({ 0.01f, 0.01f, 0.01f }));
        */
    if (wtf) {
        object->rotateX(Magnum::Rad(Math::Constants<float>::piQuarter()));
    } else {
        object->rotateX(Magnum::Rad(Math::Constants<float>::piHalf()));
    }
    /* Add a drawable if the object has a mesh and the mesh is loaded */
    if (objectData->instanceType() == Trade::ObjectInstanceType3D::Mesh && objectData->instance() != -1 &&
        meshes_[objectData->instance()]) {
        Int materialId = dynamic_cast<Trade::MeshObjectData3D *>(objectData.get())->material();

        /* Material not available / not loaded, use a default material */
        if (materialId == -1 || !materials[materialId]) {
            new ColoredDrawable{
                *object, ShaderLibrary::coloredShader(), *meshes_[objectData->instance()], 0xfffffe_rgbf, drawables
            };

            /* Textured material. If the texture failed to load, again just use a
               default colored material. */
        } else if (materials[materialId]->flags() & Trade::PhongMaterialData::Flag::DiffuseTexture) {
            Containers::Optional<GL::Texture2D> &texture = textures_[materials[materialId]->diffuseTexture()];
            std::cerr << materials[materialId]->diffuseTexture() << std::endl;
            if (texture) {
                new TexturedDrawable{
                    *object, ShaderLibrary::texturedShader(), *meshes_[objectData->instance()], *texture, drawables
                };
            } else {
                new ColoredDrawable{
                    *object, ShaderLibrary::coloredShader(), *meshes_[objectData->instance()], 0xfffffe_rgbf, drawables
                };
            }

            /* Color-only material */
        } else {
            new ColoredDrawable{
                *object,
                ShaderLibrary::coloredShader(),
                *meshes_[objectData->instance()],
                materials[materialId]->diffuseColor(),
                drawables
            };
        }
    }

    /* Recursively add children */
    for (std::size_t id : objectData->children()) {
        addObject(importer, materials, *object, id, drawables, wtf);
    }
}
