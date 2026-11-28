module;

export module Elevate.Physics.Rigidbody;

// GCC 15 workaround for PR c++/124309: every module imported directly by a
// partition must be imported here before a module that imports it indirectly.
// Reflection.h imports Renderer.Texture and Editor.Types.EditorTypeTrait in
// editor builds, and Scene reaches both indirectly, so they must come first.
export import Elevate.Physics.IPhysicsCollider;
export import Elevate.Physics.IPhysicsRigidbody;
import Elevate.Core.Objects.Category;
import Elevate.Core.Objects.Object;
import Elevate.Core.Objects.ObjectPtr;
import Elevate.Core.Reflection.ReflectionTags;
import Elevate.Core.Types.TypeField;
import Elevate.Core.Types.TypeLayout;
import Elevate.Core.Types.TypeRegistry;
#ifdef EE_EDITOR_BUILD
import Elevate.Editor.Resources;
import Elevate.Editor.Types.EditorTypeTrait;
#endif
import Elevate.Foundations.CoreLogger;
import Elevate.Foundations.Enums;
import Elevate.Renderer.Texture;
import Elevate.Serialization.PropertyField;
import Elevate.Core.Scene;

export import :Types;
export import :Component;
