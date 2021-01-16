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

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::MainWindow *ui;
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void newFile();
    void openFile();
    void saveFile();
    void zoomTextIn();
    void zoomTextOut();
    void clear();
};
