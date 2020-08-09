#pragma once
#include "memory"

namespace glGame {

	class Texture;
	class TextureArray;

	class FrameBuffer {
	public:
		FrameBuffer(const unsigned int& width, const unsigned int& height);
		FrameBuffer(std::shared_ptr<Texture> texture);
		FrameBuffer(std::shared_ptr<TextureArray> textureArray, unsigned int textureNumber);
		~FrameBuffer();

		void bind();
		void unbind();

		std::shared_ptr<Texture> getTexture() { return m_texture; }
		std::shared_ptr<TextureArray> getTextureArray() { return m_textureArray; }

	private:
		unsigned int genTexture(const unsigned int& width, const unsigned int& height);
		unsigned int genRenderBuffer(const unsigned int& width, const unsigned int& height);

	private:
		unsigned int m_frameBuffer;
		std::shared_ptr<Texture> m_texture;
		std::shared_ptr<TextureArray> m_textureArray;
		unsigned int m_renderBuffer = -1;

	};

}