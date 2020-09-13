#include "RenderDataList.h"

namespace glGame {

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