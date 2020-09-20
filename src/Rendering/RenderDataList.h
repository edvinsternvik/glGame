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
		ObjectRenderData(VertexArray* vao, const unsigned int& verticies, Material* material, const glm::mat4& modelMatrix, const UniformArray& uniforms);
		ObjectRenderData(VertexArray* vao, const unsigned int& verticies, Shader* shader, const glm::mat4& modelMatrix, const UniformArray& uniforms);

		VertexArray* vao = nullptr;
		unsigned int verticies = 0;
		Material* material = nullptr;
		Shader* shader = nullptr;
		glm::mat4 modelMatrix;
        UniformArray uniformData;
		int gameObjectId = -1;
	};

    struct ObjectRenderDataLayer {
        ObjectRenderDataLayer(int layer) : layer(layer) {}
        int layer;
        std::vector<ObjectRenderData> data;
    };

    class RenderDataListIterator {
    public:
        using ObjectRenderDataList = std::vector<ObjectRenderData>;
        using ObjectRenderLayerList = std::list<ObjectRenderDataLayer>;
        using LayerMap = std::map<int, ObjectRenderDataLayer*>;
        
    public:
        RenderDataListIterator(ObjectRenderDataList::iterator objIterator, ObjectRenderLayerList::iterator layerIterator, ObjectRenderLayerList::iterator layerEnd);

        RenderDataListIterator& operator++();
        ObjectRenderData* operator->();
        ObjectRenderData& operator*();
        bool operator==(const RenderDataListIterator& other) const;
        bool operator!=(const RenderDataListIterator& other) const;

    private:
        ObjectRenderDataList::iterator m_objIterator;
        ObjectRenderLayerList::iterator m_layerIterator, m_layerEnd;
    };

    class RenderDataList {
    public:
        using Iterator = RenderDataListIterator;

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
        using LayerMap = std::map<int, ObjectRenderDataLayer*>;

    private:
        ObjectRenderDataLayer* getLayer(int layer);
        ObjectRenderDataLayer* createLayer(int layer);

    private:
        std::list<ObjectRenderDataLayer> m_data;
        LayerMap m_layerMap;
        int m_size = 0;
        ObjectRenderData* m_last = nullptr;
    };

}