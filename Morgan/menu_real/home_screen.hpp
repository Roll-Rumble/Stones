#ifndef HOME_SCREEN_H
#define HOME_SCREEN_H

#include "menu_item.hpp"
#include "button.hpp"


class homeScreen : public menuItem {
public:
    homeScreen(float x, float y, float width, float height);
    void draw() const override;
    void select() override;
};


#endif
