#pragma once
#include "../Asset.h"
#include <string>
#include <memory>

class asIScriptEngine;
class asIScriptModule;
class asITypeInfo;
class asIScriptFunction;

namespace glGame {
    
    class Script : public Asset {
    public:
        Script(const char* filename);

        virtual const char* getAssetTypeString() { return "Script"; }
        static AssetType getAssetType() { return AssetType::Script; }

    private:
        asIScriptModule* m_asScriptModule = nullptr;
        asITypeInfo* m_typeInfo = nullptr;
        asITypeInfo* m_baseTypeInfo = nullptr;
        asIScriptFunction* m_initMethod = nullptr;
        asIScriptFunction* m_updateMethod = nullptr;

        static asIScriptEngine* s_asScriptEngine;

        friend class ScriptComponent;
    };

    using ScriptAsset = std::shared_ptr<Script>;

}