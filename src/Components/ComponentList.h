#pragma once

#include "Transform.h"
#include "CameraComponent.h"
#include "MeshRenderer.h"
#include "LineRenderer.h"
#include "ScriptComponent.h"
#include "RigidBody.h"
#include "StaticBody.h"
#include "BoxCollider.h"
#include "Skybox.h"
#include "LightComponent.h"

namespace glGame {
    namespace componentList {
        static const char* s_components[10] = { "Transform", "Camera", "Script", "MeshRenderer", "LineRenderer", "RigidBody", "StaticBody", "BoxCollider", "Skybox", "Light" };
    }
}