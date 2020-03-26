#pragma once
#include <angelscript.h>
#include <scriptbuilder/scriptbuilder.h>
#include <scriptstdstring/scriptstdstring.h>

namespace glGame {

    class AngelscriptInterface {
    public:
        static void Register(asIScriptEngine* scriptEngine);
    };

}