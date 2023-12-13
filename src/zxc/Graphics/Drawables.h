#pragma once

#include "Game/Hero.h"
#include "Types.h"

#include <Magnum/GL/Mesh.h>
#include <Magnum/Math/Color.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Primitives/Circle.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/Shaders/Flat.h>
#include <Magnum/Shaders/Phong.h>
#include <Magnum/Shaders/Vector.h>
#include <Magnum/Text/AbstractFont.h>
#include <Magnum/Text/DistanceFieldGlyphCache.h>
#include <Magnum/Text/Renderer.h>
#include <Magnum/Timeline.h>
#include <Magnum/Trade/MeshData3D.h>

#include <iostream>

using namespace Magnum;
using namespace Math::Literals;

class UnitDrawable : public SceneGraph::Drawable3D {
public:
    explicit UnitDrawable(Object3D &object, SceneGraph::DrawableGroup3D &group, const Unit &unit);

private:
    void draw(const Matrix4 &transformationMatrix, SceneGraph::Camera3D &camera) override;

    //    GL::Mesh mesh_;
    Shaders::PhongGL shader_;
    Shaders::VectorGL3D hpShader_;
    Containers::Pointer<Text::Renderer2D> hpRenderer_;
    const Unit &unit_;

    Text::GlyphCache cache{Vector2i{512}};
};

class FlatDrawable : public SceneGraph::Drawable3D {
public:
    explicit FlatDrawable(
        Object3D &object,
        Shaders::FlatGL3D &shader,
        GL::Mesh &mesh,
        SceneGraph::DrawableGroup3D &drawables
    )
        : SceneGraph::Drawable3D{object, &drawables}, shader_(shader), mesh_(mesh) {}

    void draw(const Matrix4 &transformation, SceneGraph::Camera3D &camera) override;

private:
    Shaders::FlatGL3D &shader_;
    GL::Mesh &mesh_;
};

class ColoredDrawable : public SceneGraph::Drawable3D {
public:
    explicit ColoredDrawable(
        Object3D &object,
        Shaders::PhongGL &shader,
        GL::Mesh &mesh,
        const Color4 &color,
        SceneGraph::DrawableGroup3D &group
    )
        : SceneGraph::Drawable3D{object, &group}, shader_(shader), mesh_(mesh), color_{color} {}

private:
    void draw(const Matrix4 &transformationMatrix, SceneGraph::Camera3D &camera) override;

    Shaders::PhongGL &shader_;
    GL::Mesh &mesh_;
    Color4 color_;
};

class TexturedDrawable : public SceneGraph::Drawable3D {
public:
    explicit TexturedDrawable(
        Object3D &object,
        Shaders::PhongGL &shader,
        GL::Mesh &mesh,
        GL::Texture2D &texture,
        SceneGraph::DrawableGroup3D &group
    )
        : SceneGraph::Drawable3D{object, &group},
          shader_(shader),
          mesh_(std::move(mesh)),
          texture_(std::move(texture)) {}

private:
    void draw(const Matrix4 &transformationMatrix, SceneGraph::Camera3D &camera) override;

    Shaders::PhongGL &shader_;
    GL::Mesh mesh_;
    GL::Texture2D texture_;
};

constexpr float COIL_ANIMATION_DURATION = 0.4;

class CoilDrawable : public SceneGraph::Drawable3D {
public:
    // TODO Sasha why in header?? (emu poxui)
    explicit CoilDrawable(Object3D &object, SceneGraph::DrawableGroup3D &group, Magnum::Timeline &timeline)
        : SceneGraph::Drawable3D{object, &group},
          object_(object),
          timeline_(timeline),
          creationTime_(timeline_.previousFrameTime()) {
        Debug{} << "Coil create";
    }
    ~CoilDrawable() override {
        Debug{} << "Coil delete";
    }

private:
    void draw(const Matrix4 &transformationMatrix, SceneGraph::Camera3D &camera) override;
    Shaders::PhongGL shader_;
    Object3D &object_;
    Magnum::Timeline &timeline_;
    float creationTime_;
    GL::Mesh mesh_ = MeshTools::compile(Primitives::circle3DSolid(100));
};

class AttackDrawable : public SceneGraph::Drawable3D {
public:
    explicit AttackDrawable(Object3D &object, SceneGraph::DrawableGroup3D &group, const Attack &attack);
    ~AttackDrawable() override = default;

private:
    void draw(const Matrix4 &transformationMatrix, SceneGraph::Camera3D &camera) override;
    GL::Mesh mesh_;
    Shaders::PhongGL shader_;
    const Attack &attack_;
};
