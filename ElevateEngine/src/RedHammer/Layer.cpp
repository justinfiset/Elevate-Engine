#include "rhpch.h"
#include "Layer.h"

namespace Hammer
{
	Layer::Layer(const std::string& debugName)
		: m_DebugName(debugName) { }

	Layer::~Layer() { }
}