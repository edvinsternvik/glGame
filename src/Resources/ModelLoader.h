#pragma once
#include <vector>

namespace glGame {

	struct ModelVec3 {
		float x, y, z;

		ModelVec3() {}
		ModelVec3(float x, float y, float z) : x(x), y(y), z(z) {}
	};

	struct ModelVec2 {
		float x, y;

		ModelVec2() {}
		ModelVec2(float x, float y) : x(x), y(y) {}
	};

	struct ModelData {
		ModelVec3 vertex;
		ModelVec2 texture;
		ModelVec3 normal;

		ModelData(ModelVec3 vertex, ModelVec2 texture, ModelVec3 normal) : vertex(vertex), texture(texture), normal(normal) {}
	};

	class ModelLoader {
	public:
		static void loadModel(const char* filePath, std::vector<ModelData>* data, std::vector<unsigned int>* indicies);

	private:
		static void loadObjModel(const char* filePath, std::vector<ModelData>* data, std::vector<unsigned int>* indicies);
	};

}