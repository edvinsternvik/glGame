#pragma once
#include <vector>

namespace glGame {

    class UniformBuffer {
	public:
		UniformBuffer(unsigned int bytes);
        ~UniformBuffer();

		void bindBuffer();
		void unbindBuffer();
        void bindingPoint(const unsigned int& bindingPoint);
		void setData(unsigned int index, void* data);
		void addData(unsigned int bytes, void* data);

	private:
		unsigned int m_bufferID, m_dataSize, m_alignedOffset;

        struct UniformData {
            UniformData(const unsigned int& alignedOffset, const unsigned int& bytes) : alignedOffset(alignedOffset), bytes(bytes) {}
            unsigned int alignedOffset, bytes;
        };
        std::vector<UniformData> m_data;
	};

} 
