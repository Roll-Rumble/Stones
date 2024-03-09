#include "home_screen.hpp"

homeScreen::homeScreen(float x, float y, float width, float height)
    : menuItem(x, y, width, height, 0.0f, 0.0f, 1.0f) { // Assuming dark blue as the default color for the screen itself, though this might not be necessary
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

void homeScreen::select() {
    int sel;
    std::cout << "Select an option: " << std::endl << "1. Play" << std::endl << "2. Replay" << std::endl;
    std::cin >> sel;
    std::cout << "You selected: " << sel << std::endl;
    if(sel == 1) {
        items[0]->select(); // select the play button
    } else if(sel == 2) {
        items[1]->select(); // select the replay button
    } else {
        std::cout << "Invalid selection" << std::endl;
    }
    
}


