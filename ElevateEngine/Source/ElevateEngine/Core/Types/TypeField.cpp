module Elevate.Core.Types.TypeField;

import Elevate.Core.Types.TypeRegistry;

namespace Elevate
{
    std::type_index Elevate::TypeField::GetTargetType() const
    {
        auto &types = TypeRegistry::GetTypeIndexes();
        auto it = types.find(targetTypeKey);

        if (it == types.end())
        {
            return typeid(void);
        }

        return it->second;
    }
}
