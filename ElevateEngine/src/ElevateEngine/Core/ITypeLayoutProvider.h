#pragma once

namespace Elevate
{
    class TypeLayout;

    class ITypeLayoutProvider
    {
    public:
        virtual ~ITypeLayoutProvider() = default;

        virtual TypeLayout GetLayout() const = 0;
    };
}