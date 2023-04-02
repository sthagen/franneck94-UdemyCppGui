#include <iostream>

#include <QPushButton>

#include "mainwindow.h"

MainWindow::MainWindow()
{
    QPushButton *button = new QPushButton("Hello, Qt6!", this);
    setCentralWidget(button);
    connect(button, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
}

void MainWindow::onButtonClicked()
{
    std::cout << "Hello World!\n";
}
