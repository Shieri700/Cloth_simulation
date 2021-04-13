#include <QApplication>
#include <QPushButton>
#include <iostream>
#include <QMainWindow>

#include "GL/glut.h"
#include "mainWindow.h"

using namespace std;

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    OutputDebugStringW(L"My output string.=============");
    QApplication app(argc, argv);
    MainWindow window;
    window.resize(QSize(400, 400));
    // int desktopArea = QApplication::desktop()->width() *
    //                  QApplication::desktop()->height();
    // int widgetArea = window.width() * window.height();

    window.setWindowTitle("Cloth");

    //if (((float)widgetArea / (float)desktopArea) < 0.75f)
    window.show();
    // else
    //     window.showMaximized();
    return app.exec();
}