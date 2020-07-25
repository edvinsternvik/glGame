#include "UniformBuffer.h"
#include <GL/glew.h>

#include <iostream>

namespace glGame {

    UniformBuffer::UniformBuffer(unsigned int bytes) : m_dataSize(bytes), m_alignedOffset(0) {
        glGenBuffers(1, &m_bufferID);
        bindBuffer();
        glBufferData(GL_UNIFORM_BUFFER, m_dataSize, NULL, GL_STATIC_DRAW);

        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    UniformBuffer::~UniformBuffer() {
        glDeleteBuffers(1, &m_bufferID);
    }

    void UniformBuffer::bindBuffer() {
        glBindBuffer(GL_UNIFORM_BUFFER, m_bufferID);
    }

    void UniformBuffer::unbindBuffer() {
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void UniformBuffer::bindingPoint(const unsigned int& bindingPoint) {
        glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_bufferID);
    }

    void UniformBuffer::setData(unsigned int index, void* data) {
        if(index < 0 || index > m_data.size()) {
            std::cout << "Could not set uniform data" << std::endl;
            return;
        }

        bindBuffer();
        glBufferSubData(GL_UNIFORM_BUFFER, m_data[index].alignedOffset, m_data[index].bytes, data);
        unbindBuffer();
    }

    //  IMPORTANT - "bytes" is not necessarily the size of the data, but rather the size of the data as it is stored by openGL using the std140 layout.
    void UniformBuffer::addData(unsigned int bytes, void* data) {
        m_alignedOffset += -m_alignedOffset % bytes;
        if(m_alignedOffset + bytes > m_dataSize) {
            std::cout << "Could not add data to uniform buffer" << std::endl;
            return;
        }

        m_data.push_back(UniformData(m_alignedOffset, bytes));
        setData(m_data.size() - 1, data);

        m_alignedOffset += bytes;
    }


}