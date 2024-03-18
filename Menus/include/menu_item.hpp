#ifndef MENU_ITEM_H
#define MENU_ITEM_H

#include <memory>
#include <vector>
#include <iostream>
#include "read_controller.hpp"

class menuItem { // home screen, play and replay buttons, replay screen, exit replay screen, list of replays are all menu items

public:
    menuItem(float x, float y, float width, float height, float r, float g, float b, const std::string& name_button); // r, g, b are the color values
    virtual void draw() const = 0; // Constantin function to draw buttons
    virtual int select() = 0; // nios II and highlighting the button
    void addItem(std::unique_ptr<menuItem> item); // Adds a menu item to the menu screen and replay screen
    virtual ~menuItem() {}

protected:
    float x_, y_; // Coordinates of button center
    float width_, height_; // Size of button
    float color[3]; // Color (RGB)
    std::string name_button_; // Name of the button, default color should be black for text
    std::vector<std::unique_ptr<menuItem>> items; // Menu items

};


#endif
