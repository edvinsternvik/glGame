#pragma once
#include "memory"

namespace glGame {

	class Texture;

	class FrameBuffer {
	public:
		FrameBuffer(const unsigned int& width, const unsigned int& height);
		FrameBuffer(std::shared_ptr<Texture> texture);
		~FrameBuffer();

		void bind();
		void unbind();

		inline std::shared_ptr<Texture> getTexture() { return m_texture; }

	private:
		unsigned int genTexture(const unsigned int& width, const unsigned int& height);
		unsigned int genRenderBuffer(const unsigned int& width, const unsigned int& height);

	private:
		unsigned int m_frameBuffer;
		std::shared_ptr<Texture> m_texture;
		unsigned int m_renderBuffer = -1;

	};

}