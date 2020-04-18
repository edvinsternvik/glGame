#pragma
#include <memory>
#include <vector>
#include <functional>
#include "../Events/EditorEvent.h"

namespace glGame {

	class Scene;
	class PublicVariable;

	class GuiWindow {
	public:
		GuiWindow() = default;

		virtual const char* getWindowName() = 0;
		virtual void renderWindow() = 0;

		inline void setEventFunction(std::function<void(Event&)>& eventFunction) { m_eventFunction = &eventFunction; }

	public:
		bool isOpen = true;
		std::function<void(Event&)>* m_eventFunction = nullptr;
	};

	class ViewportWindow : public GuiWindow {
	public:
		ViewportWindow(unsigned int texture, float aspectRatio);

		inline virtual const char* getWindowName() { return "Viewport"; }
		virtual void renderWindow() override;

	private:
		unsigned int m_texture;
		float m_aspectRatio;
		bool m_focused;
	};

	class SceneWindow : public GuiWindow {
	public:
		SceneWindow(Scene* scene);
		SceneWindow();

		inline void setScene(Scene* scene) { m_scene = scene; }

		inline virtual const char* getWindowName() { return "Scene"; }
		virtual void renderWindow() override;

	private:
		Scene* m_scene;
	};

	class PropertiesWindow : public GuiWindow {
	public:
		PropertiesWindow(Scene* scene);
		PropertiesWindow();

		void setScene(Scene* scene);

		inline virtual const char* getWindowName() { return "Properties"; }
		virtual void renderWindow() override;

	private:
		void drawComponentVariableGui(const PublicVariable* editorVariable);

	private:
		Scene* m_scene;
	};

}