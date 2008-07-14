

#pragma once

#include <QMainWindow>

namespace Ui 
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow();

    private:
        Ui::MainWindow* _window;
};
