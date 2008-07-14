

#pragma once

#include <QMainWindow>
#include <QPushButton>

class MainWindow : public QMainWindow
{
    public:
        MainWindow()
        {
            new QPushButton("Button", this);
        }
};
