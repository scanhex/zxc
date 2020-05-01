#include "ShaderLibrary.h"
#include <Magnum/Math/Color.h>

using namespace Magnum::Math::Literals;

Magnum::Shaders::Phong ShaderLibrary::_coloredShader = Magnum::Shaders::Phong{ Magnum::NoCreate };
Magnum::Shaders::Phong ShaderLibrary::_texturedShader = Magnum::Shaders::Phong{ Magnum::NoCreate };
Magnum::Shaders::Flat3D ShaderLibrary::_flatShader = Magnum::Shaders::Flat3D{ Magnum::NoCreate };

void ShaderLibrary::initShaders() {
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
