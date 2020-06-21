#pragma once

#include "Transform.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "LineRenderer.h"
#include "Script.h"
#include "RigidBody.h"

namespace glGame {
    namespace componentList {
        static const char* s_components[6] = { "Transform", "Camera", "MeshRenderer", "LineRenderer", "Script", "RigidBody" };
    }
}