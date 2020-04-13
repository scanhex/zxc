#include <Corrade/Containers/Array.h>
#include <Corrade/Containers/Optional.h>
#include <Corrade/PluginManager/Manager.h>
#include <Corrade/Utility/Arguments.h>
#include <Corrade/Utility/DebugStl.h>
#include <Magnum/ImageView.h>
#include <Magnum/Math/Math.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/FunctionsBatch.h>
#include <Magnum/Mesh.h>
#include <Magnum/PixelFormat.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/GL/PixelFormat.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/Primitives/Grid.h>
#include <Magnum/Shaders/Phong.h>
#include <Magnum/Trade/AbstractImporter.h>
#include <Magnum/Trade/ImageData.h>
#include <Magnum/Trade/MeshData3D.h>
#include <Magnum/Trade/MeshObjectData3D.h>
#include <Magnum/Trade/PhongMaterialData.h>
#include <Magnum/Trade/SceneData.h>
#include <Magnum/Trade/TextureData.h>
//#include <MagnumPlugins/AssimpImporter/AssimpImporter.h>
#include <Magnum/Magnum.h>
#include <Magnum/Image.h>

#include <iostream>
#include <fstream>
#include <cassert>

#include "Drawables.h"
#include "Game/StatsBuilder.h"

using namespace Magnum;

using namespace Math::Literals;

typedef SceneGraph::Object<SceneGraph::MatrixTransformation3D> Object3D;
typedef SceneGraph::Scene<SceneGraph::MatrixTransformation3D> Scene3D;

class ZxcApplication : public Platform::Application {
public:
	explicit ZxcApplication(const Arguments& arguments);

private:

	void drawEvent() override;
	void viewportEvent(ViewportEvent& event) override;
	void mousePressEvent(MouseEvent& event) override;
	void mouseReleaseEvent(MouseEvent& event) override;
	void mouseMoveEvent(MouseMoveEvent& event) override;
	void mouseScrollEvent(MouseScrollEvent& event) override;

    void keyPressEvent(KeyEvent &event) override;

    Vector3 positionOnSphere(const Vector2i& position) const;

    void addUnit(Unit &u);
	void initScene();
	void initUnits();
	void loadModels();
	Float depthAt(const Vector2i& position) const;
	Vector3 unproject(const Vector2i& position, Float depth) const;
    Vector3 intersectWithPlane(const Vector2i& windowPosition, const Vector3& planeNormal) const;

	void addObject(Trade::AbstractImporter& importer, Containers::ArrayView<const Containers::Optional<Trade::PhongMaterialData>> materials, Object3D& parent, UnsignedInt i);

	Shaders::Phong _coloredShader,
		_texturedShader{ Shaders::Phong::Flag::DiffuseTexture };
	Shaders::Flat3D _flatShader {NoCreate};
	Containers::Array<Containers::Optional<GL::Mesh>> _meshes;
	Containers::Array<Containers::Optional<GL::Texture2D>> _textures;

    std::vector<Unit> _units;
    std::vector<Object3D*> _unitObjects;

	Scene3D _scene;
	Object3D _manipulator, _cameraObject;
	SceneGraph::Camera3D* _camera = nullptr;
	Object3D _mapObject;
	SceneGraph::DrawableGroup3D _drawables;
	Vector3 _previousPosition;

	GL::Mesh _grid;
};


void ZxcApplication::initScene() {
	/* Every scene needs a camera */
	/* (c) Confucius */
	_cameraObject
		.setParent(&_scene)
		.translate(Vector3::zAxis(20.0f));
	(*(_camera = new SceneGraph::Camera3D{ _cameraObject }))
		.setAspectRatioPolicy(SceneGraph::AspectRatioPolicy::Extend)
		.setProjectionMatrix(Matrix4::perspectiveProjection(35.0_degf, 1.0f, 0.01f, 1000.0f))
		.setViewport(GL::defaultFramebuffer.viewport().size());
	/* Base object, parent of all (for easy manipulation) */
	_manipulator.setParent(&_scene);

	/* Setup renderer and shader defaults */
	GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
	GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);
	_coloredShader
		.setAmbientColor(0x111111_rgbf)
		.setSpecularColor(0xffffff_rgbf)
		.setShininess(8.0f);
	_texturedShader
		.setAmbientColor(0x111111_rgbf)
		.setSpecularColor(0x111111_rgbf)
		.setShininess(8.0f);

	_flatShader = Shaders::Flat3D{};

	_grid = MeshTools::compile(Primitives::grid3DSolid({15, 15}));
	auto grid = new Object3D{&_manipulator};
    (*grid).scale(Vector3{8});
    new FlatDrawable{*grid, _flatShader, _grid, _drawables};
}

void ZxcApplication::loadModels() {

	/* Load a scene importer plugin */
	PluginManager::Manager<Trade::AbstractImporter> manager;
	auto loadState = manager.load("AssimpImporter");
	assert(loadState == Corrade::PluginManager::LoadState::Loaded || loadState == Corrade::PluginManager::LoadState::Static);
	Corrade::Containers::Pointer<Trade::AbstractImporter> importer = manager.instantiate("AssimpImporter");

	std::string filename = "C:/Users/ISM/Code/zxc/resources/nevermore/nevermore_blender.fbx";

	Debug{} << "Opening file" << filename;

	/* Load file */
	if (!importer->openFile(filename))
		std::exit(4);

	/* Load all textures. Textures that fail to load will be NullOpt. */
	/**/
	_textures = Containers::Array<Containers::Optional<GL::Texture2D>>{ importer->textureCount() };
	for (UnsignedInt i = 0; i != importer->textureCount(); ++i) {
		Debug{} << "Importing texture" << i << importer->textureName(i);

		Containers::Optional<Trade::TextureData> textureData = importer->texture(i);
		if (!textureData || textureData->type() != Trade::TextureData::Type::Texture2D) {
			Warning{} << "Cannot load texture properties, skipping";
			continue;
		}

		Debug{} << "Importing image" << textureData->image() << importer->image2DName(textureData->image());

		Containers::Optional<Trade::ImageData2D> imageData = importer->image2D(textureData->image());
		GL::TextureFormat format;
		if (imageData && imageData->format() == PixelFormat::RGB8Unorm)
			format = GL::TextureFormat::RGB8;
		else if (imageData && imageData->format() == PixelFormat::RGBA8Unorm)
			format = GL::TextureFormat::RGBA8;
		else {
			Warning{} << "Cannot load texture image, skipping";
			continue;
		}

		/* Configure the texture */
		GL::Texture2D texture;
		texture
			.setMagnificationFilter(textureData->magnificationFilter())
			.setMinificationFilter(textureData->minificationFilter(), textureData->mipmapFilter())
			.setWrapping(textureData->wrapping().xy())
			.setStorage(Math::log2(imageData->size().max()) + 1, format, imageData->size())
			.setSubImage(0, {}, *imageData)
			.generateMipmap();

		_textures[i] = std::move(texture);
	}

	/* Load all materials. Materials that fail to load will be NullOpt. The
	   data will be stored directly in objects later, so save them only
	   temporarily. */
	Containers::Array<Containers::Optional<Trade::PhongMaterialData>> materials{ importer->materialCount() };
	for (UnsignedInt i = 0; i != importer->materialCount(); ++i) {
		Debug{} << "Importing material" << i << importer->materialName(i);

		Containers::Pointer<Trade::AbstractMaterialData> materialData = importer->material(i);
		if (!materialData || materialData->type() != Trade::MaterialType::Phong) {
			Warning{} << "Cannot load material, skipping";
			continue;
		}

		materials[i] = std::move(static_cast<Trade::PhongMaterialData&>(*materialData));
	}

	/* Load all meshes. Meshes that fail to load will be NullOpt. */
	_meshes = Containers::Array<Containers::Optional<GL::Mesh>>{ importer->mesh3DCount() };
	for (UnsignedInt i = 0; i != importer->mesh3DCount(); ++i) {
		Debug{} << "Importing mesh" << i << importer->mesh3DName(i);

		Containers::Optional<Trade::MeshData3D> meshData = importer->mesh3D(i);

		if (!meshData || !meshData->hasNormals() || meshData->primitive() != MeshPrimitive::Triangles) {
			Warning{} << "Cannot load the mesh, skipping";
			continue;
		}

		/* Compile the mesh */
		_meshes[i] = MeshTools::compile(*meshData);
	}

	/* Load the scene */
	if (importer->defaultScene() != -1) {
		Debug{} << "Adding default scene" << importer->sceneName(importer->defaultScene());

		Containers::Optional<Trade::SceneData> sceneData = importer->scene(importer->defaultScene());
		if (!sceneData) {
			Error{} << "Cannot load scene, exiting";
			return;
		}

		/* Recursively add all children */
		for (UnsignedInt objectId : sceneData->children3D())
			addObject(*importer, materials, _manipulator, objectId);

		/* The format has no scene support, display just the first loaded mesh with
		   a default material and be done with it */
	}
	else if (!_meshes.empty() && _meshes[0])
		new ColoredDrawable{ _manipulator, _coloredShader, *_meshes[0], 0xffffff_rgbf, _drawables };
}

void ZxcApplication::addObject(Trade::AbstractImporter& importer, Containers::ArrayView<const Containers::Optional<Trade::PhongMaterialData>> materials, Object3D& parent, UnsignedInt i) {
    Debug{} << "Importing object" << i << importer.object3DName(i);
    Containers::Pointer<Trade::ObjectData3D> objectData = importer.object3D(i);
    if (!objectData) {
        Error{} << "Cannot import object, skipping";
        return;
    }

    /* Add the object to the scene and set its transformation */
    auto* object = new Object3D{ &parent };
    object->setTransformation(objectData->transformation());

    /* Add a drawable if the object has a mesh and the mesh is loaded */
    if (objectData->instanceType() == Trade::ObjectInstanceType3D::Mesh && objectData->instance() != -1 && _meshes[objectData->instance()]) {
        Int materialId = static_cast<Trade::MeshObjectData3D*>(objectData.get())->material();

        materialId = -1;

        /* Material not available / not loaded, use a default material */
        if (materialId == -1 || !materials[materialId]) {
            new ColoredDrawable{ *object, _coloredShader, *_meshes[objectData->instance()], 0xfffffe_rgbf, _drawables };

            /* Textured material. If the texture failed to load, again just use a
               default colored material. */
        }
        else if (materials[materialId]->flags() & Trade::PhongMaterialData::Flag::DiffuseTexture) {
            Containers::Optional<GL::Texture2D>& texture = _textures[materials[materialId]->diffuseTexture()];
            std::cerr << materials[materialId]->diffuseTexture() << std::endl;
            if (texture)
                new TexturedDrawable{ *object, _texturedShader, *_meshes[objectData->instance()], *texture, _drawables };
            else
                new ColoredDrawable{ *object, _coloredShader, *_meshes[objectData->instance()], 0xfffffe_rgbf, _drawables };

            /* Color-only material */
        }
        else {
            new ColoredDrawable{ *object, _coloredShader, *_meshes[objectData->instance()], materials[materialId]->diffuseColor(), _drawables };
        }
    }

    /* Recursively add children */
    for (std::size_t id : objectData->children())
        addObject(importer, materials, *object, id);
}

void ZxcApplication::initUnits(){
    StatsBuilder heroStatsBuilder = StatsBuilder().
            setDamage(100).
            setAttackRange(100).
            setMoveSpeed(350).
            setAttackSpeed(100).
            setMaxHp(1000).
            setMaxMp(300).
            setHpRegen(2).
            setMpRegen(1).
            setArmor(3).
            setResist(0.25);
    Hero firstHero = Hero(heroStatsBuilder.create(), Point(0, 0));
    Hero secondHero = Hero(heroStatsBuilder.create(), Point(0, 0));
    addUnit(firstHero);
    addUnit(secondHero);

}

ZxcApplication::ZxcApplication(const Arguments& arguments) :
	Platform::Application{ arguments, Configuration{}
		.setTitle("ZXC")
		.setWindowFlags(Configuration::WindowFlag::Resizable) }

{
	setSwapInterval(1);
	initScene();
	initUnits();

	//loadModels();
	/*
	Utility::Arguments args;
	args.addArgument("file").setHelp("file", "file to load")
		.addOption("importer", "AnySceneImporter").setHelp("importer", "importer plugin to use")
		.addSkippedPrefix("magnum", "engine-specific options")
		.setGlobalHelp("Displays a 3D scene file provided on command line.")
		.parse(arguments.argc, arguments.argv);
	*/

}

void ZxcApplication::addUnit(Unit& u) {
    _units.push_back(u);
    _unitObjects.push_back(new Object3D(&_manipulator));
    new UnitDrawable(*_unitObjects.back(), _drawables, _units.back());
}




void ZxcApplication::drawEvent() {
	GL::defaultFramebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);

	assert(_camera);
	_camera->draw(_drawables);

	swapBuffers();
}

void ZxcApplication::viewportEvent(ViewportEvent& event) {
	GL::defaultFramebuffer.setViewport({ {}, event.framebufferSize() });
	_camera->setViewport(event.windowSize());
}

void ZxcApplication::mousePressEvent(MouseEvent& event) {
	if (event.button() == MouseEvent::Button::Left)
		_previousPosition = positionOnSphere(event.position());
	if (event.button() == MouseEvent::Button::Right) {
        auto newPosition = intersectWithPlane(event.position(), {0,0,1});
        _unitObjects[0]->translate(newPosition - _unitObjects[0]->transformation().translation());
        redraw();
	}
}

void ZxcApplication::mouseReleaseEvent(MouseEvent& event) {
	if (event.button() == MouseEvent::Button::Left)
		_previousPosition = Vector3();
}

void ZxcApplication::mouseScrollEvent(MouseScrollEvent& event) {
	if (!event.offset().y()) return;

	/* Distance to origin */
	const Float distance = _cameraObject.transformation().translation().z();

	/* Move 15% of the distance back or forward */
	_cameraObject.translate(Vector3::zAxis(
		distance * (1.0f - (event.offset().y() > 0 ? 1 / 0.85f : 0.85f))));

	redraw();
}

//Assuming plane contains zero
Vector3 ZxcApplication::intersectWithPlane(const Vector2i& windowPosition, const Vector3& planeNormal) const {
    /* We have to take window size, not framebuffer size, since the position is
       in window coordinates and the two can be different on HiDPI systems */
    const Vector2i viewSize = windowSize();
    const Vector2i viewPosition{windowPosition.x(), viewSize.y() - windowPosition.y() - 1};
    const Vector3 ray_nds{2*Vector2{viewPosition}/Vector2{viewSize} - Vector2{1.0f}, 1};
    const Vector4 ray_clip{ray_nds.x(), ray_nds.y(), -1, 1};
    Vector4 ray_eye = _camera->projectionMatrix().inverted() * ray_clip;
    ray_eye.z() = -1, ray_eye.w() = 0;
    Vector3 ray_world = (_camera->cameraMatrix().inverted() * ray_eye).xyz().normalized();
    const Float dist = -Math::dot(_cameraObject.absoluteTransformation().translation(), planeNormal) / Math::dot(ray_world, planeNormal);
    return _cameraObject.absoluteTransformation().translation() + ray_world * dist;
}

Float ZxcApplication::depthAt(const Vector2i& windowPosition) const {
    /* First scale the position from being relative to window size to being
       relative to framebuffer size as those two can be different on HiDPI
       systems */
    const Vector2i position = windowPosition*Vector2{framebufferSize()}/Vector2{windowSize()};
    const Vector2i fbPosition{position.x(), GL::defaultFramebuffer.viewport().sizeY() - position.y() - 1};

    GL::defaultFramebuffer.mapForRead(GL::DefaultFramebuffer::ReadAttachment::Front);
    Image2D data = GL::defaultFramebuffer.read(
            Range2Di::fromSize(fbPosition, Vector2i{1}).padded(Vector2i{2}),
            {GL::PixelFormat::DepthComponent, GL::PixelType::Float});

    return Math::min<Float>(Containers::arrayCast<const Float>(data.data()));
}

Vector3 ZxcApplication::unproject(const Vector2i& windowPosition, Float depth) const {
    /* We have to take window size, not framebuffer size, since the position is
       in window coordinates and the two can be different on HiDPI systems */
    const Vector2i viewSize = windowSize();
    const Vector2i viewPosition{windowPosition.x(), viewSize.y() - windowPosition.y() - 1};
    const Vector3 in{2*Vector2{viewPosition}/Vector2{viewSize} - Vector2{1.0f}, depth*2.0f - 1.0f};


      return (_cameraObject.absoluteTransformationMatrix()*_camera->projectionMatrix().inverted()).transformPoint(in);
      /*
    Use the following to get the camera-relative coordinates instead of global:
//    return _camera->projectionMatrix().inverted().transformPoint(in);
       */
}

Vector3 ZxcApplication::positionOnSphere(const Vector2i& position) const {
	const Vector2 positionNormalized = Vector2{ position } / Vector2{ _camera->viewport() } -Vector2{ 0.5f };
	const Float length = positionNormalized.length();
	const Vector3 result(length > 1.0f ? Vector3(positionNormalized, 0.0f) : Vector3(positionNormalized, 1.0f - length));
	return (result * Vector3::yScale(-1.0f)).normalized();
}

void ZxcApplication::mouseMoveEvent(MouseMoveEvent& event) {
	if (!(event.buttons() & MouseMoveEvent::Button::Left)) return;

	const Vector3 currentPosition = positionOnSphere(event.position());
	const Vector3 axis = Math::cross(_previousPosition, currentPosition);

	if (_previousPosition.length() < 0.001f || axis.length() < 0.001f) return;

	_cameraObject.rotate(Math::angle(_previousPosition, currentPosition), axis.normalized());
	_previousPosition = currentPosition;

	redraw();
}

void ZxcApplication::keyPressEvent(Platform::Sdl2Application::KeyEvent &event) {
    if (event.key() == Magnum::Platform::Sdl2Application::KeyEvent::Key::J) {
        _unitObjects[0]->translate({0,-1,0});
        redraw();
    }
    if (event.key() == Magnum::Platform::Sdl2Application::KeyEvent::Key::K) {
        _unitObjects[0]->translate({0,1,0});
        redraw();
    }
    if (event.key() == Magnum::Platform::Sdl2Application::KeyEvent::Key::H) {
        _unitObjects[0]->translate({-1,0,0});
        redraw();
    }
    if (event.key() == Magnum::Platform::Sdl2Application::KeyEvent::Key::L) {
        _unitObjects[0]->translate({1,0,0});
        redraw();
    }

}

MAGNUM_APPLICATION_MAIN(ZxcApplication)
