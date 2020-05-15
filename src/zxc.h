#pragma once

#include <Corrade/PluginManager/Manager.h>
#include <Magnum/ImageView.h>
#include <Magnum/Math/Math.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/Timeline.h>
#include <Magnum/Magnum.h>


#include <boost/lockfree/queue.hpp>

#include "Graphics/Types.h"
#include "Graphics/ModelLoader.h"
#include "Graphics/GraphicsHandler.h"
#include "Client/Client.h"
#include "Game/GameState.h"

using namespace Magnum;
using namespace Corrade;

using namespace Math::Literals;


bool exit_flag = false;

class ZxcApplication : public Platform::Application {
public:
    explicit ZxcApplication(const Arguments &arguments);

private:

    void drawEvent() override;
    void viewportEvent(ViewportEvent &event) override;
    void mousePressEvent(MouseEvent &event) override;
    void mouseReleaseEvent(MouseEvent &event) override;
    void mouseMoveEvent(MouseMoveEvent &event) override;
    void mouseScrollEvent(MouseScrollEvent &event) override;
    void exitEvent(ExitEvent &event) override;

    void keyPressEvent(KeyEvent &event) override;

    Vector3 positionOnSphere(const Vector2i &position) const;

    void updateGameState();

    void addUnit(const Unit &u, std::string filename, bool wtf);
    void initCamera();
    void initRenderer();
    void initGrid();
    void initScene();
    void initGame();
    void initHandlers();
    void initNetwork();

    Float depthAt(const Vector2i &position) const;
    Vector3 unproject(const Vector2i &position, Float depth) const;
    Vector3 intersectWithPlane(const Vector2i &windowPosition, const Vector3 &planeNormal) const;

    ShaderLibrary shaderLibrary_;
    ModelLoader modelLoader_;

    Hero heroes_[NUM_PLAYERS];
    Hero &myHero_; // just a ref to heroes[0]

    Creep creeps_[NUM_PLAYERS];
    GameState gameState_;

    std::thread networkThread_;

    std::vector<Object3D *> unitObjects_;

    Scene3D scene_;
    Object3D cameraObject_;
    SceneGraph::Camera3D *camera_ = nullptr;
    Object3D mapObject_;
    SceneGraph::DrawableGroup3D drawables_;
    Vector3 previousPosition_;

    GL::Mesh grid_;
    Magnum::Timeline timeline_;
    std::unique_ptr<GraphicsHandler> graphicsHandler_;

    Client client_;
};
