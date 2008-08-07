

#pragma once

#include <QObject>

class Shouter : public QObject
{
    Q_OBJECT

    public:
        static Shouter* instance();

        void trans();

    signals:
        void transaction();

    private:
        Shouter();
        Shouter(const Shouter& right);

        static Shouter* _shouter;
};
