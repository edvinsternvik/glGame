#include "Time.h"

namespace glGame {

    Time::Time() : previous(std::chrono::high_resolution_clock::now()) {

    }

    float Time::getDeltatime() {
        std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
        std::chrono::duration d = now - previous;
        previous = now;

        return d.count() * 0.000000001;
    }

}