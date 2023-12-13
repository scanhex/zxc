#include "GraphicsHandler.h"

#include "Game/Unit.h"

GraphicsHandler::GraphicsHandler(Scene3D &scene, SceneGraph::DrawableGroup3D &drawables, Timeline &timeline)
    : scene_(scene), drawables_(drawables), timeline_(timeline) {}

void GraphicsHandler::handle(const AttackEvent &event) {
    // TODO do we need this handler?
}

void GraphicsHandler::handleCoil(const SkillUseEvent &event, double dist) {
    auto *obj = new Object3D{&scene_};
    dist /= 100;  // TODO: remove this line after fixing game scale

    auto shift = (Matrix4::rotationZ(Math::Rad{static_cast<float>(event.hero_.getAngle())}) *
                  Matrix4::translation({0.f, static_cast<float>(dist), 0.f}))
                     .translation();
    obj->transform(Matrix4::translation(
        shift +
        Vector3{
            static_cast<float>(event.hero_.getPosition().x_), static_cast<float>(event.hero_.getPosition().y_), 0.1f
        }
    ));
    Debug{} << "skill use" << shift;
    new CoilDrawable(*obj, drawables_, timeline_);
}

void GraphicsHandler::handle(const FirstSkillUseEvent &event) {
    handleCoil(event, 200);
}
void GraphicsHandler::handle(const SecondSkillUseEvent &event) {
    handleCoil(event, 450);
}
void GraphicsHandler::handle(const ThirdSkillUseEvent &event) {
    handleCoil(event, 700);
}
