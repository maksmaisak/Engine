//
// Created by Maksym Maisak on 27/10/19.
//

#include "RenderImguiSystem.h"
#include "Engine.h"
#include <imgui-SFML.h>
#include <imgui.h>
#include "imgui_impl_opengl3.h"

using namespace en;

void RenderImguiSystem::start() {

    glCheckError();
    ImGui::SFML::Init(m_engine->getWindow());
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::GetIO().FontGlobalScale = 2.f;
    glCheckError();
}

RenderImguiSystem::~RenderImguiSystem() {

    glCheckError();
    ImGui::SFML::Shutdown();
    //ImGui_ImplOpenGL3_Shutdown();
    glCheckError();
}

void RenderImguiSystem::draw() {

    sf::RenderWindow& window = m_engine->getWindow();

    glCheckError();
    const float dt = static_cast<float>(1.0 / m_engine->getFps());
    ImGui::SFML::Update(window, sf::seconds(0.01f));
    glCheckError();
    ImGui_ImplOpenGL3_NewFrame();
    glCheckError();

    {
        glCheckError();
        ImGui::Begin("Sample window"); // begin window
        glCheckError();

        // Window title text edit
        char windowTitle[255] = "ImGui + SFML = <3";
        ImGui::InputText("Window title", windowTitle, 255);
        if (ImGui::Button("Update window title")) {
            // this code gets if user clicks on the button
            // yes, you could have written if(ImGui::InputText(...))
            // but I do this to show how buttons work :)
            window.setTitle(windowTitle);
        }

        glCheckError();
        ImGui::End();
        glCheckError();
    }

    glCheckError();
    ImGui::Render();
    glCheckError();
    auto* const drawData = ImGui::GetDrawData();
    assert(drawData);
    //drawData->DisplaySize = {drawData->DisplaySize.x * 0.5f, drawData->DisplaySize.y * 0.5f};
    ImGui_ImplOpenGL3_RenderDrawData(drawData);
    //TODO draw calls using data from ImGui::GetDrawData();
    glCheckError();
}

void RenderImguiSystem::receive(const sf::Event& event) {

    glCheckError();
    ImGui::SFML::ProcessEvent(event);
    glCheckError();
}
