#include <chrono>
#include <cstdint>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "ball.hpp"
#include "client_tcp.hpp"
#include "client_udp.hpp"
#include "game_util.hpp"
#include "map.hpp"
#include "read_controller.hpp"
#include "shader.hpp"
#include "texture.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "imgui_util.hpp"

enum class GameState { START, END, DB, PLAY, REPLAY };

#define WIN_CODE 2048
#define LOSE_CODE 4096

int main() {

    int replay_selected=0;
    int number_of_replays=0;

    // Instantiate controller object for reading input
    Controller nios2;

    // Start separate read thread
    std::thread controller_read_thread(&Controller::read_loop, &nios2);

    // Initialize network communication sockets
    TCPClient tcpClient;
    UDPClient udpClient(tcpClient.get_connection_nb());

    // Create window for rendering
    GLFWwindow* window;

    // Initialize the library
    if (!glfwInit()) {
        exit(1);
    }

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Roll Rumble", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(1);
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        exit(1);
    }

    // Load shaders
    Shader shader("color_shader_vs.txt", "color_shader_fs.txt");
    Texture floor_tex("floor_texture.png");
    Texture wall_tex("wall_texture_2.png");
    Texture hole_tex("hole_texture.png");
    Texture enemy_ball_tex("enemy_ball.png");
    Texture my_ball_tex("my_ball.png");
    Texture exit_tex("exit.png");
    Texture entrance_tex("start_texture.png");

    floor_tex.Bind(FLOOR_SLOT);
    wall_tex.Bind(WALL_SLOT);
    hole_tex.Bind(HOLE_SLOT);
    my_ball_tex.Bind(MY_BALL_SLOT);
    enemy_ball_tex.Bind(ENEMY_BALL_SLOT);
    entrance_tex.Bind(ENTRANCE_SLOT);
    exit_tex.Bind(EXIT_SLOT);

    shader.Bind();

    // Instantiate game objects
    Map map;
    Ball my_ball(map, MY_BALL_SLOT);
    Ball op_ball(map, ENEMY_BALL_SLOT);

    std::pair<float,float> xy_pos = {0,0};

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

    GameState state = GameState::START;
    GameState prev_state = GameState::START;

    bool won = false;

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        auto start = std::chrono::steady_clock::now(); // Start counting

        glClear(GL_COLOR_BUFFER_BIT);

        if (state == GameState::PLAY) {
            // Read accelerometer data from Nios II
            XYPairInt16 xy_tilt_input = nios2.get_xy_accel();

            // Calculate ball-wall physics
            my_ball.set_velocity(normalize_accel(xy_tilt_input));
            my_ball.update_position();
            my_ball.resolve_wall_collisions(map);
            my_ball.resolve_hole_fall(map);

            // Send data from controller over UDP
            XYPairFloat ball_position = my_ball.get_position();
            udpClient.send_xy((int16_t)ball_position.x, (int16_t)ball_position.y);

            // // Receive position data over UDP
            // xy_pos = udpClient.receive_xy(xy_pos);
            // ball.set_position({xy_pos.first, xy_pos.second});

            std::pair<float, float> op_ball_pos =
                udpClient.receive_xy({ op_ball.get_position().x, op_ball.get_position().y });
            op_ball.set_position({ op_ball_pos.first, op_ball_pos.second });

            if (op_ball_pos.first == WIN_CODE) {
                // Go to win screen
                won = true;
                state = GameState::END;
            } else if (op_ball_pos.first == LOSE_CODE) {
                // Go to lose screen
                won = false;
                state = GameState::END;
            }

            // Render frame
            map.draw(shader);
            my_ball.draw(shader);
            op_ball.draw(shader);

            auto end = std::chrono::steady_clock::now(); // End counting
            // Calculate the duration in milliseconds
            auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

            // Currently locked at 20fps
            while (elapsed.count() < 50000) { //wait until min amount has passed
                end = std::chrono::steady_clock::now();
                elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            }
        }
        else if (state == GameState::START) {
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
                    prev_state = state;
                    state = GameState::DB;
                }
                if (button_horizontally_center("Exit", ImVec2(500, 100))) {
                    break;
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
        } else if(state == GameState::DB) {
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

                if (prev_state != state) {
                    number_of_replays = tcpClient.get_nb_games();
                    std::cout << "number of replays received " << number_of_replays << std::endl;
                    prev_state = state;
                }
                for (int i = 0; i < number_of_replays; i++) {
                    std::string replay_identifier = "REPLAY: " + std::to_string(i);
                    if (button_horizontally_center(replay_identifier, ImVec2(500, 100))) {
                        replay_selected = i;
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
                ImGui::SetCursorPosY(400);
                if (won) {
                    text_horizontally_center("You have won!");
                    ImGui::Text("You have won!");
                } else {
                    text_horizontally_center("You have lost!");
                    ImGui::Text("You have lost!");
                }

                if (button_horizontally_center("Restart Game", ImVec2(500, 100))) {
                    state = GameState::PLAY;
                }
                if (button_horizontally_center("Replay", ImVec2(500, 100))) {
                    prev_state = state;
                    state = GameState::DB;
                }
                if (button_horizontally_center("Exit", ImVec2(500, 100))) {
                    break;
                }

            }
            ImGui::End();

            // Rendering
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
        else if (state == GameState::REPLAY) {
            std::cout << "before requesting replay data" << std::endl;
            std::vector<std::vector<XYPairFloat>> replay_data = tcpClient.get_game_data(replay_selected);
            std::cout << "received replay data" << std::endl;
            Ball me(map, MY_BALL_SLOT);
            Ball them(map, ENEMY_BALL_SLOT);
            int my_id = tcpClient.get_connection_nb();
            bool finished = false;
            for (int i = 0; i < replay_data.size(); i++) {
                glClear(GL_COLOR_BUFFER_BIT);
                map.draw(shader);
                me.set_position(replay_data[i][my_id]);
                them.set_position(replay_data[i][!my_id]);
                me.draw(shader);
                them.draw(shader);
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                glfwSwapBuffers(window);
                glfwPollEvents();
            }
        }
        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();

    }

    glfwTerminate();

    // This will stop the program from ever terminating right now
    controller_read_thread.join();
}
