#pragma once
#include "Magnum/Shaders/Phong.h"
#include "Types.h"
#include "Magnum/Shaders/Flat.h"

class ShaderLibrary {
private:
	static Magnum::Shaders::Phong _coloredShader, _texturedShader;
	static Magnum::Shaders::Flat3D _flatShader;
public:
	static void initShaders();
	static Magnum::Shaders::Phong& coloredShader() {
		return _coloredShader;
	}
	static Magnum::Shaders::Phong& texturedShader() {
		return _texturedShader;
	}
	static Magnum::Shaders::Flat3D& flatShader() {
		return _flatShader;
	}
};
