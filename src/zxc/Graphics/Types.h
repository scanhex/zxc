#pragma once

#include "Magnum/SceneGraph/Object.h"
#include "Magnum/SceneGraph/Scene.h"
#include "Magnum/SceneGraph/MatrixTransformation3D.h"
#include "Corrade/Containers/Pointer.h"
#include "Corrade/Containers/Optional.h"
#include "Corrade/Utility/Debug.h"
#include "Corrade/Utility/DebugStl.h"

typedef Magnum::SceneGraph::Object<Magnum::SceneGraph::MatrixTransformation3D> Object3D;
typedef Magnum::SceneGraph::Scene<Magnum::SceneGraph::MatrixTransformation3D> Scene3D;
using Corrade::Containers::Pointer;
using Corrade::Containers::Optional;
using Magnum::Debug;
