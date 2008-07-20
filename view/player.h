

#pragma once

#include <string>

class Player
{
    public:
        Player( int  key
              , const std::string& name
              , const std::string& pos
              , const std::string& team
              )
            : _key(key)
            , _name(name)
            , _pos(pos)
            , _team(team)
        {
        }

        int _key;
        std::string _name;
        std::string _pos;
        std::string _team;
};
