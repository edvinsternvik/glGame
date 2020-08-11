#pragma once

#include "Transform.h"
#include "CameraComponent.h"
#include "MeshRenderer.h"
#include "LineRenderer.h"
#include "ScriptComponent.h"
#include "RigidBody.h"
#include "BoxCollider.h"
#include "MaterialComponent.h"
#include "Skybox.h"
#include "LightComponent.h"

namespace glGame {
    namespace componentList {
        static const char* s_components[10] = { "Transform", "Camera", "Script", "MeshRenderer", "LineRenderer", "RigidBody", "BoxCollider", "Material", "Skybox", "Light" };
    }
}