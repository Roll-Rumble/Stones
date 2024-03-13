#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>
#include <string>
#include <chrono>
#include <thread>

#include "ball.hpp"
#include "map.hpp"
#include "shader.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "imgui_util.hpp"

enum class GameState {START, END, DB, PLAY, REPLAY};

         

int main(void)
{
    GameState state = GameState::START;
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1920, 1080, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cerr << "can't init GLEW" << std::endl;
    
    //Rectangle rect1(-0.5f, -0.5f, 1, 1);
    Map map;
    Ball ball(map);
    int num_of_replays = 3;

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    Shader shader("color_shader_vs.txt", "color_shader_fs.txt");
    shader.Use(1.0, 0.0, 0.0, 1.0);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 330";
    ImGui_ImplOpenGL3_Init(glsl_version);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* MENU STATE*/
        if (state == GameState::START) {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
            static bool use_work_area = true;
            static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

            // We demonstrate using the full viewport area or the work area (without menu-bars, task-bars etc.)
            // Based on your use case you may want one or the other.
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
            ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

            if (ImGui::Begin("Example: Fullscreen window", NULL, flags))
            {
                //ShowExampleMenuFile();
                ImGui::SetWindowFontScale(7);
                text_horizontally_center("Roll Rumble");
                ImGui::SetCursorPosY(400);
                ImGui::Text("Roll Rumble");
                ImGui::SetWindowFontScale(3);
                if (button_horizontally_center("Start Game", ImVec2(500, 100))) {
                    state = GameState::PLAY;
                }
                if (button_horizontally_center("Replay", ImVec2(500, 100))) {
                    state = GameState::DB;
                }
                if (button_horizontally_center("test: END", ImVec2(500, 100))) {
                    state = GameState::END;
                }


                /*ImGui::SameLine();
                ImGui::Indent();
                ImGui::Unindent();*/

                //if (p_open && ImGui::Button("Close this window"))
                  //  *p_open = false;
            }
            ImGui::End();

            // Rendering
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
        else if (state == GameState::PLAY) {
            map.draw(shader);
            ball.draw(shader);
        }
        /* REPLAY STATE */
        else if (state == GameState::DB) {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
            static bool use_work_area = true;
            static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

            // We demonstrate using the full viewport area or the work area (without menu-bars, task-bars etc.)
            // Based on your use case you may want one or the other.
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
            ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

            if (ImGui::Begin("Example: Fullscreen window", NULL, flags))
            {
                ImGui::SetWindowFontScale(3);
                text_horizontally_center("Replays");
                ImGui::SetCursorPosY(400);
                ImGui::Text("Replays");

                for (int i = 0; i < num_of_replays; i++) {
                    std::string replay_identifier = "REPLAY: " + std::to_string(i);
                    if (button_horizontally_center(replay_identifier, ImVec2(500, 100))) {
                        state = GameState::REPLAY;
                    }
                }
                if (button_horizontally_center("BACK", ImVec2(500, 100))) {
                    state = GameState::START;
                }
            }
            ImGui::End();

            // Rendering
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
        /* END GUI */
        else if (state == GameState::END) {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
            static bool use_work_area = true;
            static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

            // We demonstrate using the full viewport area or the work area (without menu-bars, task-bars etc.)
            // Based on your use case you may want one or the other.
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
            ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

            if (ImGui::Begin("Example: Fullscreen window", NULL, flags))
            {
                ImGui::SetWindowFontScale(9);
                text_horizontally_center("You have won!");
                ImGui::SetCursorPosY(400);
                ImGui::Text("You have won!");
            }
            ImGui::End();

            // Rendering
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        else if (state == GameState::REPLAY) {
            std::vector<XYPairFloat> replay_ball = {
                XYPairFloat(1,2),
                XYPairFloat(200,2),
                XYPairFloat(302,200),
                XYPairFloat(10,2)
            };
            bool finished = false;
            for (int i = 0; i < replay_ball.size(); i++) {
                glClear(GL_COLOR_BUFFER_BIT);
                map.draw(shader);
                ball.set_position(replay_ball[i]);
                ball.draw(shader);
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                glfwSwapBuffers(window);
                glfwPollEvents();
            }
            std::cout << "completely finished replay\n";
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}