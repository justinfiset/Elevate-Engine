module;

#include <string>

module Elevate.Core.Layers.Layer;

namespace Elevate
{
	Layer::Layer(const std::string& debugName)
		: m_DebugName(debugName)
	{
	}
}