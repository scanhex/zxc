#include "ShaderLibrary.h"

#include <Magnum/Math/Color.h>

#include <cassert>

using namespace Magnum::Math::Literals;

ShaderLibrary *ShaderLibrary::instance_ = nullptr;

ShaderLibrary::ShaderLibrary() {
    assert(!instance_);
    instance_ = this;
    coloredShader_ = Magnum::Shaders::PhongGL{};
    texturedShader_ = Magnum::Shaders::PhongGL{
        Magnum::Shaders::PhongGL::Configuration{}.setFlags(Magnum::Shaders::PhongGL::Flag::DiffuseTexture)
    };
    flatShader_ = Magnum::Shaders::FlatGL3D{};
    coloredShader_.setAmbientColor(0x111111_rgbf).setSpecularColor(0xffffff_rgbf).setShininess(8.0f);
    texturedShader_.setAmbientColor(0x111111_rgbf).setSpecularColor(0x111111_rgbf).setShininess(8.0f);
}
