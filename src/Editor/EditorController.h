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

		virtual std::string getName() const { return "EditorController"; }

		virtual void update(float deltatime) override;

	private:
		Vector3 getDeltaMouseOnPlane(Vector3 planeOrigin, Vector3 dirAlongPlane);

	private:
		Editor* m_editor = nullptr;
		ObjectPicker m_objectPicker;
		bool m_transformMoveSelected[3] = {false, false, false};
		bool m_transformGizmoWasSelected = false;
	};

}
