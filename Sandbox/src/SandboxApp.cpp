#include <ElevateEngine.h>

#include <ImGui/imgui.h>

#include "ElevateEngine/Renderer/Material.h"
#include "ElevateEngine/Renderer/Mesh.h"
#include "ElevateEngine/Renderer/Camera.h"
#include "ElevateEngine/Renderer/Model.h"
#include "ElevateEngine/Editor/EditorLayer.h"

// MATHS
#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/include/GLFW/glfw3.h>

#include "ElevateEngine/Renderer/Cubemap.h"
#include "ElevateEngine/Renderer/Light/SceneLighting.h"

#include "ElevateEngine/ImGui/CustomImGuiCommand.h"
#include "ElevateEngine/ImGui/ImGuiTheme.h"
#include <ElevateEngine/Renderer/FrameBuffer.h>

#include <ElevateEngine/Inputs/Input.h>
#include "ElevateEngine/Renderer/Shader/ShaderManager.h"
#include "ElevateEngine/Renderer/Shader/Shader.h"
#include "ElevateEngine/Scene/Scene.h"
#include "ElevateEngine/Core/GameObject.h"
#include "ElevateEngine/Core/Transform.h"
#include <ElevateEngine/Scene/SceneManager.h>
#include "ElevateEngine/Editor/Camera/EditorCamera.h"

#include <ElevateEngine/Physics.h>

class DebugLayer : public Elevate::Layer
{
public:
    Elevate::ShaderPtr m_Shader;
private:
    std::shared_ptr<Elevate::GameObject> m_DemoObject;
    std::shared_ptr<Elevate::GameObject> m_PointLightObject;

    Elevate::ScenePtr m_Scene;
public:
    DebugLayer() : Layer("Debug") { }

    void OnAttach() override
    {   
        Elevate::MaterialPtr material = Elevate::Material::Create(
            { 0.5f, 0.0f, 0.0f }, // Ambient
            { 0.5f, 0.0f, 0.0f }, // Diffuse
            { 0.5f, 0.0f, 0.0f }, // Specular
            128.0f                // Shine
        );

        // Scenes creation
        m_Scene = Elevate::Scene::Create("Demo Scene");

        uint32_t glslVersion = 410;
        uint32_t glslPointLightCount = 1;

        std::string glslVesionDefine = "#version " + std::to_string(glslVersion);
        std::string glslPointLightCountDefine = "#define NR_POINT_LIGHTS " + std::to_string(glslPointLightCount);
        
        m_Shader = Elevate::ShaderManager::LoadShader(
            "default",
            "shader/main.vert",
            "shader/main.frag",
            glslVesionDefine,
            (glslVesionDefine + "\n" + glslPointLightCountDefine)
        );

        m_Scene->SetSkybox("cubemap/default.sky");

        // TODO impl dans un API a part entiere
        //tinyfd_messageBox(
        //    "Erreur",
        //    "Une erreur s'est produite lors de l'ex�cution.",
        //    "ok",
        //    "error",
        //    1
        //);

        // Backpack
        m_DemoObject = Elevate::GameObject::Create("Backpack", m_Scene);
        Elevate::Model& demoModel = m_DemoObject->AddComponent<Elevate::Model>("backpack.obj");
        Elevate::Rigidbody& rb = m_DemoObject->AddComponent<Elevate::Rigidbody>();
        m_DemoObject->SetPosition({ 0.0f, 0.0f, -3.0f });

        m_DemoObject = Elevate::GameObject::Create("Backpack", m_Scene);
        Elevate::Model& demoModel1 = m_DemoObject->AddComponent<Elevate::Model>(Elevate::PrimitiveType::Cube);
        Elevate::Rigidbody& rb1 = m_DemoObject->AddComponent<Elevate::Rigidbody>();
        m_DemoObject->SetPosition({ 0.0f, 0.0f, 0.0f });

        //m_DemoObject = Elevate::GameObject::Create("Backpack", m_Scene);
        //Elevate::Model& demoModel2 = m_DemoObject->AddComponent<Elevate::Model>("backpack.obj");
        //Elevate::Rigidbody& rb2 = m_DemoObject->AddComponent<Elevate::Rigidbody>();
        //m_DemoObject->SetPosition({ 0.0f, 0.0f, 3.0f });

        //m_DemoObject = Elevate::GameObject::Create("Backpack", m_Scene);
        //Elevate::Model& demoModel3 = m_DemoObject->AddComponent<Elevate::Model>("backpack.obj");
        //Elevate::Rigidbody& rb3 = m_DemoObject->AddComponent<Elevate::Rigidbody>();
        //m_DemoObject->SetPosition({ 0.0f, 0.0f, 6.0f });

        //m_DemoObject = Elevate::GameObject::Create("Backpack", m_Scene);
        //Elevate::Model& demoModel4 = m_DemoObject->AddComponent<Elevate::Model>("backpack.obj");
        //Elevate::Rigidbody& rb4 = m_DemoObject->AddComponent<Elevate::Rigidbody>();
        //m_DemoObject->SetPosition({ 0.0f, 0.0f, 9.0f });

        //m_DemoObject = Elevate::GameObject::Create("Backpack", m_Scene);
        //Elevate::Model& demoModel5 = m_DemoObject->AddComponent<Elevate::Model>("backpack.obj");
        //Elevate::Rigidbody& rb5 = m_DemoObject->AddComponent<Elevate::Rigidbody>();
        //m_DemoObject->SetPosition({ 0.0f, 0.0f, 12.0f });

        //m_DemoObject = Elevate::GameObject::Create("Backpack", m_Scene);
        //Elevate::Model& demoModel6 = m_DemoObject->AddComponent<Elevate::Model>("backpack.obj");
        //Elevate::Rigidbody& rb6 = m_DemoObject->AddComponent<Elevate::Rigidbody>();
        //m_DemoObject->SetPosition({ 0.0f, 0.0f, 15.0f });

        // point light
        m_PointLightObject = Elevate::GameObject::Create("Point Light", m_Scene);
        m_PointLightObject->SetParent(m_DemoObject);
        m_PointLightObject->SetPosition({ -2.0f, 0.0f, 2.0f });
        Elevate::PointLight& pointLight = m_PointLightObject->AddComponent<Elevate::PointLight>(glm::vec3(1.0f, 1.0f, 1.0f));
        pointLight.SetIntensity(0.5);

        auto m_dirLightObj = Elevate::GameObject::Create("Directional Light", m_Scene);
        m_dirLightObj->SetRotation({ 0.0f, 45.0f, 20.0f });
        Elevate::DirectionalLight& dirLight = m_dirLightObj->AddComponent<Elevate::DirectionalLight>(
            glm::vec3(1.0f, 1.0f, 1.0f)
        );
        dirLight.SetIntensity(0.1f);
        
        // TODO CONSTRUIRE AUTOMATIQUEMENT VIA LA SCÈNE!!!!
        m_Scene->SetLighting(std::make_unique<Elevate::SceneLighting>(
            &dirLight,
            std::vector<Elevate::PointLight*>{ &pointLight }
        ));
    }

    // TODO ajouter un icon de point light qui suit avec imgui la point light
    void OnRender() override {
        Elevate::EditorCamera* cam = Elevate::Editor::EditorLayer::Get().GetCamera();

        glm::mat4 view = glm::mat4(glm::mat3(cam->GenViewMatrix()));
        glm::vec3 camPos = cam->gameObject->GetPosition();

        m_Shader->Bind();
        m_Shader->UpdateCamera(*cam);
        m_Scene->RenderScene(cam);
    }

    void OnUpdate() override
    {
        m_Scene->UpdateScene();
    }

    void OnEvent(Elevate::Event& event) override
    {
        // Example d'utilisation
        if (event.GetEventType() == Elevate::EventType::MouseMoved)
        {
            //Elevate::MouseMovedEvent& mouseEvent = dynamic_cast<Elevate::MouseMovedEvent&>(event);
        }

        m_Scene->Notify(event);
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
        PushLayer(new Elevate::Editor::EditorLayer()); // TODO AUTOMATISER SI ON EST DANS L'ÉDITEUR, NE PAS METTRE POUR UNE APP
    }
    ~Sandbox() = default;
};

Elevate::Application* Elevate::CreateApplication() 
{
    return new Sandbox();
}