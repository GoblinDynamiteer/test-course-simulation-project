#include <Arduino.h>

#ifndef PLAYER_H
#define PLAYER_H

class Player
{
    public:
        Player();
        int get_score(void);
        void increase_score(void);
        String get_name(void);
        void set_name(String s);

    private:
        int score;
        String initials;
};

#endif
