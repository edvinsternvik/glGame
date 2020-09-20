#include "RenderDataList.h"
#include "Shader.h"

namespace glGame {

    UniformData::UniformData(const std::string& name, const UniformDataVariant& data) 
		: name(name), data(data) {
	}

	void UniformData::UniformData::setUniform(Shader* shader) {
		std::visit(VisitUniformData(shader, name), data);
	}

    UniformData::VisitUniformData::VisitUniformData(Shader* shader, const std::string& name) 
        : shader(shader), name(name) {
    }

	void UniformData::VisitUniformData::operator()(int& data) {
		shader->setUniform1i(name.c_str(), data);
	}

	void UniformData::VisitUniformData::operator()(float& data) {

	}

	void UniformData::VisitUniformData::operator()(Vector3& data) {
        shader->setUniform3f(name.c_str(), data.x, data.y, data.z);
	}

	void UniformData::VisitUniformData::operator()(glm::mat4& data) {
		shader->setUniformMat4(name.c_str(), data);
	}

    ObjectRenderData::ObjectRenderData() {}
    ObjectRenderData::ObjectRenderData(VertexArray* vao, const unsigned int& verticies, Material* material, const glm::mat4& modelMatrix, const UniformArray& uniforms)
        : vao(vao), verticies(verticies), material(material), modelMatrix(modelMatrix), uniformData(uniforms) {
    }

    ObjectRenderData::ObjectRenderData(VertexArray* vao, const unsigned int& verticies, Shader* shader, const glm::mat4& modelMatrix, const UniformArray& uniforms)
        : vao(vao), verticies(verticies), shader(shader), modelMatrix(modelMatrix), uniformData(uniforms) {
    }

    RenderDataListIterator::RenderDataListIterator(ObjectRenderDataList::iterator objIterator, ObjectRenderLayerList::iterator layerIterator, ObjectRenderLayerList::iterator layerEnd)
        : m_objIterator(objIterator), m_layerIterator(layerIterator), m_layerEnd(layerEnd) {

    }

    RenderDataListIterator& RenderDataListIterator::operator++() {
        m_objIterator++;
        ObjectRenderDataList::iterator e = m_layerIterator->data.end();

        if(m_objIterator == e) {
            m_layerIterator++;
            if(m_layerIterator != m_layerEnd) {
                m_objIterator = m_layerIterator->data.begin();
            }
        }

        return *this;
    }

    ObjectRenderData* RenderDataListIterator::operator->() {
        return &(*m_objIterator);
    }

    ObjectRenderData& RenderDataListIterator::operator*() {
        return *m_objIterator;
    }

    bool RenderDataListIterator::operator==(const RenderDataListIterator& other) const {
        return m_objIterator == other.m_objIterator;
    }

    bool RenderDataListIterator::operator!=(const RenderDataListIterator& other) const {
        return m_objIterator != other.m_objIterator;
    }


    RenderDataList::RenderDataList() {
        createLayer(0);
    }

    void RenderDataList::insert(int layer, const ObjectRenderData& objRenderData) {
        auto& layerData = getLayer(layer)->data;
        layerData.push_back(objRenderData);
        ++m_size;
        m_last = &layerData.back();
    }

    void RenderDataList::clear() {
        m_data.clear();
        m_layerMap.clear();
        m_size = 0;
        m_last = nullptr;
    }

    int RenderDataList::size() const {
        return m_size;
    }

    ObjectRenderData& RenderDataList::front() {
        return m_data.front().data.front();
    }

    ObjectRenderData& RenderDataList::back() {
        return m_data.back().data.back();
    }

    ObjectRenderData* RenderDataList::last() {
        return m_last;
    }
    
    RenderDataList::Iterator RenderDataList::begin() {
        return RenderDataList::Iterator(m_data.begin()->data.begin(), m_data.begin(), m_data.end());
    }

    RenderDataList::Iterator RenderDataList::end() {
        return RenderDataList::Iterator(m_data.rbegin()->data.end(), std::prev(m_data.end(), 1), m_data.end());
    }

    ObjectRenderDataLayer* RenderDataList::getLayer(int layer) {
        ObjectRenderDataLayer* objRenderDataLayer = nullptr;
        auto search = m_layerMap.find(layer);
        if(search == m_layerMap.end()) {
            objRenderDataLayer = createLayer(layer);
        }
        else {
            objRenderDataLayer = &(*search->second);
        }
        
        return objRenderDataLayer;
    }

    ObjectRenderDataLayer* RenderDataList::createLayer(int layer) {
        auto search = m_data.begin();
        while(search != m_data.end() && search->layer < layer) {
            search++;
        }

        auto inserted = m_data.insert(search, ObjectRenderDataLayer(layer));
        m_layerMap.insert(std::pair<int, ObjectRenderDataLayer*>(layer, &(*inserted)));
        return &(*inserted);
    }


}