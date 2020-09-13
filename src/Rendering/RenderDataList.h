#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <list>

namespace glGame {

    class VertexArray;
    class Material;
    class Shader;

    struct ObjectRenderData {
	public:
		ObjectRenderData() {}
		ObjectRenderData(VertexArray* vao, const unsigned int& verticies, Material* material, const glm::mat4& modelMatrix)
			: vao(vao), verticies(verticies), material(material), modelMatrix(modelMatrix) {}
		ObjectRenderData(VertexArray* vao, const unsigned int& verticies, Shader* shader, const glm::mat4& modelMatrix)
			: vao(vao), verticies(verticies), shader(shader), modelMatrix(modelMatrix) {}

		VertexArray* vao = nullptr;
		unsigned int verticies = 0;
		Material* material = nullptr;
		Shader* shader = nullptr;
		glm::mat4 modelMatrix;
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