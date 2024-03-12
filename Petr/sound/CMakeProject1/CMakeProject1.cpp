// CMakeProject1.cpp : Defines the entry point for the application.
//

#include "CMakeProject1.h"
#pragma comment(lib, "winmm.lib")

int main()
{
    std::cout << "playing music\n";
    PlaySound(TEXT("C:/Users/Petr/Desktop/2nd_year/mr_crown_hill_rep/sound_effect_repo/CMakeProject1/jazz_music.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    std::string input;
    while (true) {
        std::cin >> input;
        if (input == "stop") {
            break;
        }
        else if (input == "hit") {
            PlaySound(TEXT("C:/Users/Petr/Desktop/2nd_year/mr_crown_hill_rep/sound_effect_repo/CMakeProject1/ball_hit_ball.wav"), NULL, SND_FILENAME | SND_ASYNC);
        }
        else if (input == "wall") {
            PlaySound(TEXT("C:/Users/Petr/Desktop/2nd_year/mr_crown_hill_rep/sound_effect_repo/CMakeProject1/ball_hit_wall.wav"), NULL, SND_FILENAME | SND_ASYNC);
        }
    }
    PlaySound(0, 0, 0);
    std::cout << "Stopped playing music\n";

    return 0;
}
