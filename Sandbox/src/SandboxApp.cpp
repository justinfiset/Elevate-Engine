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
#include "ElevateEngine/Renderer/Light/DirectionalLight.h"
#include "ElevateEngine/Renderer/Light/PointLight.h"

#include "tinyfiledialogs.h"

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

// TODO 
// - rename fichier EditorLayout en EditorLayer
class DebugLayer : public Elevate::Layer
{
public:
    Elevate::ShaderPtr m_Shader;
private:
    Elevate::GameObjectPtr m_DemoObject;
    Elevate::GameObjectPtr m_PointLightObject;
    // TODO make an ortographic and perspective cam class

    Elevate::ScenePtr m_Scene;

    bool blinn = true;

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

        m_Scene->SetSkybox(Elevate::Cubemap::CreateFromFile("cubemap/default.sky"));

        // TODO impl dans un API a part entiere
        //// Bo�te de dialogue pour choisir un fichier
        //const char* filePath = tinyfd_openFileDialog(
        //    "Find a skybox",
        //    "",
        //    0,
        //    NULL,
        //    NULL,
        //    0
        //);
        // TODO impl dans un API a part entiere
        //tinyfd_messageBox(
        //    "Erreur",
        //    "Une erreur s'est produite lors de l'ex�cution.",
        //    "ok",
        //    "error",
        //    1
        //);

        // Backpack
        // TODO AJOUTER DIRECTEMENT A LA SCENE PAR LA CONSTRUCTEUR ET AJOUTER COMME ROOT OBJECT
        // TODO AJOUTER UN ARGUMENT DANS LE CONSTRUCTEUR DE CREATE POUR SET LE PARENT -> SI NULL, ROOT OBJECT,
        //      IMPLEMENTER CETTE METHODE DANS LA SCENE.H
        m_DemoObject = Elevate::GameObject::Create("Backpack", m_Scene);
        Elevate::Model& demoModel = m_DemoObject->AddComponent<Elevate::Model>("backpack.obj");
        m_DemoObject->SetPosition({ 0.0f, 0.0f, -3.0f });

        // point light
        m_PointLightObject = Elevate::GameObject::Create("Point Light", m_Scene);
        m_PointLightObject->SetParent(m_DemoObject);
        m_PointLightObject->SetPosition({ -2.0f, 0.0f, 0.0f });

        m_Shader->Bind();

        Elevate::DirectionalLight dirLight;
        m_Shader->UseDirLight(&dirLight);

        // todo envoyer dans la classe pointlight
        // Set avoir le composant que l'on va cr�er
        // TODO ne plus utiliser le GetTransform, faire les méthodes appropriées pour que le tout soit dans l'interface
        m_Shader->SetUniform3f("pointLights[0].position", m_PointLightObject->GetGlobalPosition());
        m_Shader->SetUniform3f("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        m_Shader->SetUniform3f("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        m_Shader->SetUniform3f("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        m_Shader->SetUniform1f("pointLights[0].constant", 1.0f);
        m_Shader->SetUniform1f("pointLights[0].linear", 0.09f);
        m_Shader->SetUniform1f("pointLights[0].quadratic", 0.032f);

        // TODO SET AUTOMATICLY SOMEWHERE (PROB IN PROJECT???)
        Elevate::SceneManager::LoadScene(m_Scene);
    }

    // TODO ajouter un icon de point light qui suit avec imgui la point light
    void OnRender() override {
        Elevate::EditorCamera* cam = Elevate::Editor::EditorLayer::Get().GetCamera();

        glm::mat4 view = glm::mat4(glm::mat3(cam->GenViewMatrix()));
        glm::vec3 camPos = cam->gameObject->GetPosition();

        // On soumet les models et on les affiches en dessinant la stack
        // TODO -> passer par les commande Renderer:: ... pour faire le rendu � la place
        m_Shader->Bind();
        m_Shader->SetUniform1i("blinn", blinn);
        m_Shader->SetUniform3f("pointLights[0].position", m_PointLightObject->GetGlobalPosition());
        m_Shader->SetUniform3f("camPos", camPos);
        m_Shader->SetProjectionViewMatrix(*cam);

        m_Scene->RenderScene(cam);
    }

    void OnUpdate() override
    {
        // Toggle the blinn light mode
        if (Elevate::Input::IsKeyDown(EE_KEY_B)) {
            EE_CORE_TRACE("Blinn toggled! : Now {}", blinn);
            blinn = !blinn;
        }

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