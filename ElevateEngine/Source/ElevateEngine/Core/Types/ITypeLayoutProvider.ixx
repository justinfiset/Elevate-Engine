export module Elevate.Core.Types.ITypeLayoutProvider;

import Elevate.Core.Types.TypeLayout;

export namespace Elevate
{
    class ITypeLayoutProvider
    {
    public:
        virtual ~ITypeLayoutProvider() = default;

        virtual TypeLayout GetLayout() const = 0;
    };
}