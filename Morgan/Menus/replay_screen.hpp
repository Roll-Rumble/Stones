#ifndef REPLAY_SCREEN_H
#define REPLAY_SCREEN_H

#include "menu_item.hpp"
#include "button.hpp"


class replayScreen : public menuItem {
public:
    replayScreen(float x, float y, float width, float height, float r, float g, float b, const std::string& screen_name);
    void draw() const override;
    int select() override;
    void query_replay(int game_id);
    std::vector<std::vector<std::pair<int, int>>> recv_replays();
    void draw_replay(std::vector<std::vector<std::pair<int, int>>> replays);
};

#endif
