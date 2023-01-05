#include "Drawables.h"

#include "Corrade/Utility/FormatStl.h"
#include "Magnum/GL/DefaultFramebuffer.h"
#include "Magnum/GL/Mesh.h"
#include "Magnum/Math/Color.h"
#include "Magnum/Math/Complex.h"
#include "Magnum/Platform/Sdl2Application.h"
#include "Magnum/Primitives/Circle.h"
#include "Magnum/Primitives/UVSphere.h"
#include "PluginLibrary.h"

Containers::Pointer<Text::AbstractFont> font;

UnitDrawable::UnitDrawable(Object3D &object, SceneGraph::DrawableGroup3D &group, const Unit &unit)
    : SceneGraph::Drawable3D(object, &group), unit_(unit) {
    if (!font) {
        font = PluginLibrary::getFontManager().loadAndInstantiate("StbTrueTypeFont");
    }
    if (!font || !font->openFile(RESOURCE_DIR "/arial.ttf", 180.0f)) {
        Fatal{} << "Cannot open font file";
    } else {
        std::cout << "Font loaded" << std::endl;
    }
    font->fillGlyphCache(cache, "0123456789.");
    hpRenderer_.reset(new Text::Renderer2D{*font, cache, 0.5f, Text::Alignment::LineCenter});
    hpRenderer_->reserve(50, GL::BufferUsage::DynamicDraw, GL::BufferUsage::StaticDraw);
}

void UnitDrawable::draw(const Matrix4 &transformationMatrix, SceneGraph::Camera3D &camera) {
    if (unit_.isDead()) {
        return;
    }
    if (unit_.getTeam() == Team::Dire) {
        hpShader_.setColor(0xe90000_rgbf);
    } else {
        hpShader_.setColor(0x1ac100_rgbf);
    }
    hpShader_
        .setTransformationProjectionMatrix(
            camera.projectionMatrix() * Matrix4::translation(transformationMatrix.translation()) *
            Matrix4::translation({0.f, 1.f, 5.f})
        )
        .bindVectorTexture(cache.texture());

    int32_t myHP = ceil(unit_.getHealthPoints());
    hpRenderer_->render(std::to_string(myHP));
    hpRenderer_->mesh().draw(hpShader_);
    // Debugging Circle under the character:
    /*
    auto mesh = MeshTools::compile(Magnum::Primitives::circle3DSolid(100));
    shader_.setDiffuseColor(0x1ac100_rgbf)
        .setLightPosition(camera.cameraMatrix().translation())
        .setLightColor(Color4(1.f, 1.f, 1.f))
        .setTransformationMatrix(Matrix4::translation({ 0.f, 0.f, 0.01f }) * transformationMatrix * Matrix4::scaling({
    0.64f, 0.64f, 0.64f })) .setNormalMatrix(transformationMatrix.normalMatrix())
        .setProjectionMatrix(camera.projectionMatrix());
    mesh.draw(shader_);
    */
}

void FlatDrawable::draw(const Matrix4 &transformation, SceneGraph::Camera3D &camera) {
    shader_.setColor(0x747474_rgbf).setTransformationProjectionMatrix(camera.projectionMatrix() * transformation);
    mesh_.draw(shader_);
}

void ColoredDrawable::draw(const Matrix4 &transformationMatrix, SceneGraph::Camera3D &camera) {
    shader_.setDiffuseColor(color_)
        .setLightPosition(camera.cameraMatrix().translation())
        .setLightColor(Color4(1.f, 1.f, 1.f))
        .setTransformationMatrix(transformationMatrix)
        .setNormalMatrix(transformationMatrix.normalMatrix())
        .setProjectionMatrix(camera.projectionMatrix());

    mesh_.draw(shader_);
}

void TexturedDrawable::draw(const Matrix4 &transformationMatrix, SceneGraph::Camera3D &camera) {
    shader_.setLightPosition({0, 0, 10})
        .setLightColor(Color4(1.f, 1.f, 1.f))
        .setTransformationMatrix(transformationMatrix)
        .setNormalMatrix(transformationMatrix.normalMatrix())
        .setProjectionMatrix(camera.projectionMatrix())
        .bindDiffuseTexture(texture_);

    mesh_.draw(shader_);
}

void CoilDrawable::draw(const Matrix4 &transformationMatrix, SceneGraph::Camera3D &camera) {
    if (timeline_.previousFrameTime() >= creationTime_ + COIL_ANIMATION_DURATION) {
        // We can't delete self, because Magnum is currently iterating over the DrawableGroup and it can cause RE
        //        delete &object_;
        return;
    }
    shader_.setDiffuseColor(Color4(0.8f, 0.273f, 0.191f))
        .setLightPosition({0, 0, 10})
        .setLightColor(Color4(1.f, 1.f, 1.f))
        .setTransformationMatrix(transformationMatrix)
        .setNormalMatrix(transformationMatrix.normalMatrix())
        .setProjectionMatrix(camera.projectionMatrix());

    shader_.draw(mesh_);
}

AttackDrawable::AttackDrawable(Object3D &object, SceneGraph::DrawableGroup3D &group, const Attack &attack)
    : SceneGraph::Drawable3D(object, &group), attack_(attack) {
    mesh_ = MeshTools::compile(Primitives::uvSphereSolid(100, 100));
}
void AttackDrawable::draw(const Matrix4 &transformationMatrix, SceneGraph::Camera3D &camera) {
    if (!attack_.getMovingFlag()) {
        return;
    }
    shader_.setDiffuseColor(0xa5c9ea_rgbf)
        .setLightPosition(camera.cameraMatrix().transformPoint({5.0f, 5.0f, 7.0f}))
        .setTransformationMatrix(transformationMatrix * Matrix4::scaling({0.4, 0.4, 0.4}))
        .setNormalMatrix(transformationMatrix.normalMatrix())
        .setProjectionMatrix(camera.projectionMatrix());
    shader_.draw(mesh_);
}
