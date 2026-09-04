#include "RenderCommand.h"

#include <ElevateEngine/Renderer/Material.h>

namespace Elevate
{
	uint64_t RenderCommand::GetSortKey() const
	{
		uint64_t key = 0;

        // 16 bits for the shader id
        uint16_t shaderID = 0;
        if (m_MaterialInstance)
        {
            if (auto shader = m_MaterialInstance->GetShader())
            {
                shaderID = static_cast<uint16_t>(shader->GetID());
            }
        }
        key |= (static_cast<uint64_t>(shaderID) << 48);

        // 16 bits for the material id
        uint16_t materialID = 0;
        if (m_MaterialInstance)
        {
            materialID = static_cast<uint16_t>(m_MaterialInstance->GetID());
        }
        key |= (static_cast<uint64_t>(materialID) << 32);

        // 32 bits for depth and distance
        float depthValue = Transform[3][2];
        uint32_t depthKey = *reinterpret_cast<const uint32_t*>(&depthValue);
        key |= depthKey;

		return key;
	}
}