#include "AngelscriptInterface.h"
#include <iostream>
#include "../Math/Vector.h"
#include "../Input.h"
#include "Script.h"
#include "Transform.h"
#include "../GameObject.h"

namespace glGame {

    void print(const std::string &in) { std::cout << in; }
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

    void AngelscriptInterface::Register(Script* scriptComponent, asIScriptEngine* scriptEngine) {
        int r = scriptEngine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);

        RegisterStdString(scriptEngine);

        r = scriptEngine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(print), asCALL_CDECL);
        r = scriptEngine->RegisterGlobalFunction("void println(const string &in)", asFUNCTIONPR(println, (const std::string&), void), asCALL_CDECL);
        r = scriptEngine->RegisterGlobalFunction("void println(const int &in)", asFUNCTIONPR(println, (const int&), void), asCALL_CDECL);
        r = scriptEngine->RegisterGlobalFunction("void println(const float &in)", asFUNCTIONPR(println, (const float&), void), asCALL_CDECL);

        r = scriptEngine->RegisterObjectType("Vector3", sizeof(Vector3), asOBJ_VALUE | asOBJ_POD);
        r = scriptEngine->RegisterObjectBehaviour("Vector3", asBEHAVE_CONSTRUCT, "void Vector3(float x, float y, float z)", asFUNCTION(Vector3Constructor), asCALL_CDECL_OBJFIRST);
        r = scriptEngine->RegisterObjectProperty("Vector3", "float x", asOFFSET(Vector3, Vector3::x));
        r = scriptEngine->RegisterObjectProperty("Vector3", "float y", asOFFSET(Vector3, Vector3::y));
        r = scriptEngine->RegisterObjectProperty("Vector3", "float z", asOFFSET(Vector3, Vector3::z));

        r = scriptEngine->RegisterObjectType("Transform", sizeof(Transform), asOBJ_REF | asOBJ_NOCOUNT);
        r = scriptEngine->RegisterObjectMethod("Transform", "void move(float x, float y, float z)", asMETHOD(Transform, Transform::move), asCALL_THISCALL);
        r = scriptEngine->RegisterGlobalProperty("Transform transform", scriptComponent->getGameObject()->transform);

        r = scriptEngine->RegisterGlobalFunction("bool getKeyDown(int keycode)", asFUNCTION(Input::getKey), asCALL_CDECL);
    }

}