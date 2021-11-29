

#include <QApplication>

#include "mainwindow.h"

int main(int argv, char **args)
{

    QApplication app(argv, args);

    MainWindow mw;
    mw.show();

    return app.exec();
}

