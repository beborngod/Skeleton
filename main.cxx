#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.setWindowTitle("Skeleton");
    window.resize(1000,700);
    window.setWindowIcon(QIcon(":/human-skull.svg"));

    /*QFile styleSheetFile("NAME_OF.qss");
    styleSheetFile.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(styleSheetFile.readAll());
    app.setStyleSheet(styleSheet);*/

    window.show();
    return app.exec();
}
