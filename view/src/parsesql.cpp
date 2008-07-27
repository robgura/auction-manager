
#include "view/parsesql.h"
#include "sqlite/sqlite3.h"
#include <QDebug>

class Info
{
    public:
        static Rows rows;

        static int handle(void*, int columns, char** columnData, char** columnNames)
        {
            Row row;
            for(int i = 0; i < columns; ++i)
            {
                row.push_back(columnData[i]);
            }
            rows.push_back(row);
            return 0;
        }
};

Rows Info::rows;

Rows ParseSQL::exec(sqlite3* db, const std::string& sql)
{
    Info::rows.clear();

    if(sqlite3_exec(db, sql.c_str(), Info::handle, 0, 0) != SQLITE_OK)
    {
        qDebug() << sqlite3_errmsg(db);
    }

    return Info::rows;
}

std::ostream& operator<<(std::ostream& os, const Rows& rows)
{
    for(Rows::const_iterator iter = rows.begin(); iter != rows.end(); ++iter)
    {
        for(Row::const_iterator jiter = iter->begin(); jiter != iter->end(); ++jiter)
        {
            os << *jiter << ", ";
        }
        os << std::endl;
    }
    return os;
}
