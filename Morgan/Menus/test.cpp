#include "home_screen.hpp"
#include "replay_screen.hpp"
#include <iostream>

#define red 1.0f
#define green 1.0f
#define blue 1.0f
#define width 800.0f
#define height 600.0f

int main() {
    int selectedMenu;
    // Create a home screen object
    homeScreen myHomeScreen(0.0f, 0.0f, width, height, red, green, blue, "Home");

    // Display the home screen
    myHomeScreen.draw(); 

    // Select an option from the home screen
    selectedMenu = myHomeScreen.select();

    if(selectedMenu == 2) {
        int selectedReplay = 0; // game id or -1 for exit

        // Instantiate a replay screen object
        replayScreen myReplayScreen(0.0f, 0.0f, width, height, red, green, blue, "Replay");

        // Display the replay screen
        myReplayScreen.draw();

        // Select an option from the replay screen
        selectedReplay = myReplayScreen.select();

        // if not exit, render the replay
        if (selectedReplay != -1) { // might need to wait for reply
            // query the database for replays
            myReplayScreen.query_replay(selectedReplay);
            // receive replays from the database
            std::vector<std::vector<std::pair<int, int>>> replays = myReplayScreen.recv_replays();
            // draw the replays
            myReplayScreen.draw_replay(replays); // rendering
        }
        else {
            std::cout << "Exiting" << std::endl;
        }
    }

    return 0;
}
