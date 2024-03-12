#ifndef REPLAY_SCREEN_H
#define REPLAY_SCREEN_H

#include "menu_item.hpp"
#include "button.hpp"
#include "read_controller.hpp"
#include "replay_client_side.hpp"


class replayScreen : public menuItem {

public:
    replayScreen(float x, float y, float width, float height, float r, float g, float b, const std::string& screen_name);
    void draw() const override;
    int select() override;
};

#endif
