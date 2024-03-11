#include "home_screen.hpp"
#include "replay_screen.hpp"
#include <iostream>

#define red 1.0f
#define green 1.0f
#define blue 1.0f
#define width 800.0f
#define height 600.0f

int main() {
    int selected;
    // Create a home screen object
    homeScreen myHomeScreen(0.0f, 0.0f, width, height, red, green, blue, "Home");

    // Display the home screen
    myHomeScreen.draw(); 

    // Select an option from the home screen
    selected = myHomeScreen.select();

    if(selected == 2) {
        // Create a replay screen object
        replayScreen myReplayScreen(0.0f, 0.0f, width, height, red, green, blue, "Replay");

        // Display the replay screen
        myReplayScreen.draw();

        // Select an option from the replay screen
        myReplayScreen.select();
    }

    return 0;
}
