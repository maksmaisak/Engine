//
// Created by Maksym Maisak on 27/10/19.
//

#include "RenderImguiSystem.h"
#include "Engine.h"
#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

using namespace en;

namespace {

    void beginNewFrame() {

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void renderFrame() {

        glCheckError();
        ImGui::Render();
        glCheckError();
        glDisable(GL_DEPTH_TEST);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glEnable(GL_DEPTH_TEST);
        glCheckError();
    }
}

void RenderImguiSystem::start() {

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplGlfw_InitForOpenGL(Engine::get().getWindow().getUnderlyingWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");

    beginNewFrame();
}

RenderImguiSystem::~RenderImguiSystem() {

    glCheckError();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glCheckError();
}

void RenderImguiSystem::draw() {

    //ImGui::ShowDemoWindow();

    renderFrame();
    beginNewFrame();
}