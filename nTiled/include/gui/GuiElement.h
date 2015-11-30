#pragma once
#include "camera\Camera.h"

namespace nTiled_gui {
	class GuiElement {
	public:
		virtual ~GuiElement() {}
		virtual void render() = 0;
	};

	class CameraElement : public GuiElement {
	public:
		CameraElement(Camera& camera);
		void render();
	private:
		Camera& camera;
	};
}
