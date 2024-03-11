#include "replay_screen.hpp"

replayScreen::replayScreen(float x, float y, float width, float height, float r, float g, float b, const std::string& screen_name)
    : menuItem(x, y, width, height, r, g, b, screen_name) {
    
    // Create and add buttons
    float buttonWidth = 100.0f; // example width for a button
    float buttonHeight = 50.0f; // example height for a button
    float startX = (width - buttonWidth) / 2; // center horizontally

    int nb_games; // number of games stored in db 

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

    bool top_pressed = false;
    bool bottom_pressed = false;
    Controller controller;
    std::cout << "Select an option; press Bottom to go down and press Top to enter into the selected button"<< std::endl;
    
    // std::cin >> sel;
    // std::cout << "You selected: " << sel << std::endl;
    
    while (!top_pressed){ //bottom goes down list and top enters current selection
        top_pressed = controller.top_button_pressed();
        bottom_pressed = controller.bottom_button_pressed();

        if (bottom_pressed && !top_pressed){
            sel ++;
            if (sel > 2){
                sel = 0;
            }
        }
        else if (top_pressed && !bottom_pressed){
            break;
        }
        else {
            std::cout << "Invalid selection" << std::endl;
        }
    }

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
    //also output on terminal
    //output all menu option
}

void query_replay(int game_id) { // query the database for replays
    std::cout << "Querying replays" << std::endl;
}

std::vector<std::vector<std::pair<int, int>>> recvReplays() { // receive replays from the database
    std::cout << "Receiving replays" << std::endl;
    std::vector<std::vector<std::pair<int, int>>> replays;
    // need to implement this
    
    return replays;
}

void draw_replay(const std::vector<std::vector<std::pair<int, int>>>& replays) { // draw the replay
    std::cout << "Drawing replays" << std::endl;
    // need to implement this, rendering of the replays

    // later might want to add a pause/unpause or exit button
}