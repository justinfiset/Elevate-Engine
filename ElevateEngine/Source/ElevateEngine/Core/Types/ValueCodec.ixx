export module Elevate.Core.Types.ValueCodec;

import Elevate.Foundations.Data;
import Elevate.Serialization.PropertyField;

export namespace Elevate
{
    struct ValueCodec
    {
        // Memory -> PropertyField
        void (*Capture)(const void *data, PropertyField &out) = nullptr;
        // PropertyField -> Memory
        bool (*Apply)(void *data, const PropertyField &in) = nullptr;
    };

    class ValueCodecRegistry
    {
    public:
        static const ValueCodec *Find(EngineDataType type);
    };
}
