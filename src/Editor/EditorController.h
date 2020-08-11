#pragma once
#include "../Components/Component.h"
#include "ObjectPicker.h"

namespace glGame {

	class Editor;
	class ViewportWindow;

	class EditorController : public Component {
	public:
		EditorController();

		void setEditor(Editor* editor) { m_editor = editor; }
		void setViewportWindow(ViewportWindow* viewportWindow) { m_viewportWindow = viewportWindow; }

		virtual std::string getName() const { return "EditorController"; }

		virtual void update(float deltatime) override;

	private:
		Editor* m_editor = nullptr;
		ViewportWindow* m_viewportWindow = nullptr;
		ObjectPicker m_objectPicker;
	};

}
