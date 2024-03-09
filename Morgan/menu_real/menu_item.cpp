#include "menu_item.hpp"

menuItem::menuItem(float x, float y, float width, float height, float r, float g, float b)
    : x(x), y(y), width(width), height(height) {
    color[0] = r; color[1] = g; color[2] = b;
};

void menuItem::draw() const {
    // draw the menu item
    std::cout << "Drawing menu item" << std::endl;
}

void menuItem::select(){
    // select the menu item
    std::cout << "Selecting menu item" << std::endl;
}

void menuItem::addItem(std::unique_ptr<menuItem> item) {
    // add a menu item to the menu screen
        items.push_back(std::move(item));
        
}

