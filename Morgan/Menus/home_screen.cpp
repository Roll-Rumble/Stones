#include "home_screen.hpp"

homeScreen::homeScreen(float x, float y, float width, float height, float r, float g, float b, const std::string& screen_name)
    : menuItem(x, y, width, height, r, g, b, screen_name) {
    
    // Create and add buttons
    float buttonWidth = 100.0f; // example width
    float buttonHeight = 50.0f; // example height
    float startX = (width - buttonWidth) / 2; // center horizontally
    float startYPlay = y + (height / 4); // position for the play button
    float startYReplay = y + (3 * height / 4); // position for the replay button

    auto playButton = std::make_unique<Button>(startX, startYPlay, buttonWidth, buttonHeight, 0.0f, 0.0f, 0.5f, "Play");  //create new button blue
    auto replayButton = std::make_unique<Button>(startX, startYReplay, buttonWidth, buttonHeight, 0.0f, 0.0f, 0.5f, "Replay"); // create new button blue

    addItem(std::move(playButton)); // add the play button to the menu
    addItem(std::move(replayButton)); // add the replay button to the menu
}

void homeScreen::draw() const {
    items[0]->draw(); // draw the play button
    items[1]->draw(); // draw the replay button
}

int homeScreen::select() {
    bool top_pressed = false;
    bool bottom_pressed = false;
    Controller controller;
    std::cout << "Select an option: " << std::endl << "Pres Top to Play" << std::endl << "Press Bottom to go to Replays" << std::endl;

    top_pressed = controller.top_button_pressed();
    bottom_pressed = controller.bottom_button_pressed();

    if(top_pressed && !bottom_pressed) {
        items[0]->select(); // select the play button
        std::cout << "You selected: Play" << std::endl;
        return 1;
    } else if(bottom_pressed && !top_pressed) {
        items[1]->select(); // select the replay button
        std::cout << "You selected: Replay" << std::endl;
        return 2;
    } else {
        std::cout << "Invalid selection" << std::endl;
        return 0;
    }
}


