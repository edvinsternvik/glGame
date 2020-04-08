#pragma once
#include <chrono>

namespace glGame {

    class Time {
    public:
        Time();

        float getDeltatime();

    private:
        std::chrono::high_resolution_clock::time_point previous;
    };

}