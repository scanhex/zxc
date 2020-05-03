#include <Magnum/SceneGraph/Drawable.h>

#include "Game/Unit.h"
#include "GraphicsHandler.h"

GraphicsHandler::GraphicsHandler(Scene3D& scene, SceneGraph::DrawableGroup3D& drawables, Timeline& timeline) : scene_(scene), drawables_(drawables), timeline_(timeline) {}

void GraphicsHandler::handleCoil(const SkillUseEvent& event, double dist) {
	Object3D* obj = new Object3D{ &scene_ };
	dist /= 100; // TODO: remove this line after fixing game scale

	auto shift = (Matrix4::rotationZ(Math::Rad{ (float)event.hero_.getAngle() }) * Matrix4::translation({ 0.f, (float)dist, 0.f })).translation();
	obj->transform(Matrix4::translation(shift + Vector3{ (float)event.hero_.getPosition().x_, (float)event.hero_.getPosition().y_, 0.1f }));
	Debug{} << "skill use" << shift;
	new CoilDrawable(*obj, drawables_, timeline_);
}

void GraphicsHandler::handle(const ShortCoilUseEvent& event) {
	handleCoil(event, 200);
}
void GraphicsHandler::handle(const MidCoilUseEvent& event) {
	handleCoil(event, 450);
}
void GraphicsHandler::handle(const LongCoilUseEvent& event) {
	handleCoil(event, 700);
	
}

