#pragma
#include <memory>

namespace glGame {

	class Scene;
	class PublicVariable;

	class GuiWindow {
	public:
		GuiWindow() = default;

		virtual const char* getWindowName() = 0;
		virtual void renderWindow() = 0;

	public:
		bool isOpen = true;
	};

	class ViewportWindow : public GuiWindow {
	public:
		ViewportWindow(unsigned int texture, float aspectRatio);

		inline virtual const char* getWindowName() { return "Viewport"; }
		virtual void renderWindow() override;

	private:
		unsigned int m_texture;
		float m_aspectRatio;
	};

	class SceneWindow : public GuiWindow {
	public:
		SceneWindow(Scene* scene);

		inline virtual const char* getWindowName() { return "Scene"; }
		virtual void renderWindow() override;

	private:
		Scene* m_scene;
	};

	class PropertiesWindow : public GuiWindow {
	public:
		PropertiesWindow(Scene* scene);

		inline virtual const char* getWindowName() { return "Properties"; }
		virtual void renderWindow() override;

	private:
		void drawComponentVariableGui(const PublicVariable* editorVariable);

	private:
		Scene* m_scene;
	};

}