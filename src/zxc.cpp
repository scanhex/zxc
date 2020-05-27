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

#include <Corrade/PluginManager/PluginManager.h>
#include <Magnum/Text/Text.h>


#include <iostream>
#include <cassert>

#include "Graphics/Types.h"
#include "Graphics/Drawables.h"
#include "Client/Client.h"
#include "Game/GameState.h"
#include "Graphics/ShaderLibrary.h"

#include "zxc.h"

#include "Graphics/PluginLibrary.h"

void ZxcApplication::initCamera() {
    /* Every scene needs a camera */
    /* (c) Confucius */
    constexpr float camHeight = 25;
    constexpr auto camAngle = Math::Deg<Float>{30};
    cameraObject_
            .setParent(&scene_)
            .translate(Vector3::zAxis(camHeight) - Vector3::yAxis(camHeight * Math::tan(camAngle))).rotateXLocal(
                    camAngle);
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
    (*grid).scale(Vector3{MAP_SIZE});
    new FlatDrawable{*grid, ShaderLibrary::flatShader(), grid_, drawables_};
}

void ZxcApplication::initScene() {
    initCamera();
    initRenderer();
    initGrid();
}

void ZxcApplication::initGame() {
    addUnit(*heroes_[0], RESOURCE_DIR "/nevermore.fbx", false);
    addUnit(*heroes_[1], RESOURCE_DIR "/nevermore.fbx", false);
    addUnit(*units_[2], RESOURCE_DIR "/yasher.fbx", true);
    addUnit(*units_[3], RESOURCE_DIR "/crocodil.fbx", true);
}

void ZxcApplication::initHandlers() {
    graphicsHandler_ = std::make_unique<GraphicsHandler>(scene_, drawables_, timeline_);
}


void ZxcApplication::initNetwork() {
    Client client(gameState_);
    networkThread_ = std::thread(&Client::run, client_);
}

void ZxcApplication::initUi() {
    ui_.emplace(PluginLibrary::getFontManager(), Vector2{300, 300}, windowSize(), framebufferSize(),
                Ui::defaultStyleConfiguration(), "»");
    uiGoldPlane_.emplace(*ui_);
}

ZxcApplication::ZxcApplication(const Arguments &arguments) :
        Platform::Application{arguments, Configuration{}
                .setTitle("ZXC")
                .setWindowFlags(Configuration::WindowFlag::Resizable)},
        units_{new Hero(Player::First),
               new Hero(Player::Second),
               new Creep(Team::Radiant),
               new Creep(Team::Dire)},
        heroes_{dynamic_cast<Hero *>(units_[0]),
                dynamic_cast<Hero *>(units_[1])},
        myHero_{*heroes_[0]},
        gameState_{units_},
        client_{gameState_} {

    setSwapInterval(1);

    initScene();
    initHandlers();
    initUi();
    initGame();
    initNetwork();
    createAttackDrawables();
    GoldChangedEvent(239).fire();

    timeline_.start();
}

void ZxcApplication::createAttackDrawables() {
    for (Unit *unit: units_) {
        std::vector<Object3D *> objects;
        for (Attack *attack:unit->myAttacks_) {
            auto *obj = new Object3D{&scene_};
            new AttackDrawable(*obj, drawables_, *attack);
            objects.push_back(obj);
        }
        attackObjects_.push_back(objects);
    }
}

void ZxcApplication::addUnit(const Unit &u, std::string filename, bool wtf) {
    unitObjects_.push_back(
            modelLoader_.loadModel(filename, scene_, drawables_, wtf).release());
    new UnitDrawable(*unitObjects_.back(), drawables_, u);
}

void ZxcApplication::updateGameState() {
    gameState_.update(static_cast<double>(timeline_.previousFrameDuration()) * 1000);

    for (size_t i = 0; i < unitObjects_.size(); i++) {
        const Point &position = units_[i]->getPosition();

        Vector3 vectorPosition(position.x_, position.y_, position.z_);
        if (units_[i]->getMovedFlag())
            unitObjects_[i]->translate(vectorPosition - unitObjects_[i]->transformation().translation());

        double angle = units_[i]->getAngle();
        auto matrixPosition = Matrix4::translation(unitObjects_[i]->transformationMatrix().translation());
        unitObjects_[i]->setTransformation(matrixPosition * Matrix4::rotationZ(Magnum::Rad(M_PI + angle)));
    }

    for (size_t i = 0; i < attackObjects_.size(); i++) {
        for (size_t j = 0; j < attackObjects_[i].size(); j++) {
            Attack *attack = units_[i]->myAttacks_[j];
            if (!attack->getMovingFlag())
                continue;
            const Point &position = attack->getPosition();

            Vector3 vectorPosition(position.x_, position.y_, position.z_);
            attackObjects_[i][j]->translate(vectorPosition - attackObjects_[i][j]->transformation().translation());
        }
    }
}

void ZxcApplication::drawEvent() {
    GL::defaultFramebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);

    updateGameState();
    EventHandler<DrawEvent>::fireEvent(DrawEvent());

    //  Fps counter in console
    //	Debug{} << 1 / timeline_.previousFrameDuration();

    camera_->draw(drawables_);
    ui_->draw();

    swapBuffers();
    redraw();
    timeline_.nextFrame();
}

void ZxcApplication::viewportEvent(ViewportEvent &event) {
    GL::defaultFramebuffer.setViewport({{}, event.framebufferSize()});
    camera_->setViewport(event.windowSize());
}

void ZxcApplication::mousePressEvent(MouseEvent &event) {
    if (event.button() == MouseEvent::Button::Middle) {
        previousPosition_ = intersectWithPlane(event.position(), {0, 0, 1});
        cameraMoving_ = true;
    }
    if (event.button() == MouseEvent::Button::Right) {
        auto newPosition = intersectWithPlane(event.position(), {0, 0, 1});
        // unitObjects_[0]->translate(newPosition - unitObjects_[0]->transformation().translation());

        double x = newPosition.x(), y = newPosition.y();

        EventHandler<MoveEvent>::fireEvent(MoveEvent(myHero_, x, y));

        redraw();
    }
}

void ZxcApplication::mouseReleaseEvent(MouseEvent &event) {
    if (event.button() == MouseEvent::Button::Middle) {
        previousPosition_ = Containers::NullOpt;
        cameraMoving_ = false;
    }
}

void ZxcApplication::mouseScrollEvent(MouseScrollEvent &event) {
    if (!event.offset().y()) return;

    /* Distance to origin */
    // const Float distance = cameraObject_.transformation().translation().z();

    /* Move 15% of the distance back or forward */
    auto coords = cameraObject_.transformationMatrix().translation();
    coords.x() = 0;
    coords.y() = 0;
    cameraObject_.translate(
            -coords * 0.15f * (event.offset().y() > 0 ? 1 : -1));
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

void ZxcApplication::mouseMoveEvent(MouseMoveEvent &event) {
    if (cameraMoving_ || (event.buttons() & MouseMoveEvent::Button::Middle)) {
        const Vector3 currentPosition = intersectWithPlane(event.position(), {0, 0, 1});
        if (!previousPosition_)
            previousPosition_ = currentPosition;
        else {
            auto delta = currentPosition - *previousPosition_;
            cameraObject_.translate(-delta);
            redraw();
        }
    }
}

void ZxcApplication::keyPressEvent(Platform::Sdl2Application::KeyEvent &event) {
    // QWERTY and Dvorak bindings
    if (event.key() == KeyEvent::Key::Space) { // If you are a Maksim without a mouse
        cameraMoving_ = true;
        previousPosition_ = Containers::NullOpt;
    }
    if (myHero_.isDead())
        return;
    if (event.key() == KeyEvent::Key::A) {
        Attack *attack = myHero_.attack(gameState_.getAllUnits());
        if (attack) {
            EventHandler<AttackEvent>::fireEvent(AttackEvent(attack->getAttacker()->unique_id_,
                                                             attack->getTarget()->unique_id_));
        }
        redraw();
    }
    if (event.key() == KeyEvent::Key::Z || event.key() == KeyEvent::Key::Semicolon) {
        if (myHero_.isSkillReady(SkillName::FirstSkill)) {
            EventHandler<FirstSkillUseEvent>::fireEvent(FirstSkillUseEvent(myHero_));
            redraw();
        }
    }
    if (event.key() == KeyEvent::Key::X || event.key() == KeyEvent::Key::Q) {
        if (myHero_.isSkillReady(SkillName::SecondSkill)) {
            EventHandler<SecondSkillUseEvent>::fireEvent(SecondSkillUseEvent(myHero_));
            redraw();
        }
    }
    if (event.key() == KeyEvent::Key::C || event.key() == KeyEvent::Key::J) {
        if (myHero_.isSkillReady(SkillName::ThirdSkill)) {
            EventHandler<ThirdSkillUseEvent>::fireEvent(ThirdSkillUseEvent(myHero_));
            redraw();
        }
    }
    if (event.key() == KeyEvent::Key::S || event.key() == KeyEvent::Key::O) {
        EventHandler<StopEvent>::fireEvent(StopEvent(myHero_));
        redraw();
    }
}

void ZxcApplication::keyReleaseEvent(KeyEvent &event) {
    if (event.key() == Magnum::Platform::Sdl2Application::KeyEvent::Key::Space) {
        cameraMoving_ = false;
        previousPosition_ = Containers::NullOpt;
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
