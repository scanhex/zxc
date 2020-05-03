#include <Corrade/Containers/Array.h>
#include <Corrade/Containers/Optional.h>
#include <Corrade/PluginManager/Manager.h>
#include <Corrade/Utility/Arguments.h>
#include <Corrade/Utility/DebugStl.h>
#include <Magnum/ImageView.h>
#include <Magnum/Math/Math.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/FunctionsBatch.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/PixelFormat.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/Primitives/Grid.h>
#include <Magnum/Trade/SceneData.h>
#include <Magnum/Timeline.h>
#include <Magnum/Magnum.h>
#include <Magnum/Image.h>

#include <iostream>
#include <cassert>

#include <boost/lockfree/queue.hpp>

#include "Graphics/Types.h"
#include "Graphics/Drawables.h"
#include "Client/Client.h"
#include "Game/GameState.h"
#include "Graphics/ShaderLibrary.h"

#include "zxc.h"


void ZxcApplication::initCamera() {
    /* Every scene needs a camera */
    /* (c) Confucius */
    cameraObject_
            .setParent(&scene_)
            .translate(Vector3::zAxis(30.0f));
    (*(camera_ = new SceneGraph::Camera3D{cameraObject_}))
            .setAspectRatioPolicy(SceneGraph::AspectRatioPolicy::Extend)
            .setProjectionMatrix(Matrix4::perspectiveProjection(35.0_degf, 1.0f, 0.01f, 1000.0f))
            .setViewport(GL::defaultFramebuffer.viewport().size());
}


void ZxcApplication::initRenderer() {
    /* Setup renderer and shader defaults */
    GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
    GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);
    /* Setup blending for transparent text background */
    GL::Renderer::enable(GL::Renderer::Feature::Blending);
    GL::Renderer::setBlendFunction(
            GL::Renderer::BlendFunction::One, /* or SourceAlpha for non-premultiplied */
            GL::Renderer::BlendFunction::OneMinusSourceAlpha);
}

void ZxcApplication::initGrid() {
    grid_ = MeshTools::compile(Primitives::grid3DSolid({15, 15}));
    auto grid = new Object3D{&scene_};
    (*grid).scale(Vector3{8});
    new FlatDrawable{*grid, ShaderLibrary::flatShader(), grid_, drawables_};
}

void ZxcApplication::initScene() {
    initCamera();
    initRenderer();
    /* Setup shaders */

    initGrid();
}

void ZxcApplication::initGame() {
    addUnit(firstHero_);
    addUnit(secondHero_);
}

void ZxcApplication::initHandlers() {
    graphicsHandler_ = std::make_unique<GraphicsHandler>(scene_, drawables_, timeline_);
}

void ZxcApplication::initNetwork()
{
	networkThread_ = std::thread(runClient, std::ref(gameState_));
}

ZxcApplication::ZxcApplication(const Arguments &arguments) :
        Platform::Application{arguments, Configuration{}
                .setTitle("ZXC")
                .setWindowFlags(Configuration::WindowFlag::Resizable)},
        firstHero_{Hero(Player::First)},
        secondHero_{Hero(Player::Second)},
        gameState_(GameState(firstHero_, secondHero_)) {

    setSwapInterval(1);

    initScene();
    initGame();
    initHandlers();
    initNetwork();

    timeline_.start();



}

void ZxcApplication::addUnit(Unit &u) {
    unitObjects_.push_back(
            modelLoader_.loadModel(RESOURCE_DIR "/nevermore_blender_raw.fbx", scene_, drawables_).release());
    new UnitDrawable(*unitObjects_.back(), drawables_, u);
}

void ZxcApplication::updateGameState() {
    double myAngle = gameState_.getAngle(Player::First);
    double otherAngle = gameState_.getAngle(Player::Second);

    gameState_.update(static_cast<double>(timeline_.previousFrameDuration()) * 1000);

    Point myPosition = gameState_.getPosition(Player::First);
    Point otherPosition = gameState_.getPosition(Player::Second);
    Vector3 myVectorPosition(myPosition.x_, myPosition.y_, myPosition.z_);
    Vector3 otherVectorPosition(otherPosition.x_, otherPosition.y_, otherPosition.z_);

    unitObjects_[0]->translate(myVectorPosition - unitObjects_[0]->transformation().translation());
    unitObjects_[1]->translate(otherVectorPosition - unitObjects_[1]->transformation().translation());

    double myNewAngle = gameState_.getAngle(Player::First);
    double otherNewAngle = gameState_.getAngle(Player::Second);

    if (myNewAngle != myAngle) {
        double delta = myNewAngle - myAngle;
        if (delta <= -M_PI) delta += 2 * M_PI;
        if (delta >= M_PI) delta -= 2 * M_PI;
        unitObjects_[0]->rotateLocal(Math::Rad<float>(delta), Math::Vector3{0.0f, 0.0f, 1.0f});
    }

    if (otherNewAngle != otherAngle) {
        double delta = otherNewAngle - otherAngle;
        if (delta <= -M_PI) delta += 2 * M_PI;
        if (delta >= M_PI) delta -= 2 * M_PI;
        unitObjects_[1]->rotateLocal(Math::Rad<float>(delta), Math::Vector3{0.0f, 0.0f, 1.0f});
    }
}

void ZxcApplication::drawEvent() {
    GL::defaultFramebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);

    updateGameState();

//  Fps counter in console
//	Debug{} << 1 / timeline_.previousFrameDuration();

    camera_->draw(drawables_);

    swapBuffers();
    redraw();
    timeline_.nextFrame();
}

void ZxcApplication::viewportEvent(ViewportEvent &event) {
    GL::defaultFramebuffer.setViewport({{}, event.framebufferSize()});
    camera_->setViewport(event.windowSize());
}

void ZxcApplication::mousePressEvent(MouseEvent &event) {
    if (event.button() == MouseEvent::Button::Left)
        previousPosition_ = positionOnSphere(event.position());
    if (event.button() == MouseEvent::Button::Right) {
        auto newPosition = intersectWithPlane(event.position(), {0, 0, 1});
        unitObjects_[0]->translate(newPosition - unitObjects_[0]->transformation().translation());

        double x = newPosition.x(), y = newPosition.y();

        EventHandler<MoveEvent>::fireEvent(MoveEvent(firstHero_,x,y));

        redraw();
    }
}

void ZxcApplication::mouseReleaseEvent(MouseEvent &event) {
    if (event.button() == MouseEvent::Button::Left)
        previousPosition_ = Vector3();
}

void ZxcApplication::mouseScrollEvent(MouseScrollEvent &event) {
    if (!event.offset().y()) return;

    /* Distance to origin */
    // const Float distance = cameraObject_.transformation().translation().z();

    /* Move 15% of the distance back or forward */
    cameraObject_.translate(
            -cameraObject_.transformationMatrix().translation() * 0.15f * (event.offset().y() > 0 ? 1 : -1));
//	cameraObject_.translate(Vector3::zAxis(
//		distance * (1.0f - (event.offset().y() > 0 ? 1 / 0.85f : 0.85f))));

    redraw();
}

//Assuming plane contains zero
Vector3 ZxcApplication::intersectWithPlane(const Vector2i &windowPosition, const Vector3 &planeNormal) const {
    /* We have to take window size, not framebuffer size, since the position is
       in window coordinates and the two can be different on HiDPI systems */
    const Vector2i viewSize = windowSize();
    const Vector2i viewPosition{windowPosition.x(), viewSize.y() - windowPosition.y() - 1};
    const Vector3 ray_nds{2 * Vector2{viewPosition} / Vector2{viewSize} - Vector2{1.0f}, 1};
    const Vector4 ray_clip{ray_nds.x(), ray_nds.y(), -1, 1};
    Vector4 ray_eye = camera_->projectionMatrix().inverted() * ray_clip;
    ray_eye.z() = -1, ray_eye.w() = 0;
    Vector3 ray_world = (camera_->cameraMatrix().inverted() * ray_eye).xyz().normalized();
    const Float dist = -Math::dot(cameraObject_.absoluteTransformation().translation(), planeNormal) /
                       Math::dot(ray_world, planeNormal);
    return cameraObject_.absoluteTransformation().translation() + ray_world * dist;
}

Float ZxcApplication::depthAt(const Vector2i &windowPosition) const {
    /* First scale the position from being relative to window size to being
       relative to framebuffer size as those two can be different on HiDPI
       systems */
    const Vector2i position = windowPosition * Vector2{framebufferSize()} / Vector2{windowSize()};
    const Vector2i fbPosition{position.x(), GL::defaultFramebuffer.viewport().sizeY() - position.y() - 1};

    GL::defaultFramebuffer.mapForRead(GL::DefaultFramebuffer::ReadAttachment::Front);
    Image2D data = GL::defaultFramebuffer.read(
            Range2Di::fromSize(fbPosition, Vector2i{1}).padded(Vector2i{2}),
            {GL::PixelFormat::DepthComponent, GL::PixelType::Float});

    return Math::min<Float>(Containers::arrayCast<const Float>(data.data()));
}

Vector3 ZxcApplication::unproject(const Vector2i &windowPosition, Float depth) const {
    /* We have to take window size, not framebuffer size, since the position is
       in window coordinates and the two can be different on HiDPI systems */
    const Vector2i viewSize = windowSize();
    const Vector2i viewPosition{windowPosition.x(), viewSize.y() - windowPosition.y() - 1};
    const Vector3 in{2 * Vector2{viewPosition} / Vector2{viewSize} - Vector2{1.0f}, depth * 2.0f - 1.0f};


    return (cameraObject_.absoluteTransformationMatrix() * camera_->projectionMatrix().inverted()).transformPoint(in);
    /*
  Use the following to get the camera-relative coordinates instead of global:
//    return camera_->projectionMatrix().inverted().transformPoint(in);
       */
}

Vector3 ZxcApplication::positionOnSphere(const Vector2i &position) const {
    const Vector2 positionNormalized = Vector2{position} / Vector2{camera_->viewport()} - Vector2{0.5f};
    const Float length = positionNormalized.length();
    const Vector3 result(
            length > 1.0f ? Vector3(positionNormalized, 0.0f) : Vector3(positionNormalized, 1.0f - length));
    return (result * Vector3::yScale(-1.0f)).normalized();
}

void ZxcApplication::mouseMoveEvent(MouseMoveEvent &event) {
    if (!(event.buttons() & MouseMoveEvent::Button::Left)) return;

    const Vector3 currentPosition = positionOnSphere(event.position());
    const Vector3 axis = Math::cross(previousPosition_, currentPosition);

    if (previousPosition_.length() < 0.001f || axis.length() < 0.001f) return;

    cameraObject_.rotate(Math::angle(previousPosition_, currentPosition), axis.normalized());
    previousPosition_ = currentPosition;

    redraw();
}

void ZxcApplication::keyPressEvent(Platform::Sdl2Application::KeyEvent &event) {
    if (event.key() == Magnum::Platform::Sdl2Application::KeyEvent::Key::Z) {
        EventHandler<ShortCoilUseEvent>::fireEvent(ShortCoilUseEvent(firstHero_));
        redraw();
    }
    if (event.key() == Magnum::Platform::Sdl2Application::KeyEvent::Key::X) {
        EventHandler<MidCoilUseEvent>::fireEvent(MidCoilUseEvent(firstHero_));
        redraw();
    }
    if (event.key() == Magnum::Platform::Sdl2Application::KeyEvent::Key::C) {
        EventHandler<LongCoilUseEvent>::fireEvent(LongCoilUseEvent(firstHero_));
        redraw();
    }
}

void ZxcApplication::exitEvent(ExitEvent &event) {
    event.setAccepted();
    exit_flag = true;
    std::cout << "Vi v adekvate?" << '\n';
    std::cout << "net" << std::endl;
    networkThread_.join();
    exit(0);
}

MAGNUM_APPLICATION_MAIN(ZxcApplication)
