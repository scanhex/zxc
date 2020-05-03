//#include <Magnum/SceneGraph/Drawable.h>
//
//#include "Game/Unit.h"
//#include "EventHandlers.h"
//
//void handleSkill(const Hero &hero,
//                 SkillNum skillNum,
//                 Scene3D &scene,
//                 Magnum::SceneGraph::DrawableGroup3D &group,
//                 Magnum::Timeline &timeline) {
//    double dist;
//    switch (skillNum) {
//        case SkillNum::first:
//            dist = 200;
//            break;
//        case SkillNum::second:
//            dist = 450;
//            break;
//        case SkillNum::third:
//            dist = 700;
//            break;
//        default:
//            Error{} << "Unknown skill type";
//    }
//    Object3D *obj = new Object3D{&scene};
//    dist /= 100; // TODO: remove this line after fixing game scale
//
//    auto shift = (Matrix4::rotationZ(Math::Rad{(float) hero.getAngle()}) *
//                  Matrix4::translation({0.f, (float) dist, 0.f})).translation();
//    obj->transform(
//            Matrix4::translation(shift + Vector3{(float) hero.getPosition().x_, (float) hero.getPosition().y_, 0.1f}));
//    Debug{} << "skill use" << shift;
//    new CoilDrawable(*obj, group, timeline);
//}
