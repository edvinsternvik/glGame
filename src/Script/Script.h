#pragma once
#include <string>

class asIScriptEngine;
class asIScriptModule;
class asITypeInfo;
class asIScriptFunction;

namespace glGame {
    
    class Script {
    public:
        Script(const std::string& filename);

    private:
        std::string filename;
        asIScriptModule* m_asScriptModule = nullptr;
        asITypeInfo* m_typeInfo = nullptr;
        asITypeInfo* m_baseTypeInfo = nullptr;
        asIScriptFunction* m_initMethod = nullptr;
        asIScriptFunction* m_updateMethod = nullptr;

        static asIScriptEngine* s_asScriptEngine;

        friend class ScriptComponent;
    };

}