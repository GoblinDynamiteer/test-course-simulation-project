#include "Player.h"

/* Constructor */
Player::Player(void)
{
    score = 0;
}

/* Get player score */
int Player::get_score(void)
{
    return score;
}

/* Increase player score */
void Player::increase_score(void)
{
    score++;
}

/* Get player name/initials */
String Player::get_name(void)
{
    return initials;
}

/* Set player name/initials */
void Player::set_name(String s)
{
    initials = s;
}
