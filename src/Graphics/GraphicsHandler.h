#pragma once

#include "Graphics/Drawables.h"
#include "Events/Events.h"

struct GraphicsHandler : EventHandler<FirstSkillUseEvent>, EventHandler<SecondSkillUseEvent>, EventHandler<ThirdSkillUseEvent> {
public:
	GraphicsHandler(Scene3D& scene, SceneGraph::DrawableGroup3D& drawables, Timeline& timeline);
private:
	Scene3D& scene_;
	SceneGraph::DrawableGroup3D& drawables_;
	Timeline& timeline_;
	void handleCoil(const SkillUseEvent& event, double dist);
	void handle(const FirstSkillUseEvent& event) override;
	void handle(const SecondSkillUseEvent& event) override;
	void handle(const ThirdSkillUseEvent& event) override;
};

