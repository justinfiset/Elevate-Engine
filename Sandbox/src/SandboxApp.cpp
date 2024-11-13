#include <ElevateEngine.h>

#include "imgui/imgui.h"

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

#include "ElevateEngine/Scene/Scene.h"
#include "ElevateEngine/Core/GameObject.h"
#include "ElevateEngine/Core/Transform.h"
#include <ElevateEngine/Scene/SceneManager.h>
#include "ElevateEngine/Editor/Camera/EditorCamera.h"

// TODO 
// - rename fichier EditorLayout en EditorLayer
class DebugLayer : public Elevate::Layer
{
private:
    Elevate::ShaderPtr m_Shader;
    Elevate::GameObjectPtr m_DemoObject;
    Elevate::GameObjectPtr m_PointLightObject;

    // TODO make an ortographic and perspective cam class

    // Light and env settings
    std::unique_ptr<Elevate::Cubemap> m_Cubemap;

    Elevate::ScenePtr m_Scene;

public:
    // TODO Change from debug to edtitor
    DebugLayer() : Layer("Debug") { }

    void OnAttach() override
    {   
        // Scenes creation
        m_Scene = std::make_shared<Elevate::Scene>("Demo Scene");

        uint32_t glslVersion = 330;
        uint32_t glslPointLightCount = 1;

        std::string glslVesionDefine = "#version " + std::to_string(glslVersion);
        std::string glslPointLightCountDefine = "#define NR_POINT_LIGHTS " + std::to_string(glslPointLightCount);

        m_Shader.reset(Elevate::Shader::CreateFromFiles(
            "shader/main.vert",
            "shader/main.frag",
            glslVesionDefine,
            glslVesionDefine + "\n" + glslPointLightCountDefine
        ));

        m_Cubemap.reset(Elevate::Cubemap::CreateFromFile("cubemap/default.sky"));

        /// MATRIX PRINTING
        //for (int i = 0; i < 4; ++i) {
        //    std::string line;
        //    for (int j = 0; j < 4; ++j) {
        //        line.append(std::to_string(m_ModelMatrix[i][j]) + " ");
        //    }
        //    EE_TRACE(line);
        //}

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
        demoModel.SetShader(m_Shader);
        m_DemoObject->SetPosition({ 0.0f, 0.0f, -3.0f });

        // point light
        m_PointLightObject = Elevate::GameObject::Create("Point Light", m_Scene);
        m_PointLightObject->SetPosition({ -2.0f, 0.0f, 0.0f });

        m_Shader->Bind();

        Elevate::DirectionalLight dirLight;
        m_Shader->UseDirLight(&dirLight);

        // SETTING THE MATERIAL /////////////////////////////////////////
        Elevate::Material material /*
        (
            { 0.5f, 0.5f, 0.5f }, // Ambient
            { 0.5f, 0.5f, 0.5f }, // Diffuse
            { 0.5f, 0.5f, 0.5f }, // Specular
            1.0f                  // Shine
        )*/;
        m_Shader->UseMaterial(&material);

        // todo envoyer dans la classe pointlight
        // Set avoir le composant que l'on va cr�er
        m_Shader->SetUniform3f("pointLights[0].position", m_PointLightObject->GetPosition());
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

        m_Cubemap->SetProjectionMatrix(cam->GetProjectionMatrix());
        m_Cubemap->SetViewMatrix(view);
        m_Cubemap->Draw();

        // On soumet les models et on les affiches en dessinant la stack
        // TODO -> passer par les commande Renderer:: ... pour faire le rendu � la place
        m_Shader->Bind();
        m_Shader->SetModelMatrix(*m_DemoObject);
        m_Shader->SetUniform3f("pointLights[0].position", m_PointLightObject->GetPosition());
        m_Shader->SetUniform3f("camPos", camPos);
        m_Shader->SetProjectionViewMatrix(*cam);

        m_Scene->RenderScene();
    }

    void OnUpdate() override
    {
        m_Scene->UpdateScene();
    }

    void OnEvent(Elevate::Event& event) override
    {
        if (event.GetEventType() == Elevate::EventType::MouseMoved)
        {
            //Elevate::MouseMovedEvent& mouseEvent = dynamic_cast<Elevate::MouseMovedEvent&>(event);
        }

        m_Scene->Notify(event);
    }

    void OnImGuiRender() override
    {
        //// ENVIRONMENT /////////////////////////////////////////
        //ImGui::Begin("Environment");
        //ImGui::SeparatorText("Skybox");
        //ImGui::End();
        //////////////////////////////////////////////////////////

        // TODO MOVE DANS LE DEBUG , FAIRE UN StatisticPanel
        ImGuiIO& io = ImGui::GetIO();
        if (ImGui::Begin("Stats"))
        {
            ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);       
            ImGui::End();
        }
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

Elevate::Application* Elevate::CreateApplication() {
    return new Sandbox();
}