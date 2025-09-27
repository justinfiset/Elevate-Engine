#pragma once

// size in bytes
#define EE_DATA_SIZE_None 0
#define EE_DATA_SIZE_Bool 1
#define EE_DATA_SIZE_Int 4
#define EE_DATA_SIZE_Int2 (EE_DATA_SIZE_Int*2)
#define EE_DATA_SIZE_Int3 (EE_DATA_SIZE_Int*3)
#define EE_DATA_SIZE_Int4 (EE_DATA_SIZE_Int*4)
#define EE_DATA_SIZE_Float 4
#define EE_DATA_SIZE_Float2 (EE_DATA_SIZE_Float*2)
#define EE_DATA_SIZE_Float3 (EE_DATA_SIZE_Float*3)
#define EE_DATA_SIZE_Float4 (EE_DATA_SIZE_Float*4)
#define EE_DATA_SIZE_Mat3 (EE_DATA_SIZE_Float*3*3)
#define EE_DATA_SIZE_Mat4 (EE_DATA_SIZE_Float*4*4)
#define EE_DATA_SIZE_Mat5 (EE_DATA_SIZE_Float*5*5)

#define EE_DATA_COUNT_None 0
#define EE_DATA_COUNT_Bool 1
#define EE_DATA_COUNT_Int 1
#define EE_DATA_COUNT_Int2 2
#define EE_DATA_COUNT_Int3 3
#define EE_DATA_COUNT_Int4 4
#define EE_DATA_COUNT_Float 1
#define EE_DATA_COUNT_Float2 2
#define EE_DATA_COUNT_Float3 3
#define EE_DATA_COUNT_Float4 4
#define EE_DATA_COUNT_Mat3 3*3
#define EE_DATA_COUNT_Mat4 4*4
#define EE_DATA_COUNT_Mat5 5*5

// TODO remove this part to make the engine usable with other rendering api
#define EE_DATA_USEOPENGL
	#ifdef EE_DATA_USEOPENGL
	#define EE_DATA_TYPE_None 0
	#define EE_DATA_TYPE_Bool 0x8856
	#define EE_DATA_TYPE_Int 0x1404
	#define EE_DATA_TYPE_Int2 0x1404
	#define EE_DATA_TYPE_Int3 0x1404
	#define EE_DATA_TYPE_Int4 0x1404
	#define EE_DATA_TYPE_Float 0x1406
	#define EE_DATA_TYPE_Float2 0x1406
	#define EE_DATA_TYPE_Float3 0x1406
	#define EE_DATA_TYPE_Float4 0x1406
	#define EE_DATA_TYPE_Mat3 0x1406
	#define EE_DATA_TYPE_Mat4 0x1406
	#define EE_DATA_TYPE_Mat5 0x1406
#endif

namespace Elevate
{
	const int ShaderDataTypeCount[] = {
		EE_DATA_COUNT_None,  // None
		EE_DATA_COUNT_Bool,  // Bool
		EE_DATA_COUNT_Int,   // Int
		EE_DATA_COUNT_Int2,  // Int2
		EE_DATA_COUNT_Int3,  // Int3
		EE_DATA_COUNT_Int4,  // Int4
		EE_DATA_COUNT_Float, // Float
		EE_DATA_COUNT_Float2,// Float2
		EE_DATA_COUNT_Float3,// Float3
		EE_DATA_COUNT_Float4,// Float4
		EE_DATA_COUNT_Mat3,  // Mat3
		EE_DATA_COUNT_Mat4,  // Mat4
		EE_DATA_COUNT_Mat5   // Mat5
	};

	const int ShaderDataTypeSize[] = {
		EE_DATA_SIZE_None,  // None
		EE_DATA_SIZE_Bool,  // Bool
		EE_DATA_SIZE_Int,   // Int
		EE_DATA_SIZE_Int2,  // Int2
		EE_DATA_SIZE_Int3,  // Int3
		EE_DATA_SIZE_Int4,  // Int4
		EE_DATA_SIZE_Float, // Float
		EE_DATA_SIZE_Float2,// Float2
		EE_DATA_SIZE_Float3,// Float3
		EE_DATA_SIZE_Float4,// Float4
		EE_DATA_SIZE_Mat3,  // Mat3
		EE_DATA_SIZE_Mat4,  // Mat4
		EE_DATA_SIZE_Mat5   // Mat5
	};

	const unsigned int ShaderDataAPIType[] = {
		EE_DATA_TYPE_None,  // None
		EE_DATA_TYPE_Bool,  // Bool
		EE_DATA_TYPE_Int,   // Int
		EE_DATA_TYPE_Int2,  // Int2
		EE_DATA_TYPE_Int3,  // Int3
		EE_DATA_TYPE_Int4,  // Int4
		EE_DATA_TYPE_Float, // Float
		EE_DATA_TYPE_Float2,// Float2
		EE_DATA_TYPE_Float3,// Float3
		EE_DATA_TYPE_Float4,// Float4
		EE_DATA_TYPE_Mat3,  // Mat3
		EE_DATA_TYPE_Mat4,  // Mat4
		EE_DATA_TYPE_Mat5   // Mat5
	};

	enum class EngineDataType
	{
		None = 0,
		Bool,
		Int, Int2, Int3, Int4,
		Float, Float2, Float3, Float4,
		Mat3, Mat4, Mat5,
		Custom,
		Unknown
	};

	inline uint32_t GetDataTypeCount(EngineDataType type) {
		return ShaderDataTypeCount[static_cast<uint32_t>(type)];
	}

	inline uint32_t GetDataTypeSize(EngineDataType type) {
		return ShaderDataTypeSize[static_cast<uint32_t>(type)];
	}

	inline uint32_t GetDataAPIType(EngineDataType type) {
		return ShaderDataAPIType[static_cast<uint32_t>(type)];
	}
}