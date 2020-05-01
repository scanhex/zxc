#pragma once
#include "Game/Skill.h"
#include "Graphics/Drawables.h"


void handleSkill(const Hero& hero, SkillNum skillNum, Scene3D& scene, Magnum::SceneGraph::DrawableGroup3D& group, Magnum::Timeline& timeline);
