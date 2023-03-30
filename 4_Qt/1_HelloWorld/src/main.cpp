#include <QApplication>
#include <QPushButton>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.setWindowTitle("My Qt App");
    mainWindow.show();

    return app.exec();
}
