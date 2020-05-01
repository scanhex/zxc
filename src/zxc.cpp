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
#include "Graphics/EventHandlers.h"
#include "Client/Client.h"
#include "Game/GameState.h"
#include "Graphics/ModelLoader.h"
#include "Graphics/ShaderLibrary.h"

#include "zxc.h"


void ZxcApplication::initCamera() {
	/* Every scene needs a camera */
	/* (c) Confucius */
	_cameraObject
		.setParent(&_scene)
		.translate(Vector3::zAxis(30.0f));
	(*(_camera = new SceneGraph::Camera3D{ _cameraObject }))
		.setAspectRatioPolicy(SceneGraph::AspectRatioPolicy::Extend)
		.setProjectionMatrix(Matrix4::perspectiveProjection(35.0_degf, 1.0f, 0.01f, 1000.0f))
		.setViewport(GL::defaultFramebuffer.viewport().size());
}


void ZxcApplication::initRenderer()
{
	/* Setup renderer and shader defaults */
	GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
	GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);
	/* Setup blending for transparent text background */
	GL::Renderer::enable(GL::Renderer::Feature::Blending);
	GL::Renderer::setBlendFunction(
		GL::Renderer::BlendFunction::One, /* or SourceAlpha for non-premultiplied */
		GL::Renderer::BlendFunction::OneMinusSourceAlpha);
}

void ZxcApplication::initGrid()
{
	_grid = MeshTools::compile(Primitives::grid3DSolid({ 15, 15 }));
	auto grid = new Object3D{ &_scene };
	(*grid).scale(Vector3{ 8 });
	new FlatDrawable{ *grid, ShaderLibrary::flatShader(), _grid, _drawables };
}

void ZxcApplication::initScene() {
	initCamera();
	initRenderer();
	ShaderLibrary::initShaders();
	/* Setup shaders */
	/*
	*/

	initGrid();
}

void ZxcApplication::initGame(){
    _firstHero = Hero(Player::First);
    _secondHero = Hero(Player::Second);

    _gameState = GameState(*_firstHero, *_secondHero);

    addUnit(*_firstHero);
    addUnit(*_secondHero);
}

ZxcApplication::ZxcApplication(const Arguments& arguments) :
	Platform::Application{ arguments, Configuration{}
		.setTitle("ZXC")
		.setWindowFlags(Configuration::WindowFlag::Resizable) }

{

	setSwapInterval(1);
	_timeline.start();
	initScene();
    initGame();

	network_thread = std::thread(runClient, std::ref(_gameState.value()));


}

void ZxcApplication::addUnit(Unit& u) {
	_unitObjects.push_back(ModelLoader::loadModel(RESOURCE_DIR "/nevermore_blender_raw.fbx", _scene, _drawables).release());
	new UnitDrawable(*_unitObjects.back(), _drawables, u);
}

void ZxcApplication::updateGameState(){
    double myAngle = _gameState->getAngle(Player::First);
    double otherAngle = _gameState->getAngle(Player::Second);

	_gameState->update(_timeline.previousFrameDuration() * 1000);

    Point myPosition = _gameState->getPosition(Player::First);
    Point otherPosition = _gameState->getPosition(Player::Second);
    Vector3 myVectorPosition(myPosition.x_, myPosition.y_, myPosition.z_);
    Vector3 otherVectorPosition(otherPosition.x_, otherPosition.y_, otherPosition.z_);

    _unitObjects[0]->translate(myVectorPosition - _unitObjects[0]->transformation().translation());
    _unitObjects[1]->translate(otherVectorPosition - _unitObjects[1]->transformation().translation());

    double myNewAngle = _gameState->getAngle(Player::First);
    double otherNewAngle = _gameState->getAngle(Player::Second);

    if (myNewAngle != myAngle) {
        double delta = myNewAngle - myAngle;
        if (delta <= -M_PI) delta += 2 * M_PI;
        if (delta >= M_PI) delta -= 2 * M_PI;
        _unitObjects[0]->rotateLocal(Math::Rad<float>(delta), Math::Vector3{0.0f, 0.0f, 1.0f});
    }

    if (otherNewAngle != otherAngle) {
        double delta = otherNewAngle - otherAngle;
        if (delta <= -M_PI) delta += 2 * M_PI;
        if (delta >= M_PI) delta -= 2 * M_PI;
        _unitObjects[1]->rotateLocal(Math::Rad<float>(delta), Math::Vector3{0.0f, 0.0f, 1.0f});
    }
}

void ZxcApplication::drawEvent() {
	GL::defaultFramebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);

    updateGameState();

//  Fps counter in console
//	Debug{} << 1 / _timeline.previousFrameDuration();

	_camera->draw(_drawables);

	swapBuffers();
	redraw();
	_timeline.nextFrame();
}

void ZxcApplication::viewportEvent(ViewportEvent& event) {
	GL::defaultFramebuffer.setViewport({ {}, event.framebufferSize() });
	_camera->setViewport(event.windowSize());
}

void ZxcApplication::mousePressEvent(MouseEvent& event) {
	if (event.button() == MouseEvent::Button::Left)
		_previousPosition = positionOnSphere(event.position());
	if (event.button() == MouseEvent::Button::Right) {
		auto newPosition = intersectWithPlane(event.position(), { 0,0,1 });
		_unitObjects[0]->translate(newPosition - _unitObjects[0]->transformation().translation());

		double x = newPosition.x(), y = newPosition.y();

		Event curEvent(EventName::move, Player::First, x, y);
		events.push(curEvent);
		_gameState->applyEvent(curEvent);

		redraw();
	}
}

void ZxcApplication::mouseReleaseEvent(MouseEvent& event) {
	if (event.button() == MouseEvent::Button::Left)
		_previousPosition = Vector3();
}

void ZxcApplication::mouseScrollEvent(MouseScrollEvent& event) {
	if (!event.offset().y()) return;

	/* Distance to origin */
	const Float distance = _cameraObject.transformation().translation().z();

	/* Move 15% of the distance back or forward */
	_cameraObject.translate(-_cameraObject.transformationMatrix().translation() * 0.15f * (event.offset().y() > 0 ? 1 : -1) );
//	_cameraObject.translate(Vector3::zAxis(
//		distance * (1.0f - (event.offset().y() > 0 ? 1 / 0.85f : 0.85f))));

	redraw();
}

//Assuming plane contains zero
Vector3 ZxcApplication::intersectWithPlane(const Vector2i& windowPosition, const Vector3& planeNormal) const {
	/* We have to take window size, not framebuffer size, since the position is
	   in window coordinates and the two can be different on HiDPI systems */
	const Vector2i viewSize = windowSize();
	const Vector2i viewPosition{ windowPosition.x(), viewSize.y() - windowPosition.y() - 1 };
	const Vector3 ray_nds{ 2 * Vector2{viewPosition} / Vector2{viewSize} -Vector2{1.0f}, 1 };
	const Vector4 ray_clip{ ray_nds.x(), ray_nds.y(), -1, 1 };
	Vector4 ray_eye = _camera->projectionMatrix().inverted() * ray_clip;
	ray_eye.z() = -1, ray_eye.w() = 0;
	Vector3 ray_world = (_camera->cameraMatrix().inverted() * ray_eye).xyz().normalized();
	const Float dist = -Math::dot(_cameraObject.absoluteTransformation().translation(), planeNormal) / Math::dot(ray_world, planeNormal);
	return _cameraObject.absoluteTransformation().translation() + ray_world * dist;
}

Float ZxcApplication::depthAt(const Vector2i& windowPosition) const {
	/* First scale the position from being relative to window size to being
	   relative to framebuffer size as those two can be different on HiDPI
	   systems */
	const Vector2i position = windowPosition * Vector2{ framebufferSize() } / Vector2{ windowSize() };
	const Vector2i fbPosition{ position.x(), GL::defaultFramebuffer.viewport().sizeY() - position.y() - 1 };

	GL::defaultFramebuffer.mapForRead(GL::DefaultFramebuffer::ReadAttachment::Front);
	Image2D data = GL::defaultFramebuffer.read(
		Range2Di::fromSize(fbPosition, Vector2i{ 1 }).padded(Vector2i{ 2 }),
		{ GL::PixelFormat::DepthComponent, GL::PixelType::Float });

	return Math::min<Float>(Containers::arrayCast<const Float>(data.data()));
}

Vector3 ZxcApplication::unproject(const Vector2i& windowPosition, Float depth) const {
	/* We have to take window size, not framebuffer size, since the position is
	   in window coordinates and the two can be different on HiDPI systems */
	const Vector2i viewSize = windowSize();
	const Vector2i viewPosition{ windowPosition.x(), viewSize.y() - windowPosition.y() - 1 };
	const Vector3 in{ 2 * Vector2{viewPosition} / Vector2{viewSize} -Vector2{1.0f}, depth * 2.0f - 1.0f };


	return (_cameraObject.absoluteTransformationMatrix() * _camera->projectionMatrix().inverted()).transformPoint(in);
	/*
  Use the following to get the camera-relative coordinates instead of global:
//    return _camera->projectionMatrix().inverted().transformPoint(in);
	   */
}

Vector3 ZxcApplication::positionOnSphere(const Vector2i& position) const {
	const Vector2 positionNormalized = Vector2{ position } / Vector2{ _camera->viewport() } -Vector2{ 0.5f };
	const Float length = positionNormalized.length();
	const Vector3 result(length > 1.0f ? Vector3(positionNormalized, 0.0f) : Vector3(positionNormalized, 1.0f - length));
	return (result * Vector3::yScale(-1.0f)).normalized();
}

void ZxcApplication::mouseMoveEvent(MouseMoveEvent& event) {
	if (!(event.buttons() & MouseMoveEvent::Button::Left)) return;

	const Vector3 currentPosition = positionOnSphere(event.position());
	const Vector3 axis = Math::cross(_previousPosition, currentPosition);

	if (_previousPosition.length() < 0.001f || axis.length() < 0.001f) return;

	_cameraObject.rotate(Math::angle(_previousPosition, currentPosition), axis.normalized());
	_previousPosition = currentPosition;

	redraw();
}

void ZxcApplication::keyPressEvent(Platform::Sdl2Application::KeyEvent &event) {
    if (event.key() == Magnum::Platform::Sdl2Application::KeyEvent::Key::Z) {
        Event curEvent(EventName::firstSkill, Player::First);

        events.push(curEvent);
        _gameState->applyEvent(curEvent);
		handleSkill(*_firstHero, SkillNum::first, _scene, _drawables, _timeline);
        // draw skill use
        redraw();
    }
    if (event.key() == Magnum::Platform::Sdl2Application::KeyEvent::Key::X) {
        Event curEvent(EventName::secondSkill, Player::First);

        events.push(curEvent);
        _gameState->applyEvent(curEvent);
		handleSkill(*_firstHero, SkillNum::second, _scene, _drawables, _timeline);
        // draw skill use
        redraw();
    }
    if (event.key() == Magnum::Platform::Sdl2Application::KeyEvent::Key::C) {
        Event curEvent(EventName::thirdSkill, Player::First);

        events.push(curEvent);
        _gameState->applyEvent(curEvent);
		handleSkill(*_firstHero, SkillNum::third, _scene, _drawables, _timeline);
        // draw skill use
        redraw();
    }
}

void ZxcApplication::exitEvent(ExitEvent& event) {
    static_cast<void>(event);
    exit_flag = true;
    std::cout << "Vi v adekvate?" << '\n';
    std::cout << "net" << std::endl;
    network_thread.join();
    exit(0);
}

MAGNUM_APPLICATION_MAIN(ZxcApplication)
