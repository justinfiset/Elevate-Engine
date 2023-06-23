#include "eepch.h"
#include "Layer.h"

namespace Elevate
{
	Layer::Layer(const std::string& debugName)
		: m_DebugName(debugName) { }

	Layer::~Layer() { }
}