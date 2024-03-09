#ifndef BUTTON_H
#define BUTTON_H

#include "menu_item.hpp"
#include <string>

class Button : public menuItem {
    std::string name;

public:
    Button(float x, float y, float width, float height, float r, float g, float b, std::string name)
        : menuItem(x, y, width, height, r, g, b), name(std::move(name)) {}

    void draw() const override;
    void select() override;
};


#endif 
