module;


#include <ElevateEngine/Core/Scene/SceneEnTTADL.h>
#include <memory>
#include <new>
#include <typeinfo>

export module Elevate.Editor.DI;

// Foundations
import Elevate.Foundations.CoreLogger;
import Elevate.Foundations.Services;
// Core
import Elevate.Core.Scene;
// Renderer
import Elevate.Renderer.Materials.Material;
// Editor
import Elevate.Editor.Renderer.AssetPreviewSystem;
import Elevate.Editor.Renderer.MaterialPreviewRenderer;

export namespace Elevate
{
	class EditorServiceModule : public IServiceModule
	{
	public:
		void Register(ServiceCollection& services) override
		{
			services.RegisterSingleton<AssetPreviewSystem>([this](ServiceProvider& provider)
				{
					auto previewSystem = std::make_shared<AssetPreviewSystem>();
					RegisterPreviewRenderers(*previewSystem);
					return previewSystem;
				});

			CoreLogger::Info("Editor Service Module Registered.");
		}

		void RegisterPreviewRenderers(AssetPreviewSystem& previewSystem)
		{
			previewSystem.Register<Material, MaterialPreviewRenderer>();
		}
	};
}
