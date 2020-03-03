#include "Script.h"

namespace glGame {

    Script::Script() : filename("test") {
        addPublicVariable((void*)filename, PublicVariableType::String, "scriptfile");
    }

    Script::~Script() {
    }

    void Script::update() {
        
    }

}