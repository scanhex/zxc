#pragma once

#include "Types.h"

#include <string>

#include <Corrade/Containers/Array.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Shaders/Phong.h>
#include <Magnum/Trade/AbstractImporter.h>

using namespace Magnum;
using namespace Corrade;

class ModelLoader {
public:
    Containers::Array<Containers::Optional<GL::Mesh>> meshes_;
    Containers::Array<Containers::Optional<GL::Texture2D>> textures_;
    Pointer<Object3D> loadModel(std::string filename, Scene3D &scene, SceneGraph::DrawableGroup3D &drawables, bool wtf);
    void addObject(
        Trade::AbstractImporter &importer,
        Containers::ArrayView<const Containers::Optional<Trade::PhongMaterialData>> materials,
        Object3D &parent,
        UnsignedInt i,
        SceneGraph::DrawableGroup3D &drawables,
        bool wtf
    );
};
