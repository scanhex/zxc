#pragma once

#include <Magnum/Shaders/Phong.h>
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
#include "Game/Hero.h"

#include<iostream>

using namespace Magnum;

typedef SceneGraph::Object<SceneGraph::MatrixTransformation3D> Object3D;
typedef SceneGraph::Scene<SceneGraph::MatrixTransformation3D> Scene3D;

class UnitDrawable : public SceneGraph::Drawable3D
{
public:
    explicit UnitDrawable(Object3D& object, SceneGraph::DrawableGroup3D& group, const Unit& unit) : SceneGraph::Drawable3D(object, &group), _unit(unit)
    {
        _mesh = MeshTools::compile(Primitives::cubeSolid());
    }
    ~UnitDrawable() override {
        std::cerr<<"Bye from UnitDrawable!"<<'\n';
    }

private:
    void draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) override {
        using namespace Math::Literals;

        _shader.setDiffuseColor(0xa5c9ea_rgbf)
                .setLightPosition(camera.cameraMatrix().transformPoint(
                        { 5.0f, 5.0f, 7.0f }))
                .setTransformationMatrix(transformationMatrix)
                .setNormalMatrix(transformationMatrix.normalMatrix())
                .setProjectionMatrix(camera.projectionMatrix());
        _mesh.draw(_shader);
    }

    GL::Mesh _mesh;
    Shaders::Phong _shader;
    const Unit& _unit;
};
