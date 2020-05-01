#pragma once
#include <Corrade/PluginManager/Manager.h>
#include <Magnum/ImageView.h>
#include <Magnum/Math/Math.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/Trade/SceneData.h>
#include <Magnum/Timeline.h>
#include <Magnum/Magnum.h>


#include <boost/lockfree/queue.hpp>

#include "Graphics/Types.h"
#include "Graphics/EventHandlers.h"
#include "Client/Client.h"
#include "Game/GameState.h"

using namespace Magnum;
using namespace Corrade;

using namespace Math::Literals;


boost::lockfree::queue<Event> events{ 100 };
bool exit_flag = false;

class ZxcApplication : public Platform::Application {
public:
	explicit ZxcApplication(const Arguments& arguments);

private:

	void drawEvent() override;
	void viewportEvent(ViewportEvent& event) override;
	void mousePressEvent(MouseEvent& event) override;
	void mouseReleaseEvent(MouseEvent& event) override;
	void mouseMoveEvent(MouseMoveEvent& event) override;
	void mouseScrollEvent(MouseScrollEvent& event) override;
	void exitEvent(ExitEvent& event) override;

	void keyPressEvent(KeyEvent& event) override;

	Vector3 positionOnSphere(const Vector2i& position) const;

    void updateGameState();

	void addUnit(Unit& u);
	void initCamera();
	void initRenderer();
	void initGrid();
	void initScene();
	void initGame();

	Float depthAt(const Vector2i& position) const;
	Vector3 unproject(const Vector2i& position, Float depth) const;
	Vector3 intersectWithPlane(const Vector2i& windowPosition, const Vector3& planeNormal) const;

	ShaderLibrary _shaderLibrary;
    std::optional<GameState> _gameState;
	std::optional<Hero> _firstHero;
	std::optional<Hero> _secondHero;

	std::thread network_thread;

	std::vector<Object3D*> _unitObjects;

	Scene3D _scene;
	Object3D _cameraObject;
	SceneGraph::Camera3D* _camera = nullptr;
	Object3D _mapObject;
	SceneGraph::DrawableGroup3D _drawables;
	Vector3 _previousPosition;

	GL::Mesh _grid;
	Magnum::Timeline _timeline;
};
