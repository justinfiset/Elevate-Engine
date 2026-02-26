#include <ElevateEngine.h>

// MATHS
#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ElevateEngine/Renderer/Cubemap.h"
#include "ElevateEngine/Renderer/Shader/ShaderManager.h"
#include "ElevateEngine/Renderer/Shader/Shader.h"
#include <ElevateEngine/Renderer/Camera/CameraManager.h>

class DebugLayer : public Elevate::SceneLayer
{
public:
	Elevate::ShaderPtr m_shader;
private:
	std::shared_ptr<Elevate::GameObject> m_DemoObject;
	std::shared_ptr<Elevate::GameObject> m_demoCube;
	std::shared_ptr<Elevate::GameObject> m_PointLightObject;
public:
	DebugLayer() : SceneLayer(Elevate::Scene::Create("Demo Scene")) { }

	void OnAttach() override
	{
		uint32_t glslVersion = 410;
		uint32_t glslPointLightCount = 1;
		std::string glslVesionDefine = "#version " + std::to_string(glslVersion);
		std::string glslPointLightCountDefine = "#define NR_POINT_LIGHTS " + std::to_string(glslPointLightCount);
		m_shader = Elevate::ShaderManager::LoadShader(
			"main",
			"Content/Shaders/main.vert",
			"Content/Shaders/main.frag",
			glslVesionDefine,
			(glslVesionDefine + "\n" + glslPointLightCountDefine)
		);

		Elevate::MaterialPtr material = Elevate::MaterialRegistry::LoadMaterial(m_shader);
		material->Set("material.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		material->Set("material.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		material->Set("material.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		material->Set("material.shininess", 32.0f);

		m_scene->SetSkybox("Engine/Cubemap/default.sky");

		// TODO impl dans un API a part entiere
		//tinyfd_messageBox(
		//    "Erreur",
		//    "Une erreur s'est produite lors de l'ex�cution.",
		//    "ok",
		//    "error",
		//    1
		//);

		// Backpack
		m_DemoObject = Elevate::GameObject::Create("Backpack", m_scene);
		Elevate::Model& demoModel = m_DemoObject->AddComponent<Elevate::Model>("Content/Models/backpack.obj");
		demoModel.SetMaterial(material);
		//Elevate::Rigidbody& rb = m_DemoObject->AddComponent<Elevate::Rigidbody>();
		m_DemoObject->SetPosition({ 0.0f, 0.0f, -4.0f });

		m_demoCube = Elevate::GameObject::Create("Cube", m_scene);
		Elevate::Model& demoModel1 = m_demoCube->AddComponent<Elevate::Model>(Elevate::PrimitiveType::Cube);
		//Elevate::Rigidbody& rb1 = m_demoCube->AddComponent<Elevate::Rigidbody>();
		Elevate::Camera& cam = m_demoCube->AddComponent<Elevate::Camera>();
		m_demoCube->SetPosition({ 0.0f, 0.0f, 0.0f });
		m_demoCube->SetRotation({ 0.0, -90.0f, 0.0f });

		// point light
		m_PointLightObject = Elevate::GameObject::Create("Point Light", m_scene);
		m_PointLightObject->SetParent(m_DemoObject);
		m_PointLightObject->SetPosition({ -2.0f, 0.0f, 2.0f });
		Elevate::PointLight& pointLight = m_PointLightObject->AddComponent<Elevate::PointLight>(glm::vec3(1.0f, 1.0f, 1.0f));
		pointLight.SetIntensity(0.5);

		auto m_dirLightObj = Elevate::GameObject::Create("Directional Light", m_scene);
		m_dirLightObj->SetRotation({ 0.0f, 45.0f, 20.0f });
		Elevate::DirectionalLight& dirLight = m_dirLightObj->AddComponent<Elevate::DirectionalLight>(
			glm::vec3(1.0f, 1.0f, 1.0f)
		);
		dirLight.SetIntensity(0.1f);
		
		// TODO CONSTRUIRE AUTOMATIQUEMENT VIA LA SCÈNE!!!!
		m_scene->SetLighting(std::make_unique<Elevate::SceneLighting>(
			&dirLight,
			std::vector<Elevate::PointLight*>{ &pointLight }
		));
	}

	// TODO ajouter un icon de point light qui suit avec imgui la point light
	void OnRender() override {
		Elevate::Camera* cam = Elevate::CameraManager::GetCurrent();
		SceneLayer::OnRender(cam);
	}

	void OnUpdate() override
	{
		SceneLayer::OnUpdate();
	}

	void OnEvent(Elevate::Event& event) override
	{
		switch (event.GetEventType())
		{
		case Elevate::EventType::KeyTyped:
			Elevate::KeyPressedEvent kp = (Elevate::KeyPressedEvent&)event;
			if (kp.GetKeyCode() == EE_KEY_SPACE)
			{
				// Do something here...
			}
			break;
		}

		SceneLayer::OnEvent(event);
	}

	void OnImGuiRender() override
	{
	}
};

class Sandbox : public Elevate::Application
{
public:
	Sandbox() 
	{
		PushLayer(new DebugLayer());
	}
	~Sandbox() = default;
};

Elevate::Application* Elevate::CreateApplication() 
{
	return new Sandbox();
}   