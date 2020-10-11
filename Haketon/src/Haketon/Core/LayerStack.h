#pragma once

#include "Haketon/Core/Core.h"
#include "Layer.h"

#include <vector>


namespace Haketon {

	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* Layer);
		void PushOverlay(Layer* Overlay);
		void PopLayer(Layer* Layer);
		void PopOverlay(Layer* Overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }

	private:
		std::vector<Layer*> m_Layers;
		unsigned int m_LayerInsertIndex = 0;
	};
}
