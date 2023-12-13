#pragma once

#include <Magnum/Shaders/Flat.h>
#include <Magnum/Shaders/Phong.h>

class ShaderLibrary {
private:
    static ShaderLibrary *instance_;
    Magnum::Shaders::PhongGL coloredShader_{Magnum::NoCreate}, texturedShader_{Magnum::NoCreate};
    Magnum::Shaders::FlatGL3D flatShader_{Magnum::NoCreate};

public:
    ShaderLibrary();

    static Magnum::Shaders::PhongGL &coloredShader() {
        return instance_->coloredShader_;
    }
    static Magnum::Shaders::PhongGL &texturedShader() {
        return instance_->texturedShader_;
    }
    static Magnum::Shaders::FlatGL3D &flatShader() {
        return instance_->flatShader_;
    }
};
