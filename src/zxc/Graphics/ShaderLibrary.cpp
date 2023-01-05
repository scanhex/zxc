#include <cassert>

#include "ShaderLibrary.h"
#include "Magnum/Math/Color.h"

using namespace Magnum::Math::Literals;

ShaderLibrary *ShaderLibrary::instance_ = nullptr;

ShaderLibrary::ShaderLibrary() {
    assert(!instance_);
    instance_ = this;
    coloredShader_ = Magnum::Shaders::Phong{};
    texturedShader_ = Magnum::Shaders::Phong{Magnum::Shaders::Phong::Flag::DiffuseTexture};
    flatShader_ = Magnum::Shaders::Flat3D{};
    coloredShader_
            .setAmbientColor(0x111111_rgbf)
            .setSpecularColor(0xffffff_rgbf)
            .setShininess(8.0f);
    texturedShader_
            .setAmbientColor(0x111111_rgbf)
            .setSpecularColor(0x111111_rgbf)
            .setShininess(8.0f);
}
