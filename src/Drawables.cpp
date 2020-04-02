#include "Drawables.h"


void UnitDrawable::draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) {
    _shader.setDiffuseColor(0xa5c9ea_rgbf)
    .setLightPosition(camera.cameraMatrix().transformPoint(
    { 5.0f, 5.0f, 7.0f }))
    .setTransformationMatrix(transformationMatrix)
    .setNormalMatrix(transformationMatrix.normalMatrix())
    .setProjectionMatrix(camera.projectionMatrix());
    _mesh.draw(_shader);
}


void FlatDrawable::draw(const Matrix4& transformation, SceneGraph::Camera3D& camera) {
    _shader.setColor(0x747474_rgbf)
            .setTransformationProjectionMatrix(camera.projectionMatrix()*transformation);
    _mesh.draw(_shader);
}

void ColoredDrawable::draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) {
    _shader
            .setDiffuseColor(_color)
            .setLightPosition(camera.cameraMatrix().transformPoint({ -3.0f, 10.0f, 10.0f }))
            .setLightColor(Color4(255,255,255,255))
            .setTransformationMatrix(transformationMatrix)
            .setNormalMatrix(transformationMatrix.normalMatrix())
            .setProjectionMatrix(camera.projectionMatrix());

    _mesh.draw(_shader);
}

void TexturedDrawable::draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) {
    _shader
            .setLightPosition(camera.cameraMatrix().transformPoint({ -3.0f, 10.0f, 10.0f }))
            .setLightColor(Color4(255, 255, 255, 255))
            .setTransformationMatrix(transformationMatrix)
            .setNormalMatrix(transformationMatrix.normalMatrix())
            .setProjectionMatrix(camera.projectionMatrix())
            .bindDiffuseTexture(_texture);

    _mesh.draw(_shader);
}
