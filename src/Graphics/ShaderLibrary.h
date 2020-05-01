#pragma once
#include "Magnum/Shaders/Phong.h"
#include "Magnum/Shaders/Flat.h"

class ShaderLibrary {
private:
	static ShaderLibrary* _instance;
	Magnum::Shaders::Phong _coloredShader{ Magnum::NoCreate }, _texturedShader{ Magnum::NoCreate };
	Magnum::Shaders::Flat3D _flatShader{ Magnum::NoCreate };
public:
	ShaderLibrary();
	~ShaderLibrary();

	static Magnum::Shaders::Phong& coloredShader() {
		return _instance->_coloredShader;
	}
	static Magnum::Shaders::Phong& texturedShader() {
		return _instance->_texturedShader;
	}
	static Magnum::Shaders::Flat3D& flatShader() {
		return _instance->_flatShader;
	}
};
