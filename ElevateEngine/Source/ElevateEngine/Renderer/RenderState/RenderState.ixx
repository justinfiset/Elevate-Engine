export module Elevate.Renderer.RenderState;

export namespace Elevate
{
	enum class CullFace
	{
		None,
		Front,
		Back,
		FrontAndBack
	};

	enum class BlendModeType
	{
		Alpha,
		Additive,
		None
	};

	enum class DepthFunction
	{
		Less,
		LessEqual,
		Equal,
		Always,
		Never,
		Greater,
		GreaterEqual
	};

	struct RenderState
	{
		CullFace CullMode = CullFace::Back;
		bool DepthWrite = true;
		bool DepthTest = true;
		DepthFunction DepthFunc = DepthFunction::Less;
		BlendModeType BlendMode = BlendModeType::None;
	};
}
