#include "Vector.h"
#include <string>
#include <sstream>

namespace glGame {

    Vector2::Vector2(float x, float y) : x(x), y(y) {
    }

    Vector2::Vector2(std::string& str) {
        Vector2 vec2(0, 0);

        std::stringstream valueBuffer;
        valueBuffer << str.substr(1, str.size() - 2);
        std::string tmp;
        for(int i = 0; i < 2; ++i) {
            std::getline(valueBuffer, tmp, ',');
            
            ((float*)&vec2)[i] = std::stof(tmp);
        };

        x = vec2.x;
        y = vec2.y;
    }

    Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {
    }

    Vector3::Vector3(std::string& str) {
        Vector3 vec3(0, 0, 0);

        std::stringstream valueBuffer;
        valueBuffer << str.substr(1, str.size() - 2);
        std::string tmp;
        for(int i = 0; i < 3; ++i) {
            std::getline(valueBuffer, tmp, ',');
            
            ((float*)&vec3)[i] = std::stof(tmp);
        };

        x = vec3.x; y = vec3.y; z = vec3.z;
    }
}