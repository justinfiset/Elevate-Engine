#include <ElevateEngine.h>

#include "imgui/imgui.h"

#include "ElevateEngine/Renderer/Material.h"
#include "ElevateEngine/Renderer/Mesh.h"
#include "ElevateEngine/Renderer/Camera.h"
#include "ElevateEngine/Renderer/Model.h"


// MATHS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/include/GLFW/glfw3.h>

#include "ElevateEngine/Renderer/Cubemap.h"
#include "ElevateEngine/Renderer/Light/DirectionalLight.h"
#include "ElevateEngine/Renderer/Light/PointLight.h"

#include "tinyfiledialogs.h"

class DebugLayer : public Elevate::Layer
{
private:
    bool debugMenuActive = false;

    std::shared_ptr<Elevate::Shader> m_Shader;

    // Camera specific stuff
    float lastX, lastY;
    bool followCursor = false;
    // todo make an ortographic and perspective cam class
    Elevate::Camera cam = Elevate::Camera(60.0f);

    std::unique_ptr<Elevate::Model> m_Model;
    std::unique_ptr<Elevate::Cubemap> m_Cubemap;
public:
    DebugLayer() : Layer("Debug") { }

    void OnAttach() override
    {   
        // TODO in depth testing pour voir si l es texures se load tous comme du monde avec le nouveau systeme
        m_Model = std::make_unique<Elevate::Model>(Elevate::Model("backpack.obj"));

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


        // TODO REMOVE FOLLOWING LINES
        //std::string paths[6] =
        //{
        //    "skybox/graycloud_rt.jpg",
        //    "skybox/graycloud_lf.jpg",
        //    "skybox/graycloud_up.jpg",
        //    "skybox/graycloud_dn.jpg",
        //    "skybox/graycloud_ft.jpg",
        //    "skybox/graycloud_bk.jpg"
        //};
        //m_skyboxShader.reset(Elevate::Shader::CreateFromFiles("shader/skybox.vert", "shader/skybox.frag"));
        //std::string paths[6] =
        //{
        //    "sky/skybox/mountain-skyboxes/Maskonaive/right.jpg",
        //    "sky/skybox/mountain-skyboxes/Maskonaive/left.jpg",
        //    "sky/skybox/mountain-skyboxes/Maskonaive/top.jpg",
        //    "sky/skybox/mountain-skyboxes/Maskonaive/bottom.jpg",
        //    "sky/skybox/mountain-skyboxes/Maskonaive/back.jpg",
        //    "sky/skybox/mountain-skyboxes/Maskonaive/front.jpg"
        //};
        //m_Cubemap = std::make_unique<Elevate::Cubemap>(paths);

        m_Cubemap.reset(Elevate::Cubemap::CreateFromFile("cubemap/default.sky"));

        m_Model->GetMatrix() = glm::translate(m_Model->GetMatrix(), glm::vec3(0.0f, 0.0f, -3.0f));
        //m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        /// MATRIX PRINTING
        //for (int i = 0; i < 4; ++i) {
        //    std::string line;
        //    for (int j = 0; j < 4; ++j) {
        //        line.append(std::to_string(m_ModelMatrix[i][j]) + " ");
        //    }
        //    EE_TRACE(line);
        //}

        glm::vec3 pointLightPositions[] = {
            glm::vec3(-2.0f,  0.0f, 0.0f),
            glm::vec3(-1.0f,  0.0f, 0.0f),
            glm::vec3(0.0f,  0.0f, 0.0f),
            glm::vec3(1.0f,  0.0f, 0.0f)
        };

        m_Shader->Bind();
        // Lighting ////////////////////////////////////////
        // TODO CREATE A LIGHT STRUCT OR CLASS (something like material bellow)
        Elevate::DirectionalLight dirLight;
        dirLight.Use(m_Shader);

        
        //// # light 1
        m_Shader->SetUniform3f("pointLights[0].position", pointLightPositions[0]);
        m_Shader->SetUniform3f("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        m_Shader->SetUniform3f("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        m_Shader->SetUniform3f("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        m_Shader->SetUniform1f("pointLights[0].constant", 1.0f);    
        m_Shader->SetUniform1f("pointLights[0].linear", 0.09f);
        m_Shader->SetUniform1f("pointLights[0].quadratic", 0.032f);
        //// point light 2
        //m_Shader->SetUniform3f("pointLights[1].position", pointLightPositions[1]);
        //m_Shader->SetUniform3f("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        //m_Shader->SetUniform3f("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        //m_Shader->SetUniform3f("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        //m_Shader->SetUniform1f("pointLights[1].constant", 1.0f);
        //m_Shader->SetUniform1f("pointLights[1].linear", 0.09f);
        //m_Shader->SetUniform1f("pointLights[1].quadratic", 0.032f);
        //// point light 3
        //m_Shader->SetUniform3f("pointLights[2].position", pointLightPositions[2]);
        //m_Shader->SetUniform3f("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        //m_Shader->SetUniform3f("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        //m_Shader->SetUniform3f("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        //m_Shader->SetUniform1f("pointLights[2].constant", 1.0f);
        //m_Shader->SetUniform1f("pointLights[2].linear", 0.09f);
        //m_Shader->SetUniform1f("pointLights[2].quadratic", 0.032f);
        //// point light 4
        //m_Shader->SetUniform3f("pointLights[3].position", pointLightPositions[3]);
        //m_Shader->SetUniform3f("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
            //m_Shader->SetUniform3f("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
            //m_Shader->SetUniform3f("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        //m_Shader->SetUniform1f("pointLights[3].constant", 1.0f);
        //m_Shader->SetUniform1f("pointLights[3].linear", 0.09f);
        //m_Shader->SetUniform1f("pointLights[3].quadratic", 0.032f);
        //////////////////////////////////////////////////////
        

        // SETTING THE MATERIAL /////////////////////////////////////////
        Elevate::Material material /*
            { 0.5f, 0.5f, 0.5f }, // Ambient
            { 0.5f, 0.5f, 0.5f }, // Diffuse
            { 0.5f, 0.5f, 0.5f }, // Specular
            1.0f                  // Shine */
        ;
        material.Use(m_Shader);
        /////////////////////////////////////////////////////////////////

        //Elevate::Renderer::SubmitModel(*m_Model); /// WRONG

        //// Bo�te de dialogue pour choisir un fichier
        //const char* filePath = tinyfd_openFileDialog(
        //    "Find a skybox",
        //    "",
        //    0,
        //    NULL,
        //    NULL,
        //    0
        //);

        //tinyfd_messageBox(
        //    "Erreur",
        //    "Une erreur s'est produite lors de l'ex�cution.",
        //    "ok",
        //    "error",
        //    1
        //);

        //SetupImGuiLightStyle();
        SetupImGuiDarkStyle();
    }

    void OnRender() override {
        Elevate::Renderer::SetClearColor({ 1.0f, 0.0f, 1.0f, 1.0f }); // Peut �tre facultatif car on s'en fou un peu au final
        Elevate::Renderer::Clear();

        m_Cubemap->SetProjectionMatrix(cam.GetProjectionMatrix());
        glm::mat4 view = glm::mat4(glm::mat3(cam.GenViewMatrix()));
        m_Cubemap->SetViewMatrix(view);
        m_Cubemap->Draw();

        //Elevate::Renderer::BeginSceneFrame(m_Shader);

        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

        m_Shader->Bind();
        m_Shader->SetUniform3f("camPos", cam.GetPoition()->x, cam.GetPoition()->y, cam.GetPoition()->z);
        m_Shader->SetUniformMatrix4fv("viewProj", cam.GenViewProjectionMatrix());

        // On soumet les models et on les affiches en dessinant la stack
        // TODO -> passer par les commande Renderer:: ... pour faire le rendu � la place
        m_Model->Draw(m_Shader);
        //Elevate::Renderer::DrawStack(m_Shader); // WRONG // TODO redo and use
        //Elevate::Renderer::EndSceneFrame(m_Shader);
    }

    void OnUpdate() override
    {
        // TODO remove at some point cuz useless    
        float mod = 2.0f * Elevate::Time::GetDeltaTime();
        if (Elevate::Input::IsKeyPressed(EE_KEY_LEFT))
        {
            m_Model->GetMatrix() = glm::translate(m_Model->GetMatrix(), glm::vec3(-mod, 0.0f, 0.0f));
        }
        else if (Elevate::Input::IsKeyPressed(EE_KEY_RIGHT))
        {
            m_Model->GetMatrix() = glm::translate(m_Model->GetMatrix(), glm::vec3(mod, 0.0f, 0.0f));
        }
        if (Elevate::Input::IsKeyPressed(EE_KEY_UP))
        {
            m_Model->GetMatrix() = glm::translate(m_Model->GetMatrix(), glm::vec3(0.0f, 0.0f, -mod));
        }
        else if (Elevate::Input::IsKeyPressed(EE_KEY_DOWN))
        {
            m_Model->GetMatrix() = glm::translate(m_Model->GetMatrix(), glm::vec3(0.0f, 0.0f, mod));
        }

        if (Elevate::Input::IsKeyDown(EE_KEY_TAB))
        {
            debugMenuActive = !debugMenuActive;
        }

        // Camera movement ///////////////////////////////
        // sprint
        float baseCamSpeed = 1.0f;
        if (Elevate::Input::IsKeyPressed(EE_KEY_LEFT_SHIFT))
        {
            baseCamSpeed = 2.5f;
        }
        else
        {
            baseCamSpeed = 0.5f;
        }
        float cameraSpeed = baseCamSpeed * Elevate::Time::GetDeltaTime();

        if (Elevate::Input::IsKeyPressed(EE_KEY_W))
            cam.GetTransform()->position += cameraSpeed * cam.m_front;
        if (Elevate::Input::IsKeyPressed(EE_KEY_S))
            cam.GetTransform()->position -= cameraSpeed * cam.m_front;
        if (Elevate::Input::IsKeyPressed(EE_KEY_A))
            cam.GetTransform()->position -= cameraSpeed * cam.m_right;
        if (Elevate::Input::IsKeyPressed(EE_KEY_D))
            cam.GetTransform()->position += cameraSpeed * cam.m_right;

        if (followCursor)
        {
            // calculating the offset
            float xpos = Elevate::Input::GetMouseX();
            float ypos = Elevate::Input::GetMouseY();
            float xoffset = xpos - lastX;
            float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
            // setting the last mouse pos
            lastX = xpos;
            lastY = ypos;
            // adjusting to a sensitivity
            const float sensitivity = 0.5f;
            xoffset *= sensitivity;
            yoffset *= sensitivity;
            // clamp between 89 and -89 deg.
            if (cam.GetRotation()->x > 89.0f)
                cam.GetRotation()->x = 89.0f;
            if (cam.GetRotation()->x < -89.0f)
                cam.GetRotation()->x = -89.0f;


            if (xoffset != 0 && yoffset != 0)
            {
                cam.GetTransform()->rotation.y += xoffset;
                cam.GetTransform()->rotation.x += yoffset;
                cam.UpdateCameraVectors();
            }
        }
    }

    void OnEvent(Elevate::Event& event) override
    {
        if (event.GetEventType() == Elevate::EventType::MouseMoved)
        {
            Elevate::MouseMovedEvent& mouseEvent = dynamic_cast<Elevate::MouseMovedEvent&>(event);
        }

        if (event.IsInCategory(Elevate::EventCategoryMouseButton))
        {
            if (Elevate::Input::IsMouseButtonDown(EE_MOUSE_BUTTON_RIGHT))
            {
                followCursor = true;
                lastX = Elevate::Input::GetMouseX();
                lastY = Elevate::Input::GetMouseY();
            }
            else if (Elevate::Input::IsMouseButtonUp(EE_MOUSE_BUTTON_RIGHT))
            {
                followCursor = false;
            }
        }
    }

    void OnImGuiRender() override
    {
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        if (!debugMenuActive) return;

        // Obtenez la taille de l'�cran / de la fen�tre principale
        ImVec2 mainViewportPos = ImGui::GetMainViewport()->Pos;
        ImVec2 mainViewportSize = ImGui::GetMainViewport()->Size;

        // D�finissez la taille et la position de la fen�tre
        ImVec2 windowSize = ImVec2(mainViewportSize.x / 4, mainViewportSize.y); // 200 est la hauteur de la fen�tre en pixels
        ImVec2 windowPos = ImVec2(mainViewportPos.x, mainViewportPos.y + mainViewportSize.y - windowSize.y);
        // Appliquez la taille et la position avant de cr�er la fen�tre
        ImGui::SetNextWindowSize(windowSize);
        ImGui::SetNextWindowPos(windowPos);
        
        // Cr�ez la fen�tre
        if (ImGui::Begin("Bottom Dock Window", nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar))
        {
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("Debug"))
                {
                    if (ImGui::MenuItem("Hide")) { debugMenuActive = false; }
                    if (ImGui::MenuItem("Exit")) { exit(0); }
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }
            ImGui::Text("Elevate Engine - Debugging");  
            ImGui::Separator();
            ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            
            //ImGui::InputFloat3("Model Position", &m_ModelMatrix[3][0]);
            //ImGui::Text("X: %.2f, Y: %.2f, Z: %.2f", m_ModelMatrix[3].x, m_ModelMatrix[3].y, m_ModelMatrix[3].z);
            ImGui::End();
        }
    }

    void SetupImGuiDarkStyle()
    {
        // Hazy Dark style by kaitabuchi314 from ImThemes
        ImGuiStyle& style = ImGui::GetStyle();

        style.Alpha = 1.0f;
        style.DisabledAlpha = 0.6000000238418579f;
        style.WindowPadding = ImVec2(5.5f, 8.300000190734863f);
        style.WindowRounding = 4.5f;
        style.WindowBorderSize = 1.0f;
        style.WindowMinSize = ImVec2(32.0f, 32.0f);
        style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
        style.WindowMenuButtonPosition = ImGuiDir_Left;
        style.ChildRounding = 3.200000047683716f;
        style.ChildBorderSize = 1.0f;
        style.PopupRounding = 2.700000047683716f;
        style.PopupBorderSize = 1.0f;
        style.FramePadding = ImVec2(4.0f, 3.0f);
        style.FrameRounding = 2.400000095367432f;
        style.FrameBorderSize = 0.0f;
        style.ItemSpacing = ImVec2(8.0f, 4.0f);
        style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
        style.CellPadding = ImVec2(4.0f, 2.0f);
        style.IndentSpacing = 21.0f;
        style.ColumnsMinSpacing = 6.0f;
        style.ScrollbarSize = 14.0f;
        style.ScrollbarRounding = 9.0f;
        style.GrabMinSize = 10.0f;
        style.GrabRounding = 3.200000047683716f;
        style.TabRounding = 3.5f;
        style.TabBorderSize = 1.0f;
        style.TabMinWidthForCloseButton = 0.0f;
        style.ColorButtonPosition = ImGuiDir_Right;
        style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
        style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

        style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.4980392158031464f, 0.4980392158031464f, 0.4980392158031464f, 1.0f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.05882352963089943f, 0.05882352963089943f, 0.05882352963089943f, 0.9399999976158142f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.9399999976158142f);
        style.Colors[ImGuiCol_Border] = ImVec4(0.4274509847164154f, 0.4274509847164154f, 0.4980392158031464f, 0.5f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1372549086809158f, 0.1725490242242813f, 0.2274509817361832f, 0.5400000214576721f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.2117647081613541f, 0.2549019753932953f, 0.3019607961177826f, 0.4000000059604645f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.04313725605607033f, 0.0470588244497776f, 0.0470588244497776f, 0.6700000166893005f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.03921568766236305f, 0.03921568766236305f, 0.03921568766236305f, 1.0f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0784313753247261f, 0.08235294371843338f, 0.09019608050584793f, 1.0f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 0.5099999904632568f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.1372549086809158f, 0.1372549086809158f, 0.1372549086809158f, 1.0f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.01960784383118153f, 0.01960784383118153f, 0.01960784383118153f, 0.5299999713897705f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3098039329051971f, 1.0f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.407843142747879f, 0.407843142747879f, 0.407843142747879f, 1.0f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.5098039507865906f, 0.5098039507865906f, 0.5098039507865906f, 1.0f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.7176470756530762f, 0.7843137383460999f, 0.843137264251709f, 1.0f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.47843137383461f, 0.5254902243614197f, 0.572549045085907f, 1.0f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.2901960909366608f, 0.3176470696926117f, 0.3529411852359772f, 1.0f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.1490196138620377f, 0.1607843190431595f, 0.1764705926179886f, 0.4000000059604645f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1372549086809158f, 0.1450980454683304f, 0.1568627506494522f, 1.0f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.09019608050584793f, 1.0f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.196078434586525f, 0.2156862765550613f, 0.239215686917305f, 0.3100000023841858f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.1647058874368668f, 0.1764705926179886f, 0.1921568661928177f, 0.800000011920929f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.07450980693101883f, 0.08235294371843338f, 0.09019608050584793f, 1.0f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.4274509847164154f, 0.4274509847164154f, 0.4980392158031464f, 0.5f);
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.239215686917305f, 0.3254902064800262f, 0.4235294163227081f, 0.7799999713897705f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.2745098173618317f, 0.3803921639919281f, 0.4980392158031464f, 1.0f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.2901960909366608f, 0.3294117748737335f, 0.3764705955982208f, 0.2000000029802322f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.239215686917305f, 0.2980392277240753f, 0.3686274588108063f, 0.6700000166893005f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.1647058874368668f, 0.1764705926179886f, 0.1882352977991104f, 0.949999988079071f);
        style.Colors[ImGuiCol_Tab] = ImVec4(0.1176470592617989f, 0.125490203499794f, 0.1333333402872086f, 0.8619999885559082f);
        style.Colors[ImGuiCol_TabHovered] = ImVec4(0.3294117748737335f, 0.407843142747879f, 0.501960813999176f, 0.800000011920929f);
        style.Colors[ImGuiCol_TabActive] = ImVec4(0.2431372553110123f, 0.2470588237047195f, 0.2549019753932953f, 1.0f);
        style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.06666667014360428f, 0.1019607856869698f, 0.1450980454683304f, 0.9724000096321106f);
        style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1333333402872086f, 0.2588235437870026f, 0.4235294163227081f, 1.0f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(0.6078431606292725f, 0.6078431606292725f, 0.6078431606292725f, 1.0f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.4274509847164154f, 0.3490196168422699f, 1.0f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8980392217636108f, 0.6980392336845398f, 0.0f, 1.0f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.6000000238418579f, 0.0f, 1.0f);
        style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
        style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
        style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
        style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.3499999940395355f);
        style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.8999999761581421f);
        style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f);
        style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
        style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
        style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);
    }

    void SetupImGuiLightStyle()
    {
        // Light style by dougbinks from ImThemes
        ImGuiStyle& style = ImGui::GetStyle();

        style.Alpha = 1.0f;
        style.DisabledAlpha = 0.6000000238418579f;
        style.WindowPadding = ImVec2(8.0f, 8.0f);
        style.WindowRounding = 0.0f;
        style.WindowBorderSize = 1.0f;
        style.WindowMinSize = ImVec2(32.0f, 32.0f);
        style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
        style.WindowMenuButtonPosition = ImGuiDir_Left;
        style.ChildRounding = 0.0f;
        style.ChildBorderSize = 1.0f;
        style.PopupRounding = 0.0f;
        style.PopupBorderSize = 1.0f;
        style.FramePadding = ImVec2(4.0f, 3.0f);
        style.FrameRounding = 0.0f;
        style.FrameBorderSize = 0.0f;
        style.ItemSpacing = ImVec2(8.0f, 4.0f);
        style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
        style.CellPadding = ImVec2(4.0f, 2.0f);
        style.IndentSpacing = 21.0f;
        style.ColumnsMinSpacing = 6.0f;
        style.ScrollbarSize = 14.0f;
        style.ScrollbarRounding = 9.0f;
        style.GrabMinSize = 10.0f;
        style.GrabRounding = 0.0f;
        style.TabRounding = 4.0f;
        style.TabBorderSize = 0.0f;
        style.TabMinWidthForCloseButton = 0.0f;
        style.ColorButtonPosition = ImGuiDir_Right;
        style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
        style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

        style.Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.6000000238418579f, 0.6000000238418579f, 0.6000000238418579f, 1.0f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.9372549057006836f, 0.9372549057006836f, 0.9372549057006836f, 1.0f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.9800000190734863f);
        style.Colors[ImGuiCol_Border] = ImVec4(0.0f, 0.0f, 0.0f, 0.300000011920929f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.4000000059604645f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.6700000166893005f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.95686274766922f, 0.95686274766922f, 0.95686274766922f, 1.0f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.8196078538894653f, 0.8196078538894653f, 0.8196078538894653f, 1.0f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.0f, 1.0f, 1.0f, 0.5099999904632568f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.8588235378265381f, 0.8588235378265381f, 0.8588235378265381f, 1.0f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.9764705896377563f, 0.9764705896377563f, 0.9764705896377563f, 0.5299999713897705f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.686274528503418f, 0.686274528503418f, 0.686274528503418f, 0.800000011920929f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.4862745106220245f, 0.4862745106220245f, 0.4862745106220245f, 0.800000011920929f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.4862745106220245f, 0.4862745106220245f, 0.4862745106220245f, 1.0f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.7799999713897705f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.4588235318660736f, 0.5372549295425415f, 0.800000011920929f, 0.6000000238418579f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.4000000059604645f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.05882352963089943f, 0.529411792755127f, 0.9764705896377563f, 1.0f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.3100000023841858f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.800000011920929f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.3882353007793427f, 0.3882353007793427f, 0.3882353007793427f, 0.6200000047683716f);
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.1372549086809158f, 0.4392156898975372f, 0.800000011920929f, 0.7799999713897705f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.1372549086809158f, 0.4392156898975372f, 0.800000011920929f, 1.0f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.3490196168422699f, 0.3490196168422699f, 0.3490196168422699f, 0.1700000017881393f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.6700000166893005f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.949999988079071f);
        style.Colors[ImGuiCol_Tab] = ImVec4(0.7607843279838562f, 0.7960784435272217f, 0.8352941274642944f, 0.9309999942779541f);
        style.Colors[ImGuiCol_TabHovered] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.800000011920929f);
        style.Colors[ImGuiCol_TabActive] = ImVec4(0.5921568870544434f, 0.7254902124404907f, 0.8823529481887817f, 1.0f);
        style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.9176470637321472f, 0.9254902005195618f, 0.9333333373069763f, 0.9861999750137329f);
        style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.7411764860153198f, 0.8196078538894653f, 0.9137254953384399f, 1.0f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(0.3882353007793427f, 0.3882353007793427f, 0.3882353007793427f, 1.0f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.4274509847164154f, 0.3490196168422699f, 1.0f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8980392217636108f, 0.6980392336845398f, 0.0f, 1.0f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.4470588266849518f, 0.0f, 1.0f);
        style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.7764706015586853f, 0.8666666746139526f, 0.9764705896377563f, 1.0f);
        style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.5686274766921997f, 0.5686274766921997f, 0.6392157077789307f, 1.0f);
        style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.6784313917160034f, 0.6784313917160034f, 0.7372549176216125f, 1.0f);
        style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.2980392277240753f, 0.2980392277240753f, 0.2980392277240753f, 0.09000000357627869f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.3499999940395355f);
        style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.949999988079071f);
        style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.800000011920929f);
        style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.6980392336845398f, 0.6980392336845398f, 0.6980392336845398f, 0.699999988079071f);
        style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2000000029802322f, 0.2000000029802322f);
        style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2000000029802322f, 0.3499999940395355f);
    }
};



class Sandbox : public Elevate::Application
{
public:
    Sandbox() 
    {
        PushLayer(new DebugLayer());
    }

    ~Sandbox()
    {

    }
};

Elevate::Application* Elevate::CreateApplication() {
    return new Sandbox();
}