
#include "mainwindow.h"
#include "ui_mainform.h"
#include <QPushButton>

MainWindow::MainWindow()
    : _window(new Ui::MainWindow())
{
    _window->setupUi(this);
    //new QPushButton("Button", this);
}
