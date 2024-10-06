#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>

class PlayerStats
{
public:
    std::string name;
    int kills;
    int deaths;
};

class Player
{
public:
    PlayerStats player_stats;
};



/*
a "real-world" example of function pointers and lambdas:
if we want to return a Player that holds a top stat among other players we can write
a function for each stat, for example:
1. "Player get_player_with_top_kills(players)" - for each player in players, store current
player.kills and return the player with highest kills
2. "Player get_player_with_top_deaths(players)" - same thing as (1) but with player.

for example:
*/


Player* get_player_by_top_kills(std::vector<Player>& players)
{
    Player* top_player{ nullptr };
    int top_stat = -1;
    for (Player& player : players)
    {
        const int& stat{ player.player_stats.kills }; //<- Only this changes (kills, deaths, ...)
        if (stat > top_stat)
        {
            top_stat = stat;
            top_player = &player;
        }
    }
    return top_player;
}

/*
The problem:
This technique can lead to a bloated code, since each stat requires its own function.
The "bit" of code that we want to generalize is the field that represents the top stat we look for, e.g:
player.player_stats.kills, or player.player_stats.deaths. This field is queried for each player we
traverse, to check if the current player indeed holds the top stat.

Solution:
Generalize the function in such way that it returns a top player by ANY specific stat.
*************************************************************************************
We need to pass a function that returns a specific field (stat)  per player.
we can do this by passing a lambda (or a normal function) "get_stat(player)" that takes in a Player 
and returns a specific field, each lambda can be different, for example:
1. a lambda that accpets a "Player" and returns player.player_stats.kills
2. a lambda that accpets a "Player" and returns player.player_stats.deaths
Then the function get_player_by_top_stat() will invoke the lambda with "get_stat(player)"
and this will return the field(stat) for which we look the player with the top stat (kills, deaths,...)
*************************************************************************************

*/
Player* get_player_by_top_stat(std::vector<Player>& players, int(*get_stat)(Player&))
{
    Player* top_player{ nullptr };
    int top_stat = -1;
    for (Player& player : players)
    {
        const int& stat{ get_stat(player) };
        if (stat > top_stat)
        {
            top_stat = stat;
            top_player = &player;
        }
    }
    return top_player;
}

/* shorter version of the function above:*/
Player* test(std::vector<Player>& players, int(*get_stat)(Player&))
{
    std::function<bool(Player&, Player&)> fp = [&get_stat](Player& playerA, Player& playerB) -> bool
    {  return get_stat(playerA) > get_stat(playerB); };
    std::vector<Player>::iterator it = std::max_element(players.begin(), players.end(), fp);
    return it._Ptr;
}

int main()
{
    std::vector<Player> players;
    get_player_by_top_stat(players, [](Player& player) {return player.player_stats.kills; });
    get_player_by_top_stat(players, [](Player& player) {return player.player_stats.deaths; });
}