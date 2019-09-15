#pragma once
#include <string>

namespace glGame {

	class GuiWindow {
	public:
		GuiWindow() = default;

		virtual std::string getWindowName() = 0;
		virtual void renderWindow() = 0;
	};

	class ViewportWindow : public GuiWindow {
	public:
		ViewportWindow(unsigned int texture, float aspectRatio);

		inline virtual std::string getWindowName() { return "ViewportWindow"; }
		virtual void renderWindow() override;

	private:
		unsigned int m_texture;
		float m_aspectRatio;
	};

}