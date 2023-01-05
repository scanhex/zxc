#pragma once

#include "Corrade/Containers/Optional.h"
#include "Corrade/Containers/Pointer.h"
#include "Corrade/Utility/Debug.h"
#include "Corrade/Utility/DebugStl.h"
#include "Magnum/SceneGraph/MatrixTransformation3D.h"
#include "Magnum/SceneGraph/Object.h"
#include "Magnum/SceneGraph/Scene.h"

typedef Magnum::SceneGraph::Object<Magnum::SceneGraph::MatrixTransformation3D> Object3D;
typedef Magnum::SceneGraph::Scene<Magnum::SceneGraph::MatrixTransformation3D> Scene3D;
using Corrade::Containers::Optional;
using Corrade::Containers::Pointer;
using Magnum::Debug;
