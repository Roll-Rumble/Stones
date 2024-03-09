#include "home_screen.hpp"
#include <iostream>

int main() {
    homeScreen myHomeScreen(800.0f, 600.0f, 0.0f, 1.0f);

    myHomeScreen.draw(); 

    myHomeScreen.select(); 

    return 0;
    
}
