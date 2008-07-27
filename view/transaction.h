

#pragma once

#include <string>

class Transaction
{
    public:
        Transaction( Owner
             , const std::string& name
             , const std::string& teamName
             )
            : _key(key)
            , _name(name)
            , _teamName(teamName)
        {
        }

        int _key;
        std::string _name;
        std::string _teamName;
};
