#pragma once
#include <angelscript.h>
#include <scriptbuilder/scriptbuilder.h>
#include <scriptstdstring/scriptstdstring.h>

namespace glGame {

    class Script;

    class AngelscriptInterface {
    public:
        static void Register(Script* scriptComponent, asIScriptEngine* scriptEngine);

    private:
        static const int keycodes[121];
        static const int mousekeycodes[8];
    };

}