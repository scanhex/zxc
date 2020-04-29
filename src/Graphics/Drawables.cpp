#include "Drawables.h"

#include <Corrade/Utility/FormatStl.h>
#include <Corrade/Utility/Resource.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Complex.h>
#include <Magnum/Math/Matrix3.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Shaders/DistanceFieldVector.h>


PluginManager::Manager<Text::AbstractFont> manager;
Containers::Pointer<Text::AbstractFont> font;
UnitDrawable::UnitDrawable(Object3D& object, SceneGraph::DrawableGroup3D& group, const Unit& unit) : SceneGraph::Drawable3D(object, &group), _unit(unit)
{
    if (!font) {
        font = manager.loadAndInstantiate("StbTrueTypeFont");
    }
	if (!font || !font->openFile(RESOURCE_DIR "/arial.ttf", 180.0f))
		Fatal{} << "Cannot open font file";
	else std::cout << "Font loaded" << std::endl;
	font->fillGlyphCache(cache, "0123456789.");
    _hpRenderer.reset(new Text::Renderer2D{ *font, cache, 0.5f, Text::Alignment::LineCenter });
    _hpRenderer->reserve(50, GL::BufferUsage::DynamicDraw, GL::BufferUsage::StaticDraw);
}

void UnitDrawable::draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) {
    /*
    _shader.setDiffuseColor(0xa5c9ea_rgbf)
    .setLightPosition(camera.cameraMatrix().transformPoint(
    { 5.0f, 5.0f, 7.0f }))
    .setTransformationMatrix(transformationMatrix)
    .setNormalMatrix(transformationMatrix.normalMatrix())
    .setProjectionMatrix(camera.projectionMatrix());
    _mesh.draw(_shader);
    */
    _hpShader.setColor(0x00ff00_rgbf)
        .setTransformationProjectionMatrix(camera.projectionMatrix()  * Matrix4::translation(transformationMatrix.translation()) * Matrix4::translation({ 0.f, 1.f, 5.f }))
        .bindVectorTexture(cache.texture());
    int32_t myHP = ceil(_unit.getHealthPoints());
    _hpRenderer->render(std::to_string(myHP));
    _hpRenderer->mesh().draw(_hpShader);
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
		.setLightPosition(camera.cameraMatrix().translation())
		.setLightColor(Color4(3.f, 3.f, 3.f, 3.f))
		.setTransformationMatrix(transformationMatrix)
		.setNormalMatrix(transformationMatrix.normalMatrix())
		.setProjectionMatrix(camera.projectionMatrix())
		.bindDiffuseTexture(_texture);

    _mesh.draw(_shader);
}

void CoilDrawable::draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) {
    if (_timeline.previousFrameDuration() >= _creationTime + COIL_ANIMATION_DURATION) {
        delete &_object;
        delete this;
        return;
    }
    _shader
        .setDiffuseColor(Color4(1, 0, 0))
		.setLightPosition(camera.cameraMatrix().transformPoint({ -3.0f, 10.0f, 10.0f }))
		.setLightColor(Color4(3.f, 3.f, 3.f, 3.f))
		.setTransformationMatrix(transformationMatrix)
		.setNormalMatrix(transformationMatrix.normalMatrix())
		.setProjectionMatrix(camera.projectionMatrix());

    _mesh.draw(_shader);
}
