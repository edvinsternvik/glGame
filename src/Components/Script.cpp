#include "Script.h"
#include "../Math/Vector.h"
#include "AngelscriptInterface.h"
#include "../Input.h"

namespace glGame {

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
        m_asScriptEngine = asCreateScriptEngine();

        AngelscriptInterface::Register(this, m_asScriptEngine);

        CScriptBuilder scriptbuilder;
        int r = scriptbuilder.StartNewModule(m_asScriptEngine, "ScriptModule");
        if(r < 0) std::cout << "ERROR when starting new module" << std::endl;
        r = scriptbuilder.AddSectionFromFile(filename.c_str());
        if(r < 0) {
            std::cout << "ERROR when adding script from file" << std::endl;
            cleanupScriptEngine();
            return;
        }
        r = scriptbuilder.BuildModule();
        if(r < 0) std::cout << "ERROR when building module" << std::endl;

        asIScriptModule* asScriptModule = m_asScriptEngine->GetModule("ScriptModule");

        // Showing public variables from script
        unsigned int globalVariableCount = asScriptModule->GetGlobalVarCount();
        for(unsigned int i = 0; i < globalVariableCount; ++i) {
            bool isPublic = false;

            std::vector<std::string> metadata = scriptbuilder.GetMetadataForVar(i);
            for(std::string& meta : metadata) {
                if(meta == "public") {
                    isPublic = true;
                    break;
                }
            }
            
            if(isPublic) {
                std::string globalVarDecl = asScriptModule->GetGlobalVarDeclaration(i);
                std::string globalVarTypeStr = globalVarDecl.substr(0, globalVarDecl.find(' '));
                PublicVariableType globalVarType = PublicVariable::getPublicVariableType(globalVarTypeStr);
                void* globalVarAddr = asScriptModule->GetAddressOfGlobalVar(i);

                PublicVariable publicVariable(getPublicVarVariant(globalVarAddr, globalVarType), globalVarDecl);
                auto search = m_registeredPublicVars.find(globalVarDecl);
                if(search != m_registeredPublicVars.end()) {
                    publicVariable.setData(search->second);
                }
                addPublicVariable(publicVariable);
            }
        }

        // --

        m_asScriptInitFunction = asScriptModule->GetFunctionByDecl("void init()");
        if(m_asScriptInitFunction == 0) std::cout << "ERROR no function void init()" << std::endl;
        m_asScriptUpdateFunction = asScriptModule->GetFunctionByDecl("void update(float)");
        if(m_asScriptUpdateFunction == 0) std::cout << "ERROR no function void update(float)" << std::endl;

        m_asScriptContext = m_asScriptEngine->CreateContext();
        if(m_asScriptInitFunction) {
            m_asScriptContext->Prepare(m_asScriptInitFunction);
            r = m_asScriptContext->Execute();
            if(r != asEXECUTION_FINISHED) std::cout << "Could not execute" << std::endl;
        }
    }

    void Script::update(float deltatime) {
        #ifndef GL_GAME_EDITOR
        if(m_asScriptUpdateFunction != nullptr) {
            m_asScriptContext->Prepare(m_asScriptUpdateFunction);
            m_asScriptContext->SetArgFloat(0, deltatime);
            int r = m_asScriptContext->Execute();
            if(r != asEXECUTION_FINISHED) std::cout << "Could not execute" << std::endl;
        }
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
        if(m_asScriptEngine != nullptr) m_asScriptEngine->ShutDownAndRelease();

        m_asScriptContext = nullptr;
        m_asScriptEngine = nullptr;
        m_asScriptInitFunction = nullptr;
        m_asScriptUpdateFunction = nullptr;
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