export module Elevate.Core.Types.ITypeLayoutProvider;

export namespace Elevate
{
    class TypeLayout;

    class ITypeLayoutProvider
    {
    public:
        virtual ~ITypeLayoutProvider() = default;

        virtual TypeLayout GetLayout() const = 0;
    };
}