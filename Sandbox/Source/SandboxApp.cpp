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

// TODO remove this is temporary as without the include the component is not registered
#include <ElevateEngine/Audio/Components/AudioDistanceProbe.h>

class DebugLayer : public Elevate::SceneLayer
{
public:
    Elevate::ShaderPtr m_Shader;
private:
    std::shared_ptr<Elevate::GameObject> m_DemoObject;
    std::shared_ptr<Elevate::GameObject> m_demoCube;
    std::shared_ptr<Elevate::GameObject> m_PointLightObject;
public:
    DebugLayer() : SceneLayer(Elevate::Scene::Create("Demo Scene")) { }

    void OnAttach() override
    {
        Elevate::MaterialPtr material = Elevate::Material::Create(
            { 0.5f, 0.0f, 0.0f }, // Ambient
            { 0.5f, 0.0f, 0.0f }, // Diffuse
            { 0.5f, 0.0f, 0.0f }, // Specular
            128.0f                // Shine
        );

        uint32_t glslVersion = 410;
        uint32_t glslPointLightCount = 1;

        std::string glslVesionDefine = "#version " + std::to_string(glslVersion);
        std::string glslPointLightCountDefine = "#define NR_POINT_LIGHTS " + std::to_string(glslPointLightCount);
        
        m_Shader = Elevate::ShaderManager::LoadShader(
            "default",
            "Content/Shaders/main.vert",
            "Content/Shaders/main.frag",
            glslVesionDefine,
            (glslVesionDefine + "\n" + glslPointLightCountDefine)
        );

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
        Elevate::Rigidbody& rb = m_DemoObject->AddComponent<Elevate::Rigidbody>();
        m_DemoObject->SetPosition({ 0.0f, 0.0f, -3.0f });

        m_demoCube = Elevate::GameObject::Create("Cube", m_scene);
        Elevate::Model& demoModel1 = m_demoCube->AddComponent<Elevate::Model>(Elevate::PrimitiveType::Cube);
        Elevate::Rigidbody& rb1 = m_demoCube->AddComponent<Elevate::Rigidbody>();
        Elevate::Camera& cam = m_demoCube->AddComponent<Elevate::Camera>();
        m_demoCube->SetPosition({ 0.0f, 0.0f, 0.0f });
        m_demoCube->SetRotation({ 0.0, -90.0f, 0.0f });

        //m_DemoObject = Elevate::GameObject::Create("Backpack", m_scene);
        //Elevate::Model& demoModel2 = m_DemoObject->AddComponent<Elevate::Model>("backpack.obj");
        //Elevate::Rigidbody& rb2 = m_DemoObject->AddComponent<Elevate::Rigidbody>();
        //m_DemoObject->SetPosition({ 0.0f, 0.0f, 3.0f });

        //m_DemoObject = Elevate::GameObject::Create("Backpack", m_scene);
        //Elevate::Model& demoModel3 = m_DemoObject->AddComponent<Elevate::Model>("backpack.obj");
        //Elevate::Rigidbody& rb3 = m_DemoObject->AddComponent<Elevate::Rigidbody>();
        //m_DemoObject->SetPosition({ 0.0f, 0.0f, 6.0f });

        //m_DemoObject = Elevate::GameObject::Create("Backpack", m_scene);
        //Elevate::Model& demoModel4 = m_DemoObject->AddComponent<Elevate::Model>("backpack.obj");
        //Elevate::Rigidbody& rb4 = m_DemoObject->AddComponent<Elevate::Rigidbody>();
        //m_DemoObject->SetPosition({ 0.0f, 0.0f, 9.0f });

        //m_DemoObject = Elevate::GameObject::Create("Backpack", m_scene);
        //Elevate::Model& demoModel5 = m_DemoObject->AddComponent<Elevate::Model>("backpack.obj");
        //Elevate::Rigidbody& rb5 = m_DemoObject->AddComponent<Elevate::Rigidbody>();
        //m_DemoObject->SetPosition({ 0.0f, 0.0f, 12.0f });

        //m_DemoObject = Elevate::GameObject::Create("Backpack", m_Scene);
        //Elevate::Model& demoModel6 = m_DemoObject->AddComponent<Elevate::Model>("backpack.obj");
        //Elevate::Rigidbody& rb6 = m_DemoObject->AddComponent<Elevate::Rigidbody>();
        //m_DemoObject->SetPosition({ 0.0f, 0.0f, 15.0f });

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
        m_Shader->Bind();
        m_Shader->UpdateCamera(*cam); // TODO make the camera upload herself to the shader and check if there was any changes => if(changed) then updateUniforms()
        SceneLayer::OnRender();
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
                Elevate::SoundEngine::PostEvent("Play_Sandstep", m_DemoObject.get());
                EE_TRACE("Space bar pressed");
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