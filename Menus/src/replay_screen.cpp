#include "replay_screen.hpp"

replayScreen::replayScreen(float x, float y, float width, float height, float r, float g, float b, const std::string& screen_name)
    : menuItem(x, y, width, height, r, g, b, screen_name) {

    // Create and add buttons
    float buttonWidth = 100.0f; // example width for a button
    float buttonHeight = 50.0f; // example height for a button
    float startX = (width - buttonWidth) / 2; // center horizontally

    ReplayClientSide replayClientSide;
    int nb_games = replayClientSide.get_num_games(); // get the number of games from the server
    // TODO: IMPLEMENT CORRECT NUMBER OF BUTTONS

    // for loop for number of games button
    float startYReplay1 = y + (height / 4); // position for the replay1 button
    float startYReplay2 = y + (3 * height / 4); // position for the replay2 button

    float startYExit = y + (5 * height / 4); // position for the exit button, need to redo calculation for position, maybe do 2 columns instead of only 1

    auto replay1Button = std::make_unique<Button>(startX, startYReplay1, buttonWidth, buttonHeight, 0.0f, 0.0f, 0.5f, "Replay 1");  //create new replay button blue
    auto replay2Button = std::make_unique<Button>(startX, startYReplay2, buttonWidth, buttonHeight, 0.0f, 0.0f, 0.5f, "Replay 2"); // create new replay button blue
    auto exitButton = std::make_unique<Button>(startX, startYExit, buttonWidth, buttonHeight, 0.0f, 0.0f, 0.5f, "Exit"); // create new exit button blue

    addItem(std::move(replay1Button)); // add the replay1  button to the replay menu
    addItem(std::move(replay2Button)); // add the replay2 button to the replay menu
    addItem(std::move(exitButton)); // add the exit button to the replay menu
}

void replayScreen::draw() const {
    items[0]->draw(); // draw the replay 1 button
    items[1]->draw(); // draw the replay 2 button
    items[2]->draw(); // draw the exit button
}

int replayScreen::select() {
    int sel = 0;
    bool bottom_pressed_prev = false;
    bool top_pressed = false;
    bool bottom_pressed = false;
    Controller controller;

    ReplayClientSide replayClientSide;
    int nb_games = replayClientSide.get_num_games();

    std::cout << "Select an option; press Bottom to go down and press Top to enter into the selected button" << std::endl;

    while (!top_pressed) {
        controller.read_inputs();
        top_pressed = controller.top_button_pressed();
        bottom_pressed = controller.bottom_button_pressed();

        if ((bottom_pressed && !bottom_pressed_prev) && !top_pressed) {
            sel++;
            std::cout << sel%(nb_games+1) << std::endl;
            if (sel > nb_games + 1) {
                sel = 0;
            }
        }
        else if (top_pressed && !bottom_pressed) {
            break;
        }
        else if (top_pressed && bottom_pressed){
            std::cout << "Invalid selection, both buttons pressed at once" << std::endl;
        }
        bottom_pressed_prev = bottom_pressed;
    }

    std::cout << "You selected: " << sel << std::endl;
    std::vector< std::vector<XYPairInt16> > replay_data;
    replay_data = replayClientSide.get_replay_data(sel); // get the replay data from the server
    draw_replay(replay_data); // draw the replay data

    if(sel == 0) {
        items[0]->select(); // select the replay1 button
    } else if(sel == 1) {
        items[1]->select(); // select the replay2 button
    } else if(sel == 2) {
        std::cout << "Exiting replay screen" << std::endl;
        items[2]->select(); // select the exit button
    } else {
        std::cout << "Invalid selection" << std::endl;
    }
    return sel;
}

