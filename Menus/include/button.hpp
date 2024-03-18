#ifndef BUTTON_H
#define BUTTON_H

#include "menu_item.hpp"
#include <string>
#include "read_controller.hpp"

class Button : public menuItem {

public:
    Button(float x, float y, float width, float height, float r, float g, float b, std::string button_name)
        : menuItem(x, y, width, height, r, g, b, button_name) {}

    void draw() const override;
    int select() override;
};


#endif
