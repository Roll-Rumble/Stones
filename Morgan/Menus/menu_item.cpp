#include "menu_item.hpp"

menuItem::menuItem(float x, float y, float width, float height, float r, float g, float b, const std::string& name_button)
    : x_(x), y_(y), width_(width), height_(height), name_button_(name_button) {
    color[0] = r; color[1] = g; color[2] = b;
};

void menuItem::draw() const {
    // draw the menu item from Constantin
    std::cout << "Drawing menu item" << std::endl;
}

int menuItem::select(){
    // select the menu item
    std::cout << "Selecting menu item" << std::endl;
    // need to implement the select function
    return 0;
}

void menuItem::addItem(std::unique_ptr<menuItem> item) {
    // add a menu item to the menu screen
    items.push_back(std::move(item));
}

