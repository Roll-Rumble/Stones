#ifndef HOME_SCREEN_H
#define HOME_SCREEN_H

#include "menu_item.hpp"
#include "button.hpp"
#include "../../top/client/include/read_controller.hpp"

class homeScreen : public menuItem {

public:
    homeScreen(float x, float y, float width, float height, float r, float g, float b, const std::string& screen_name);
    void draw() const override;
    int select() override;
    int selected;
};

#endif
