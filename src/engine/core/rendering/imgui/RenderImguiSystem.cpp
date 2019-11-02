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

    ImGui::SFML::Init(m_engine->getWindow());
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::GetIO().FontGlobalScale = 2.f;

    // Setup for first frame
    ImGui::SFML::Update(m_engine->getWindow(), sf::seconds(1.f / 60.f));
    ImGui_ImplOpenGL3_NewFrame();
}

RenderImguiSystem::~RenderImguiSystem() {

    glCheckError();
    ImGui::SFML::Shutdown();
    glCheckError();
}

void RenderImguiSystem::draw() {

    //ImGui::ShowDemoWindow();

    // Render current frame
    glCheckError();
    ImGui::Render();
    glCheckError();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glCheckError();

    // Setup for next frame
    const float dt = static_cast<float>(1.0 / m_engine->getFps());
    ImGui::SFML::Update(m_engine->getWindow(), sf::seconds(dt));
    glCheckError();
    ImGui_ImplOpenGL3_NewFrame();
    glCheckError();
}

void RenderImguiSystem::receive(const sf::Event& event) {

    glCheckError();
    ImGui::SFML::ProcessEvent(event);
    glCheckError();
}
