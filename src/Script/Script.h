#pragma once
#include <string>
#include <memory>

class asIScriptEngine;
class asIScriptModule;
class asITypeInfo;
class asIScriptFunction;

namespace glGame {
    
    class Script {
    public:
        Script(const char* filename);

    private:
        asIScriptModule* m_asScriptModule = nullptr;
        asITypeInfo* m_typeInfo = nullptr;
        asITypeInfo* m_baseTypeInfo = nullptr;
        asIScriptFunction* m_initMethod = nullptr;
        asIScriptFunction* m_updateMethod = nullptr;

        static asIScriptEngine* s_asScriptEngine;

        friend class ScriptComponent;
    };

}