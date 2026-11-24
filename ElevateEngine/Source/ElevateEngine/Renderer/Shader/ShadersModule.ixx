module;

#include <ElevateEngine/Core/Scene/SceneEnTTOperators.h>

export module Elevate.Renderer.Shaders;

// GCC 15 PR c++/124309: the primary interface must also import the modules
// used by its partitions, or importers cannot read this BMI.
import Elevate.Core.Assets.Asset;
import Elevate.Core.Objects.Category;
import Elevate.Core.Objects.Object;
import Elevate.Core.Objects.ObjectPtr;
import Elevate.Core.Reflection.ReflectionTags;
import Elevate.Core.Transform.ITransformable;
import Elevate.Core.Types.TypeField;
import Elevate.Core.Types.TypeLayout;
import Elevate.Core.Types.TypeRegistry;
import Elevate.Editor.Resources;
import Elevate.Editor.Types.EditorTypeTrait;
import Elevate.Foundations.CoreLogger;
import Elevate.Foundations.Guid;
import Elevate.Renderer.Buffers;
import Elevate.Renderer.Lights.Light;
import Elevate.Renderer.Texture;
import Elevate.Serialization.PropertyField;

export import :Fwd;
export import :Shader;
export import :ShaderManager;
export import :ShaderHeader;
export import :ShaderAsset;
