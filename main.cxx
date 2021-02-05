#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.setWindowTitle("Skeleton");
    window.resize(1000,700);
    window.setWindowIcon(QIcon(":/main_icon.svg"));

    window.show();
    return app.exec();
}
