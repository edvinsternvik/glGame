#include "Script.h"
#include <angelscript.h>
#include <scriptbuilder/scriptbuilder.h>
#include <scriptstdstring/scriptstdstring.h>

namespace glGame {

    void print(const std::string &in) {
        std::cout << in;
    }

    void println(const std::string &in) {
        std::cout << in << std::endl;
    }
    

    void MessageCallback(const asSMessageInfo *msg, void *param) {
        const char *type = "ERR ";
        if( msg->type == asMSGTYPE_WARNING ) 
            type = "WARN";
        else if( msg->type == asMSGTYPE_INFORMATION ) 
            type = "INFO";
        printf("%s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type, msg->message);
    }

    Script::Script() : filename("test") {
        addPublicVariable((void*)filename, PublicVariableType::String, "scriptfile");

        asIScriptEngine *m_asScriptEngine = asCreateScriptEngine();
        int r = m_asScriptEngine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);

        RegisterStdString(m_asScriptEngine);

        r = m_asScriptEngine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(print), asCALL_CDECL);
        r = m_asScriptEngine->RegisterGlobalFunction("void println(const string &in)", asFUNCTION(println), asCALL_CDECL);

        CScriptBuilder scriptbuilder;
        r = scriptbuilder.StartNewModule(m_asScriptEngine, "ScriptModule");
        if(r < 0) std::cout << "ERROR when starting new module" << std::endl;
        r = scriptbuilder.AddSectionFromFile("script.as");
        if(r < 0) std::cout << "ERROR when adding script from file" << std::endl;
        r = scriptbuilder.BuildModule();
        if(r < 0) std::cout << "ERROR when building module" << std::endl;

        asIScriptModule* mod = m_asScriptEngine->GetModule("ScriptModule");
        m_asScriptUpdateFunction = mod->GetFunctionByDecl("void main()");
        if(m_asScriptUpdateFunction == 0) std::cout << "ERROR no function void main()" << std::endl;

        asIScriptContext* ctx = m_asScriptEngine->CreateContext();
        ctx->Prepare(m_asScriptUpdateFunction);
        r = ctx->Execute();
        if(r != asEXECUTION_FINISHED) std::cout << "Could not execute" << std::endl;
        ctx->Release();
        m_asScriptEngine->ShutDownAndRelease();
    }

    Script::~Script() {
    }

    void Script::update() {
        
    }

}