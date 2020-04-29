#include "Game/Skill.h"
#include "Graphics/Drawables.h"
#include "Game/Unit.h"
#include <Magnum/SceneGraph/Drawable.h>


void handleSkill(Hero hero, SkillNum skillNum, Magnum::SceneGraph::DrawableGroup3D& group) {
	double dist;
	switch (skillNum) {
	case SkillNum::first:
		dist = 200;
	case SkillNum::second:
		dist = 450;
	case SkillNum::third:
		dist = 700;
		break;
	default:
		Error{} << "unknown skill type" << '\n';
	}
}
