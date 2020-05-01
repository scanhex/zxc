#include "ShaderLibrary.h"
#include "Types.h"
#include <Magnum/Math/Color.h>
#include <cassert>

using namespace Magnum::Math::Literals;

ShaderLibrary* ShaderLibrary::_instance = nullptr;

ShaderLibrary::ShaderLibrary() {
	assert(!_instance);
	_instance = this;
	_coloredShader = Magnum::Shaders::Phong{};
	_texturedShader = Magnum::Shaders::Phong{ Magnum::Shaders::Phong::Flag::DiffuseTexture };
	_flatShader = Magnum::Shaders::Flat3D{};
	_coloredShader
		.setAmbientColor(0x111111_rgbf)
		.setSpecularColor(0xffffff_rgbf)
		.setShininess(8.0f);
	_texturedShader
		.setAmbientColor(0x111111_rgbf)
		.setSpecularColor(0x111111_rgbf)
		.setShininess(8.0f);
}

ShaderLibrary::~ShaderLibrary() {
	Debug{} << "~ShaderLibrary";
}
