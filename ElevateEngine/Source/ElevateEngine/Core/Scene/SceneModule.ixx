module;

export module Elevate.Core.Scene;

// GCC 15 workaround for PR c++/124309: the primary interface must directly
// import modules also imported (directly or transitively) by its partitions,
// otherwise reading this BMI fails with "Bad import dependency".
import Elevate.Audio.Object;
import Elevate.Core.Events.Event;
import Elevate.Core.Objects.Category;
import Elevate.Core.Objects.Object;
import Elevate.Core.Objects.ObjectPtr;
import Elevate.Core.Reflection.ReflectionTags;
import Elevate.Core.Transform.ITransformable;
import Elevate.Core.Types.ITypeTrait;
import Elevate.Core.Types.TypeField;
import Elevate.Core.Types.TypeLayout;
import Elevate.Core.Types.TypeRegistry;
import Elevate.Editor.Resources;
import Elevate.Editor.Types.EditorTypeTrait;
import Elevate.Foundations.Assert;
import Elevate.Foundations.CoreLogger;
import Elevate.Foundations.Enums;
import Elevate.Foundations.Services;
import Elevate.Renderer.Texture;
import Elevate.Serialization.PropertyField;

export import :Fwd;

export import :Component;
export import :ComponentRegistry;
export import :ComponentTypeTrait;

export import :GameObject;

export import :Scene;
export import :SceneManager;
export import :SceneType;
