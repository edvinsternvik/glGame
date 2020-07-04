#include "Script.h"
#include "../Math/Vector.h"
#include "AngelscriptInterface.h"
#include "../Input.h"

#include <cstring>

namespace glGame {

    asIScriptEngine* Script::s_asScriptEngine = nullptr;

    Script::Script() : filename("") {
        addPublicVariable(&filename, "scriptfile");
    }

    Script::~Script() {
        cleanupScriptEngine();
    }

    void Script::init() {
        cleanupScriptEngine();

        if(filename == "") {
            return;
        }
        if(s_asScriptEngine == nullptr) {
            s_asScriptEngine = asCreateScriptEngine();
            AngelscriptInterface::Register(s_asScriptEngine);
        }

        CScriptBuilder scriptbuilder;
        int r = scriptbuilder.StartNewModule(s_asScriptEngine, filename.c_str());
        if(r < 0) std::cout << "ERROR when starting new module" << std::endl;
        r = scriptbuilder.AddSectionFromMemory("ScriptObjectSection", AngelscriptInterface::scriptObjectSectionCode, std::strlen(AngelscriptInterface::scriptObjectSectionCode));
        r = scriptbuilder.AddSectionFromFile(filename.c_str());
        if(r < 0) {
            std::cout << "ERROR when adding script from file" << std::endl;
            cleanupScriptEngine();
            return;
        }
        r = scriptbuilder.BuildModule();
        if(r < 0) std::cout << "ERROR when building module" << std::endl;

        asIScriptModule* asScriptModule = s_asScriptEngine->GetModule(filename.c_str());


        asITypeInfo* classType;
        asITypeInfo* base = asScriptModule->GetTypeInfoByName("ScriptObject");
        for(int i = 0; i < asScriptModule->GetObjectTypeCount(); ++i) {
            asITypeInfo* type = asScriptModule->GetObjectTypeByIndex(i);
            if(type->GetTypeId() != base->GetTypeId() && type->GetBaseType() && type->GetBaseType()->GetTypeId() == base->GetTypeId()) {
                classType = type;
                break;
            }
        }

        m_scriptObj = reinterpret_cast<asIScriptObject*>(s_asScriptEngine->CreateScriptObject(classType));


        // Showing public variables from script
        unsigned int classVariableCount = m_scriptObj->GetPropertyCount();
        for(unsigned int i = base->GetPropertyCount(); i < classVariableCount; ++i) {
            bool isPrivate = false;
            classType->GetProperty(i, nullptr, nullptr, &isPrivate);
            
            if(!isPrivate) {
                std::string publicVarDecl = classType->GetPropertyDeclaration(i);
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

        // --
    }

    void Script::update(float deltatime) {
        #ifndef GL_GAME_EDITOR
        #endif
    }

    void Script::registerPublicScriptVariable(std::vector<std::string>& strings) {
        if(strings.size() < 3) return;

        std::string pVarName = strings[1];
        for(int i = 2; i < strings.size() - 1; ++i) pVarName += " " + strings[i];
        std::string pVarValueStr = strings[strings.size() - 1];

        m_registeredPublicVars.insert(std::pair<std::string, std::string>(pVarName, pVarValueStr));
    }

    void Script::cleanupScriptEngine() {
        if(m_asScriptContext != nullptr) m_asScriptContext->Release();

        m_asScriptContext = nullptr;
        if(m_scriptObj) m_scriptObj->Release();
        m_scriptObj = nullptr;
        m_scriptPublicVars.clear();
        for(int i = getPublicVariableCount() - 1; i > 0 ; --i) {
            removePublicVariable(i);
        }
    }

    PublicVarVariant Script::getPublicVarVariant(void* data, const PublicVariableType& type) {
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

}