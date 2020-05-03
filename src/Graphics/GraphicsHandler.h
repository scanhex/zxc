#pragma once

#include "Graphics/Drawables.h"
#include "Events/Events.h"

struct GraphicsHandler : EventHandler<ShortCoilUseEvent>, EventHandler<MidCoilUseEvent>, EventHandler<LongCoilUseEvent> {
public:
	GraphicsHandler(Scene3D& scene, SceneGraph::DrawableGroup3D& drawables, Timeline& timeline);
private:
	Scene3D& scene_;
	SceneGraph::DrawableGroup3D& drawables_;
	Timeline& timeline_;
	void handleCoil(const SkillUseEvent& event, double dist);
	void handle(const ShortCoilUseEvent& event) override;
	void handle(const MidCoilUseEvent& event) override;
	void handle(const LongCoilUseEvent& event) override;
};

