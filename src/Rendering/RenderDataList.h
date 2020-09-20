#pragma once
#include "../Math/Vector.h"
#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <list>
#include <variant>

namespace glGame {

    class VertexArray;
    class Material;
    class Shader;

    using UniformDataVariant = std::variant<int, float, Vector3, glm::mat4>;

	struct UniformData {
		UniformData(const std::string& name, const UniformDataVariant& data);
		void setUniform(Shader* shader);
    private:
		std::string name;
		UniformDataVariant data;

	private:
		struct VisitUniformData {
			VisitUniformData(Shader* shader, const std::string& name);
			void operator()(int& data);
			void operator()(float& data);
			void operator()(Vector3& data);
			void operator()(glm::mat4& data);
		private:
            Shader* shader;
            const std::string& name;
		};
	};

	using UniformArray = std::vector<UniformData>;

    struct ObjectRenderData {
	public:
		ObjectRenderData();
		ObjectRenderData(VertexArray* vao, const unsigned int& verticies, Material* material, const glm::mat4& modelMatrix, const UniformArray& uniforms, const int& gameObjectId);
		ObjectRenderData(VertexArray* vao, const unsigned int& verticies, Shader* shader, const glm::mat4& modelMatrix, const UniformArray& uniforms, const int& gameObjectId);

		VertexArray* vao = nullptr;
		unsigned int verticies = 0;
		Material* material = nullptr;
		Shader* shader = nullptr;
		glm::mat4 modelMatrix;
        UniformArray uniformData;
		int gameObjectId = -1;
	};

    class ObjectRenderDataLayer {
    public:
        using Iterator = std::vector<ObjectRenderData>::iterator;

    public:
        ObjectRenderDataLayer(int layer) : layer(layer) {}
        int layer;
        std::vector<ObjectRenderData> data;

    public:
        Iterator begin();
        Iterator end();
    };

    class RenderDataList {
    private:
        using LayerMap = std::map<int, ObjectRenderDataLayer*>;
        using LayerList = std::list<ObjectRenderDataLayer>;
    
    public:
        using Iterator = LayerList::iterator;

    public:
        RenderDataList();

        void insert(int layer, const ObjectRenderData& objRenderData);
        void clear();
        int size() const;
        ObjectRenderData& front();
        ObjectRenderData& back();
        ObjectRenderData* last();

        Iterator begin();
        Iterator end();

    private:
        ObjectRenderDataLayer* getLayer(int layer);
        ObjectRenderDataLayer* createLayer(int layer);

    private:
        LayerList m_data;
        LayerMap m_layerMap;
        int m_size = 0;
        ObjectRenderData* m_last = nullptr;
    };

}