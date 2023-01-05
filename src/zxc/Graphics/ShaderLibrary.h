#pragma once

#include "Magnum/Shaders/Flat.h"
#include "Magnum/Shaders/Phong.h"

class ShaderLibrary {
private:
    static ShaderLibrary *instance_;
    Magnum::Shaders::Phong coloredShader_{Magnum::NoCreate}, texturedShader_{Magnum::NoCreate};
    Magnum::Shaders::Flat3D flatShader_{Magnum::NoCreate};

public:
    ShaderLibrary();

    static Magnum::Shaders::Phong &coloredShader() {
        return instance_->coloredShader_;
    }
    static Magnum::Shaders::Phong &texturedShader() {
        return instance_->texturedShader_;
    }
    static Magnum::Shaders::Flat3D &flatShader() {
        return instance_->flatShader_;
    }
};
