export module Elevate.Core.Scene;

// GCC 15 workaround for PR c++/124309: the primary interface must directly
// import a module that several partitions import as well, otherwise reading
// this BMI fails with "Bad import dependency".
import Elevate.Core.Objects.ObjectPtr;

export import :Fwd;

export import :Component;
export import :ComponentRegistry;
export import :ComponentTypeTrait;

export import :GameObject;

export import :Scene;
export import :SceneManager;
export import :SceneType;
