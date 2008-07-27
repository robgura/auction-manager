

#pragma once

#include <string>
#include <vector>

struct sqlite3;

typedef std::vector<std::string> Row;
typedef std::vector<Row> Rows;

class ParseSQL
{
    public:
        static Rows exec(sqlite3* db, const std::string& sql);
};
