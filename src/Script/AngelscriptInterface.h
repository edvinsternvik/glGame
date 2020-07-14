#pragma once
#include <angelscript.h>
#include <scriptbuilder/scriptbuilder.h>
#include <scriptmath/scriptmath.h>
#include <scriptstdstring/scriptstdstring.h>

namespace glGame {

    class AngelscriptInterface {
    public:
        static void Register(asIScriptEngine* scriptEngine);

        static const char scriptObjectSectionCode[];
    private:
        static const int keycodes[121];
        static const int mousekeycodes[8];
        static const int cursortypes[3];
    };

}