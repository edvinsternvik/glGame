#include "ScriptComponent.h"
#include "Transform.h"
#include "../GameObject.h"

#include <angelscript.h>

namespace glGame {

    ScriptComponent::ScriptComponent() {
        addPublicVariable(&script, "Script");
    }

    ScriptComponent::~ScriptComponent() {
        cleanupScript();
    }

    void ScriptComponent::init() {
        cleanupScript();

        if(script.get() && script->m_typeInfo) {
            m_scriptObj = reinterpret_cast<asIScriptObject*>(Script::s_asScriptEngine->CreateScriptObject(script->m_typeInfo));
            Transform** scriptTransform = (Transform**)m_scriptObj->GetAddressOfProperty(0);
            *scriptTransform = getGameObject()->transform;


            // Show public variables from script
            unsigned int classVariableCount = script->m_typeInfo->GetPropertyCount();
            for(unsigned int i = script->m_baseTypeInfo->GetPropertyCount(); i < classVariableCount; ++i) {
                bool isPrivate = false;
                script->m_typeInfo->GetProperty(i, nullptr, nullptr, &isPrivate);
                
                if(!isPrivate) {
                    std::string publicVarDecl = script->m_typeInfo->GetPropertyDeclaration(i);
                    std::string publicVarTypeStr = publicVarDecl.substr(0, publicVarDecl.find(' '));
                    PublicVariableType publicVarType = PublicVariable::getPublicVariableType(publicVarTypeStr);
                    void* publicVarAddr = m_scriptObj->GetAddressOfProperty(i);

                    PublicVariable publicVariable(getPublicVarVariant(publicVarAddr, publicVarType), publicVarDecl);
                    auto search = m_registeredPublicVars.find(publicVarDecl);
                    if(search != m_registeredPublicVars.end()) {
                        publicVariable.setData(search->second);
                    }
                    addPublicVariable(publicVariable);
                }
            }

            if(script->m_initMethod) {
                asIScriptContext* context = script->s_asScriptEngine->RequestContext();
                context->Prepare(script->m_initMethod);
                context->SetObject(m_scriptObj);
                context->Execute();
                context->Release();
            }
        }
    }

    void ScriptComponent::update(float deltatime) {
        #ifndef GL_GAME_EDITOR
        if(script.get() && m_scriptObj && script->m_updateMethod) {
            asIScriptContext* context = script->s_asScriptEngine->RequestContext();
            context->Prepare(script->m_updateMethod);
            context->SetObject(m_scriptObj);
            context->SetArgFloat(0, deltatime);
            context->Execute();
            context->Release();
        }
        #endif
    }

    void ScriptComponent::registerPublicScriptVariable(std::vector<std::string>& strings) {
        if(strings.size() < 3) return;

        std::string pVarName = strings[1];
        for(int i = 2; i < strings.size() - 1; ++i) pVarName += " " + strings[i];
        std::string pVarValueStr = strings[strings.size() - 1];

        m_registeredPublicVars.insert(std::pair<std::string, std::string>(pVarName, pVarValueStr));
    }

    PublicVarVariant ScriptComponent::getPublicVarVariant(void* data, const PublicVariableType& type) {
        switch(type) {
		case PublicVariableType::Int: return PublicVarVariant((int*)data);
		case PublicVariableType::Float: return PublicVarVariant((float*)data);
		case PublicVariableType::Double: return PublicVarVariant((double*)data);
		case PublicVariableType::Char: return PublicVarVariant((char*)data);
		case PublicVariableType::String: return PublicVarVariant((std::string*)data);
		case PublicVariableType::Vec2: return PublicVarVariant((Vector2*)data);
		case PublicVariableType::Vec3: return PublicVarVariant((Vector3*)data);
		// case PublicVariableType::GameObject:
		// case PublicVariableType::Component:
		// case PublicVariableType::Color:
        // case PublicVariableType::Model: return addPublicVariable(&std::get<unsigned int>(scriptVariable.var), scriptVariable.declaration);
		}
        return PublicVarVariant();
    }

    void ScriptComponent::cleanupScript() {
        if(script.get() && script->s_asScriptEngine) {
            if(m_scriptObj) m_scriptObj->Release();
            m_scriptObj = nullptr;
            m_scriptPublicVars.clear();
            for(int i = getPublicVariableCount() - 1; i > 0 ; --i) {
                removePublicVariable(i);
            }
        }
    }


}