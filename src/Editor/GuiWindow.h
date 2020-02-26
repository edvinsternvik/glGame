#pragma once
#include <string>
#include <memory>

namespace glGame {

	class Scene;
	class PublicVariable;

	class GuiWindow {
	public:
		GuiWindow() = default;

		virtual std::string getWindowName() = 0;
		virtual void renderWindow() = 0;

	public:
		bool isOpen = true;
	};

	class ViewportWindow : public GuiWindow {
	public:
		ViewportWindow(unsigned int texture, float aspectRatio);

		inline virtual std::string getWindowName() { return "Viewport"; }
		virtual void renderWindow() override;

	private:
		unsigned int m_texture;
		float m_aspectRatio;
	};

	class SceneWindow : public GuiWindow {
	public:
		SceneWindow(std::shared_ptr<Scene>* scene);

		inline virtual std::string getWindowName() { return "Scene"; }
		virtual void renderWindow() override;

	private:
		std::shared_ptr<Scene>* m_scene;
	};

	class PropertiesWindow : public GuiWindow {
	public:
		PropertiesWindow(std::shared_ptr<Scene>* scene);

		inline virtual std::string getWindowName() { return "Properties"; }
		virtual void renderWindow() override;

	private:
		void drawComponentVariableGui(const PublicVariable* editorVariable);

	private:
		std::shared_ptr<Scene>* m_scene;
	};

}