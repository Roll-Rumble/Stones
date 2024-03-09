#ifndef MENU_ITEM_H
#define MENU_ITEM_H

#include <memory>
#include <vector>
#include <iostream>

class menuItem {

public:
    menuItem(float x, float y, float width, float height, float r, float g, float b);
    virtual void draw() const = 0; 
    virtual void select() = 0;
    void addItem(std::unique_ptr<menuItem> item); // Adds a menu item to the menu screen
    virtual ~menuItem() {}

protected:
    float x, y; // Coordinates
    float width, height; // Size
    float color[3]; // Color (RGB)
    std::vector<std::unique_ptr<menuItem>> items; // Menu items

};


#endif
