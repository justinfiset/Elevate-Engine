module;

export module Elevate.Renderer.Camera;

import Elevate.Core.Objects.Category;
import Elevate.Core.Objects.Object;
import Elevate.Core.Objects.ObjectPtr;
import Elevate.Core.Reflection.ReflectionTags;
import Elevate.Core.Scene;
import Elevate.Core.Types.TypeField;
import Elevate.Core.Types.TypeLayout;
import Elevate.Core.Types.TypeRegistry;
import Elevate.Editor.Resources;
import Elevate.Editor.Types.EditorTypeTrait;
import Elevate.Foundations;
import Elevate.Foundations.CoreLogger;
import Elevate.Foundations.Enums;
import Elevate.Renderer.Texture;
import Elevate.Serialization.PropertyField;

export import :Fwd;

export import :CameraProjectionMode;
export import :CameraProjection;
export import :PerspectiveProjection;
export import :OrthographicProjection;

export import :Camera;
export import :CameraManager;
