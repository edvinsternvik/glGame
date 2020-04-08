#include "AngelscriptInterface.h"
#include <iostream>
#include "../Math/Vector.h"
#include "../Input.h"
#include "Script.h"
#include "Transform.h"
#include "../GameObject.h"

namespace glGame {

    void print(const std::string &in) { std::cout << in << std::flush; }
    void print(const int &in) { std::cout << in << std::flush; }
    void print(const float &in) { std::cout << in << std::flush; }
    void print(const double &in) { std::cout << in << std::flush; }
    void print(const Vector2 &in) { std::cout << in << std::flush; }
    void print(const Vector3 &in) { std::cout << in << std::flush; }
    void println(const std::string &in) { std::cout << in << std::endl; }
    void println(const int &in) { std::cout << in << std::endl; }
    void println(const float &in) { std::cout << in << std::endl; }
    void println(const Vector2 &in) { std::cout << in << std::endl; }
    void println(const Vector3 &in) { std::cout << in << std::endl; }

    void Vector3DefConstructor(void* memory) { new(memory) Vector3(0, 0, 0); }
    void Vector3Constructor(void* memory, float x, float y, float z) { new(memory) Vector3(x, y, z); }
    void Vector3Destructor(void* memory) { ((Vector3*)memory)->~Vector3(); }
    void Vector2DefConstructor(void* memory) { new(memory) Vector2(0, 0); }
    void Vector2Constructor(void* memory, float x, float y) { new(memory) Vector2(x, y); }
    void Vector2Destructor(void* memory) { ((Vector2*)memory)->~Vector2(); }

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

        r = scriptEngine->RegisterObjectType("Vector2", sizeof(Vector2), asOBJ_VALUE | asGetTypeTraits<Vector2>() | asOBJ_APP_CLASS_ALLFLOATS);
        r = scriptEngine->RegisterObjectBehaviour("Vector2", asBEHAVE_CONSTRUCT, "void Vector2(float x, float y)", asFUNCTION(Vector2Constructor), asCALL_CDECL_OBJFIRST);
        r = scriptEngine->RegisterObjectBehaviour("Vector2", asBEHAVE_CONSTRUCT, "void Vector2()", asFUNCTION(Vector2DefConstructor), asCALL_CDECL_OBJFIRST);
        r = scriptEngine->RegisterObjectBehaviour("Vector2", asBEHAVE_DESTRUCT, "void Vector2()", asFUNCTION(Vector2Destructor), asCALL_CDECL_OBJLAST);
        r = scriptEngine->RegisterObjectMethod("Vector2", "Vector2& opAssign(const Vector2& in)", asMETHODPR(Vector2, operator=, (const Vector2&), Vector2&), asCALL_THISCALL);
        r = scriptEngine->RegisterObjectMethod("Vector2", "Vector2 opAdd(const Vector2& in)", asMETHODPR(Vector2, operator+, (const Vector2&), Vector2), asCALL_THISCALL);
        r = scriptEngine->RegisterObjectMethod("Vector2", "Vector2 opSub(const Vector2& in)", asMETHODPR(Vector2, operator-, (const Vector2&), Vector2), asCALL_THISCALL);
        r = scriptEngine->RegisterObjectProperty("Vector2", "float x", asOFFSET(Vector2, Vector2::x));
        r = scriptEngine->RegisterObjectProperty("Vector2", "float y", asOFFSET(Vector2, Vector2::y));

        r = scriptEngine->RegisterObjectType("Vector3", sizeof(Vector3), asOBJ_VALUE | asGetTypeTraits<Vector3>() | asOBJ_APP_CLASS_ALLFLOATS);
        r = scriptEngine->RegisterObjectBehaviour("Vector3", asBEHAVE_CONSTRUCT, "void Vector3(float x, float y, float z)", asFUNCTION(Vector3Constructor), asCALL_CDECL_OBJFIRST);
        r = scriptEngine->RegisterObjectBehaviour("Vector3", asBEHAVE_CONSTRUCT, "void Vector3()", asFUNCTION(Vector3DefConstructor), asCALL_CDECL_OBJFIRST);
        r = scriptEngine->RegisterObjectBehaviour("Vector3", asBEHAVE_DESTRUCT, "void Vector3()", asFUNCTION(Vector3Destructor), asCALL_CDECL_OBJLAST);
        r = scriptEngine->RegisterObjectMethod("Vector3", "Vector3& opAssign(const Vector3& in)", asMETHODPR(Vector3, operator=, (const Vector3&), Vector3&), asCALL_THISCALL);
        r = scriptEngine->RegisterObjectMethod("Vector3", "Vector3 opAdd(const Vector3& in)", asMETHODPR(Vector3, operator+, (const Vector3&), Vector3), asCALL_THISCALL);
        r = scriptEngine->RegisterObjectMethod("Vector3", "Vector3 opSub(const Vector3& in)", asMETHODPR(Vector3, operator-, (const Vector3&), Vector3), asCALL_THISCALL);
        r = scriptEngine->RegisterObjectProperty("Vector3", "float x", asOFFSET(Vector3, Vector3::x));
        r = scriptEngine->RegisterObjectProperty("Vector3", "float y", asOFFSET(Vector3, Vector3::y));
        r = scriptEngine->RegisterObjectProperty("Vector3", "float z", asOFFSET(Vector3, Vector3::z));

        r = scriptEngine->RegisterObjectType("Transform", sizeof(Transform), asOBJ_REF | asOBJ_NOCOUNT);
        r = scriptEngine->RegisterObjectMethod("Transform", "void move(float x, float y, float z)", asMETHODPR(Transform, Transform::move, (float, float, float), void), asCALL_THISCALL);
        r = scriptEngine->RegisterObjectMethod("Transform", "void move(Vector3)", asMETHODPR(Transform, Transform::move, (Vector3), void), asCALL_THISCALL);
        r = scriptEngine->RegisterGlobalProperty("Transform transform", scriptComponent->getGameObject()->transform);

        
        r = scriptEngine->RegisterGlobalFunction("void print(const string &in)", asFUNCTIONPR(print, (const std::string&), void), asCALL_CDECL);
        r = scriptEngine->RegisterGlobalFunction("void print(const int &in)", asFUNCTIONPR(print, (const int&), void), asCALL_CDECL);
        r = scriptEngine->RegisterGlobalFunction("void print(const float &in)", asFUNCTIONPR(print, (const float&), void), asCALL_CDECL);
        r = scriptEngine->RegisterGlobalFunction("void print(const double &in)", asFUNCTIONPR(print, (const double&), void), asCALL_CDECL);
        r = scriptEngine->RegisterGlobalFunction("void print(const Vector2 &in)", asFUNCTIONPR(print, (const Vector2&), void), asCALL_CDECL);
        r = scriptEngine->RegisterGlobalFunction("void print(const Vector3 &in)", asFUNCTIONPR(print, (const Vector3&), void), asCALL_CDECL);
        r = scriptEngine->RegisterGlobalFunction("void println(const string &in)", asFUNCTIONPR(println, (const std::string&), void), asCALL_CDECL);
        r = scriptEngine->RegisterGlobalFunction("void println(const int &in)", asFUNCTIONPR(println, (const int&), void), asCALL_CDECL);
        r = scriptEngine->RegisterGlobalFunction("void println(const float &in)", asFUNCTIONPR(println, (const float&), void), asCALL_CDECL);
        r = scriptEngine->RegisterGlobalFunction("void println(const Vector2 &in)", asFUNCTIONPR(println, (const Vector2&), void), asCALL_CDECL);
        r = scriptEngine->RegisterGlobalFunction("void println(const Vector3 &in)", asFUNCTIONPR(println, (const Vector3&), void), asCALL_CDECL);


        r = scriptEngine->RegisterGlobalFunction("bool getKeyDown(int keycode)", asFUNCTION(Input::getKeyDown), asCALL_CDECL);
        r = scriptEngine->RegisterGlobalFunction("bool getKeyUp(int keycode)", asFUNCTION(Input::getKeyUp), asCALL_CDECL);
        r = scriptEngine->RegisterGlobalFunction("bool getKey(int keycode)", asFUNCTION(Input::getKey), asCALL_CDECL);
        r = scriptEngine->RegisterGlobalFunction("Vector2 getMouseDelta()", asFUNCTION(Input::getMouseDelta), asCALL_CDECL);
        r = scriptEngine->RegisterGlobalFunction("float getMouseDeltaX()", asFUNCTION(Input::getMouseDeltaX), asCALL_CDECL);
        r = scriptEngine->RegisterGlobalFunction("float getMouseDeltaY()", asFUNCTION(Input::getMouseDeltaY), asCALL_CDECL);
        r = scriptEngine->RegisterGlobalFunction("bool getMouseKey(int keycode)", asFUNCTION(Input::getMouseKey), asCALL_CDECL);
        r = scriptEngine->RegisterGlobalFunction("bool getMouseKeyDown(int keycode)", asFUNCTION(Input::getMouseKeyDown), asCALL_CDECL);
        r = scriptEngine->RegisterGlobalFunction("Vector2 getMousePosition()", asFUNCTION(Input::getMousePosition), asCALL_CDECL);
        r = scriptEngine->RegisterGlobalFunction("float getMouseX()", asFUNCTION(Input::getMouseX), asCALL_CDECL);
        r = scriptEngine->RegisterGlobalFunction("float getMouseY()", asFUNCTION(Input::getMouseY), asCALL_CDECL);
    }

}