#include "Script.h"
#include "../../Math/Vector.h"
#include "AngelscriptInterface.h"
#include "../../Input.h"

#include <cstring>

namespace glGame {

    asIScriptEngine* Script::s_asScriptEngine = nullptr;

    Script::Script(const std::string& filename) : filename(filename) {
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
            return;
        }
        r = scriptbuilder.BuildModule();
        if(r < 0) std::cout << "ERROR when building module" << std::endl;

        m_asScriptModule = s_asScriptEngine->GetModule(filename.c_str());


        m_baseTypeInfo = m_asScriptModule->GetTypeInfoByName("ScriptObject");
        for(int i = 0; i < m_asScriptModule->GetObjectTypeCount(); ++i) {
            asITypeInfo* type = m_asScriptModule->GetObjectTypeByIndex(i);
            if(type->GetTypeId() != m_baseTypeInfo->GetTypeId() && type->GetBaseType() && type->GetBaseType()->GetTypeId() == m_baseTypeInfo->GetTypeId()) {
                m_typeInfo = type;
                break;
            }
        }

        if(m_typeInfo == nullptr) {
            return;
        }

        m_initMethod = m_typeInfo->GetMethodByDecl("void init()");
        m_updateMethod = m_typeInfo->GetMethodByDecl("void update(float deltaTime)");
    }
}