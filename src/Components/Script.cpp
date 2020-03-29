#include "Script.h"
#include "../Math/Vector.h"
#include "AngelscriptInterface.h"
#include "../Input.h"

namespace glGame {

    Script::Script() : filename("") {
        addPublicVariable(&filename, PublicVariableType::String, "scriptfile");
    }

    Script::~Script() {
        if(m_asScriptContext != nullptr) m_asScriptContext->Release();
        if(m_asScriptEngine != nullptr) m_asScriptEngine->ShutDownAndRelease();
    }

    void Script::init() {
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
            m_asScriptEngine->ShutDownAndRelease();
            m_asScriptEngine = nullptr;
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

                auto search = m_registeredPublicVars.find(globalVarDecl);
                if(search != m_registeredPublicVars.end()) {
                    PublicVariable temp(globalVarAddr, globalVarType, "t");
                    temp.setData(search->second);
                }

                m_scriptPublicVars.push_back(PublicScriptVariable(globalVarAddr, globalVarType, globalVarDecl));
            }
        }
        m_registeredPublicVars.clear();

        for(auto& pVar : m_scriptPublicVars) {
            addPublicVariable(&pVar.var, pVar.type, pVar.declaration);
        }

        // --

        m_asScriptInitFunction = asScriptModule->GetFunctionByDecl("void init()");
        if(m_asScriptInitFunction == 0) std::cout << "ERROR no function void init()" << std::endl;
        m_asScriptUpdateFunction = asScriptModule->GetFunctionByDecl("void update()");
        if(m_asScriptUpdateFunction == 0) std::cout << "ERROR no function void update()" << std::endl;

        m_asScriptContext = m_asScriptEngine->CreateContext();
        m_asScriptContext->Prepare(m_asScriptInitFunction);
        r = m_asScriptContext->Execute();
        if(r != asEXECUTION_FINISHED) std::cout << "Could not execute" << std::endl;
    }

    void Script::update() {
        m_asScriptContext->Prepare(m_asScriptUpdateFunction);
        int r = m_asScriptContext->Execute();
        if(r != asEXECUTION_FINISHED) std::cout << "Could not execute" << std::endl;
    }

    void Script::registerPublicScriptVariable(std::vector<std::string>& strings) {
        if(strings.size() < 3) return;

        std::string pVarName = strings[1];
        for(int i = 2; i < strings.size() - 1; ++i) pVarName += " " + strings[i];
        std::string pVarValueStr = strings[strings.size() - 1];

        m_registeredPublicVars.insert(std::pair<std::string, std::string>(pVarName, pVarValueStr));
    }

}