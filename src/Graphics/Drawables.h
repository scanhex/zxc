#pragma once

#include <Magnum/Shaders/Phong.h>
#include <Magnum/Shaders/Flat.h>
#include <Magnum/Shaders/Vector.h>
#include <Magnum/Math/Color.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/Trade/MeshData3D.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Primitives/Circle.h>
#include <Magnum/Text/AbstractFont.h>
#include <Magnum/Text/DistanceFieldGlyphCache.h>
#include <Magnum/Text/Renderer.h>
#include <Magnum/Timeline.h>
#include <Corrade/PluginManager/Manager.h>
#include "../Game/Hero.h"

#include<iostream>

using namespace Magnum;
using namespace Math::Literals;

typedef SceneGraph::Object<SceneGraph::MatrixTransformation3D> Object3D;
typedef SceneGraph::Scene<SceneGraph::MatrixTransformation3D> Scene3D;


class UnitDrawable : public SceneGraph::Drawable3D
{
public:
    explicit UnitDrawable(Object3D& object, SceneGraph::DrawableGroup3D& group, const Unit& unit);
    ~UnitDrawable() override {
//        std::cerr<<"Bye from UnitDrawable!"<<'\n';
    }

private:
    void draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) override;

    //    GL::Mesh _mesh;
    Shaders::Phong _shader;
    Shaders::Vector3D _hpShader;
    Containers::Pointer<Text::Renderer2D> _hpRenderer;
    const Unit& _unit;

	Text::GlyphCache cache{ Vector2i{512} };
};

class FlatDrawable: public SceneGraph::Drawable3D {
public:
    explicit FlatDrawable(Object3D& object, Shaders::Flat3D& shader, GL::Mesh& mesh, SceneGraph::DrawableGroup3D& drawables): SceneGraph::Drawable3D{object, &drawables}, _shader(shader), _mesh(mesh) {}

    void draw(const Matrix4& transformation, SceneGraph::Camera3D& camera);

private:
    Shaders::Flat3D& _shader;
    GL::Mesh& _mesh;
};

class ColoredDrawable : public SceneGraph::Drawable3D {
public:
    explicit ColoredDrawable(Object3D& object, Shaders::Phong& shader, GL::Mesh& mesh, const Color4& color, SceneGraph::DrawableGroup3D& group) : SceneGraph::Drawable3D{ object, &group }, _shader(shader), _mesh(mesh), _color{ color } {}

private:
    void draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) override;

    Shaders::Phong& _shader;
    GL::Mesh& _mesh;
    Color4 _color;
};

class TexturedDrawable : public SceneGraph::Drawable3D {
public:
    explicit TexturedDrawable(Object3D& object, Shaders::Phong& shader, GL::Mesh& mesh, GL::Texture2D& texture, SceneGraph::DrawableGroup3D& group) : SceneGraph::Drawable3D{ object, &group }, _shader(shader), _mesh(mesh), _texture(texture) {}

private:
    void draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) override;

    Shaders::Phong& _shader;
    GL::Mesh& _mesh;
    GL::Texture2D& _texture;
};

constexpr float COIL_ANIMATION_DURATION = 0.4;

class CoilDrawable : public SceneGraph::Drawable3D {
public:
	explicit CoilDrawable(Object3D& object, SceneGraph::DrawableGroup3D& group, Magnum::Timeline& timeline) : SceneGraph::Drawable3D{ object, &group }, _object(object),  _timeline(timeline), _creationTime(_timeline.previousFrameTime()) {
        Debug{} << "Coil create";
	}
    ~CoilDrawable() {
        Debug{} << "Coil delete";
    }
private:
    void draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) override;
    Shaders::Phong _shader;
    Object3D& _object;
    Magnum::Timeline& _timeline;
    float _creationTime;
	GL::Mesh _mesh = MeshTools::compile(Primitives::circle3DSolid(100));
};
