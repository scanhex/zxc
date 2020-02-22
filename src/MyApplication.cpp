#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Math/Color.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/ImGuiIntegration/context.hpp>
#include <Magnum/Shaders/VertexColor.h>
#include <Magnum/Timeline.h>

using namespace Magnum;
using namespace Math::Literals;

class MyApplication: public Platform::Application {
    public:
        explicit MyApplication(const Arguments& arguments);

    private:
        void drawEvent() override;

    GL::Mesh _mesh;
    Shaders::VertexColor2D _shader;
    Timeline _timeline;
};

struct TriangleVertex {
    Vector2 position;
    Color3 color;
};



MyApplication::MyApplication(const Arguments& arguments): Platform::Application{arguments} {
    _timeline.start();
    setMinimalLoopPeriod(100);
}

float ticks=0;
const int C=100;

void MyApplication::drawEvent() {
    GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);
    if (ticks++==C)ticks=-C;

    GL::Buffer buffer;
    TriangleVertex data[]{
            {{-0.5f+ticks/C, -0.5f}, 0xff0000_rgbf},    /* Left vertex, red color */
            {{ 0.5f+ticks/C, -0.5f}, 0x00ff00_rgbf},    /* Right vertex, green color */
            {{ 0.0f+ticks/C,  0.5f}, 0x0000ff_rgbf}     /* Top vertex, blue color */
    };

    buffer.setData(data);
    _mesh.setCount(3).addVertexBuffer(std::move(buffer), 0, Shaders::VertexColor2D::Position{}, Shaders::VertexColor2D::Color3{});

    _mesh.draw(_shader);

    swapBuffers();
    _timeline.nextFrame();
    redraw();
}

MAGNUM_APPLICATION_MAIN(MyApplication)
