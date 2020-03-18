#include "Script.h"
#include "../Math/Vector.h"
#include <angelscript.h>
#include <scriptbuilder/scriptbuilder.h>
#include <scriptstdstring/scriptstdstring.h>

namespace glGame {

    class Test {
    public:
        void epic(int testVal, int t2) {
            std::cout << "Epic" << testVal << t2 << std::endl;
        }
    };

    void print(const std::string &in) {
        std::cout << in;
    }

    void println(const std::string &in) { std::cout << in << std::endl; }
    void println(const int &in) { std::cout << in << std::endl; }
    void println(const float &in) { std::cout << in << std::endl; }

    void Vector3Constructor(void* memory, float x, float y, float z) { new(memory) Vector3(x, y, z); }
    

    void MessageCallback(const asSMessageInfo *msg, void *param) {
        const char *type = "ERR ";
        if( msg->type == asMSGTYPE_WARNING ) 
            type = "WARN";
        else if( msg->type == asMSGTYPE_INFORMATION ) 
            type = "INFO";
        printf("%s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type, msg->message);
    }

    Script::Script() : filename("") {
        addPublicVariable(&filename, PublicVariableType::String, "scriptfile");
    }

    Script::~Script() {
        for(auto pVar : m_scriptPublicVars) {
            free(pVar);
        }
    }

    void Script::init() {
        asIScriptEngine *m_asScriptEngine = asCreateScriptEngine();
        int r = m_asScriptEngine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);

        RegisterStdString(m_asScriptEngine);

        r = m_asScriptEngine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(print), asCALL_CDECL);
        r = m_asScriptEngine->RegisterGlobalFunction("void println(const string &in)", asFUNCTIONPR(println, (const std::string&), void), asCALL_CDECL);
        r = m_asScriptEngine->RegisterGlobalFunction("void println(const int &in)", asFUNCTIONPR(println, (const int&), void), asCALL_CDECL);
        r = m_asScriptEngine->RegisterGlobalFunction("void println(const float &in)", asFUNCTIONPR(println, (const float&), void), asCALL_CDECL);
        r = m_asScriptEngine->RegisterObjectType("Test", sizeof(Test), asOBJ_VALUE | asOBJ_POD);
        r = m_asScriptEngine->RegisterObjectMethod("Test", "void epic(int a, int b)", asMETHOD(Test, Test::epic), asCALL_THISCALL);
        r = m_asScriptEngine->RegisterObjectType("Vector3", sizeof(Vector3), asOBJ_VALUE | asOBJ_POD);
        r = m_asScriptEngine->RegisterObjectBehaviour("Vector3", asBEHAVE_CONSTRUCT, "void Vector3(float x, float y, float z)", asFUNCTION(Vector3Constructor), asCALL_CDECL_OBJFIRST);
        r = m_asScriptEngine->RegisterObjectProperty("Vector3", "float x", asOFFSET(Vector3, Vector3::x));
        r = m_asScriptEngine->RegisterObjectProperty("Vector3", "float y", asOFFSET(Vector3, Vector3::y));
        r = m_asScriptEngine->RegisterObjectProperty("Vector3", "float z", asOFFSET(Vector3, Vector3::z));

        CScriptBuilder scriptbuilder;
        r = scriptbuilder.StartNewModule(m_asScriptEngine, "ScriptModule");
        if(r < 0) std::cout << "ERROR when starting new module" << std::endl;
        r = scriptbuilder.AddSectionFromFile("script.as");
        if(r < 0) std::cout << "ERROR when adding script from file" << std::endl;
        r = scriptbuilder.BuildModule();
        if(r < 0) std::cout << "ERROR when building module" << std::endl;

        asIScriptModule* asScriptModule = m_asScriptEngine->GetModule("ScriptModule");

        // Showing public variables from script
        unsigned int globalVariableCount = asScriptModule->GetGlobalVarCount();
        for(unsigned int i = 0; i < globalVariableCount; ++i) {
            const char* globalVarDecl = asScriptModule->GetGlobalVarDeclaration(i);
            void* globalVarAddr = asScriptModule->GetAddressOfGlobalVar(i);

            void* var = malloc(sizeof(int));
            *(int*)var = *((int*)globalVarAddr);
            m_scriptPublicVars.push_back(var);

            addPublicVariable(var, PublicVariableType::Int, std::string(globalVarDecl));
        }
        // --

        m_asScriptInitFunction = asScriptModule->GetFunctionByDecl("void init()");
        if(m_asScriptInitFunction == 0) std::cout << "ERROR no function void init()" << std::endl;

        asIScriptContext* ctx = m_asScriptEngine->CreateContext();
        ctx->Prepare(m_asScriptInitFunction);
        r = ctx->Execute();
        if(r != asEXECUTION_FINISHED) std::cout << "Could not execute" << std::endl;
        ctx->Release();
        m_asScriptEngine->ShutDownAndRelease();
    }

    void Script::update() {
        
    }

}