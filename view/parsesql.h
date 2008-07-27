
#pragma once

#include <string>
#include <vector>
#include <iostream>

struct sqlite3;

typedef std::vector<std::string> Row;
typedef std::vector<Row> Rows;

std::ostream& operator<<(std::ostream& os, const Rows& rows);

class ParseSQL
{
    public:
        static Rows exec(sqlite3* db, const std::string& sql);
};
