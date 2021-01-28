#pragma once

#include <QMainWindow>
#include <QApplication>
#include <QTextEdit>
#include <QFile>
#include <QToolBar>
#include <QIcon>
#include <QAction>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QSplitter>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QStackedWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::MainWindow *ui;
    QSplitter *splitter;
    QTextEdit *edit;
    QTextEdit *second_edit;
    QTextEdit *focus_edit;
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected slots:
    void newFile();
    void openFile();
    void saveFile();
    void zoomTextIn();
    void zoomTextOut();
    void splitDisplay();
    void playSnake();
    void clear();
};
