#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Snake/Snake.h"

static int zoom_first_window = 0;
static int zoom_second_window = 0;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    splitter = new QSplitter(Qt::Horizontal,this);
    splitter->setStyleSheet("background-color : rgb(88,88,88);");

    edit = new QTextEdit(this);

    splitter->addWidget(edit);

    setCentralWidget(splitter);


    QPixmap newPix("file.svg");
    QPixmap openPix("045-file.svg");
    QPixmap savePix("012-edit.svg");
    QPixmap minusPix("zoom_out.svg");
    QPixmap plusPix("zoom_in.svg");
    QPixmap clearPix("017-trash.svg");
    QPixmap splitPix("013-layers.svg");
    QPixmap snakePix("026-gamepad.svg");

    auto newFile = ui->toolbar->addAction(QIcon(newPix),"New file");
    auto openFile = ui->toolbar->addAction(QIcon(openPix),"Open file");
    auto saveFile = ui->toolbar->addAction(QIcon(savePix),"Save file");
    auto zoomIn = ui->toolbar->addAction(QIcon(plusPix),"Zoom in");
    auto zoomOut = ui->toolbar->addAction(QIcon(minusPix),"Zoom out");
    auto split = ui->toolbar->addAction(QIcon(splitPix),"Split display");
    auto snakeGame = ui->toolbar->addAction(QIcon(snakePix),"Snake");

    ui->toolbar->addSeparator();

    auto clearDisplay = ui->toolbar->addAction(QIcon(clearPix),"Clear");

    connect(clearDisplay,&QAction::triggered, this, &MainWindow::clear);
    connect(newFile,&QAction::triggered, this, &MainWindow::newFile);
    connect(openFile,&QAction::triggered,this,&MainWindow::openFile);
    connect(saveFile,&QAction::triggered, this, &MainWindow::saveFile);

    connect(zoomIn,&QAction::triggered, this, &MainWindow::zoomTextIn);
    connect(zoomOut,&QAction::triggered, this, &MainWindow::zoomTextOut);

    connect(split,&QAction::triggered, this, &MainWindow::splitDisplay);

    connect(snakeGame,&QAction::triggered,this,&MainWindow::playSnake);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newFile()
{
    focus = static_cast<QTextEdit *>(QApplication::focusWidget());

    focus->clear();

    QMessageBox::StandardButton reply = QMessageBox::question(
    this,"Save the file?","Do you want to save the file?",QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes){
        QString c_text = QFileDialog::getSaveFileName();
        QString s_text = c_text.simplified();

        if(not s_text.isEmpty()){
            QFile file(s_text);
            file.open(QIODevice::WriteOnly);
            file.write(focus->toPlainText().toUtf8());
//            if(edit == focus)
//                file.write(edit->toPlainText().toUtf8());
//            else if(second_edit == focus)
//                file.write(second_edit->toPlainText().toUtf8());
            file.close();
        }
    }
}

void MainWindow::openFile()
{
    //focus = QApplication::focusWidget();

    QString c_text = QFileDialog::getOpenFileName();
    QString s_text = c_text.simplified();

    if(not s_text.isEmpty()){
        QFile file(s_text);
        if (file.open(QIODevice::ReadOnly))
         {
            if(focus == edit){
                edit->clear();
                edit->append(QString(file.readAll()));
            }
            else if(focus == second_edit){
                second_edit->clear();
                second_edit->append(QString(file.readAll()));
            }
        }
        file.close();
    }
}

void MainWindow::saveFile()
{
    //focus = QApplication::focusWidget();

    QString c_text = QFileDialog::getSaveFileName();
    QString s_text = c_text.simplified();

    if(not s_text.isEmpty()){
        QFile file(s_text);
        file.open(QIODevice::WriteOnly);
        if(focus == edit)
            file.write(edit->toPlainText().toUtf8());
        else if(focus == second_edit)
            file.write(second_edit->toPlainText().toUtf8());
        file.close();
    }
}

void MainWindow::zoomTextIn()
{
    //focus = QApplication::focusWidget();

    if(focus == edit){
        edit->zoomIn();
        ++zoom_first_window;
    }else if(focus == second_edit){
        second_edit->zoomIn();
        ++zoom_second_window;
    }
}

void MainWindow::zoomTextOut()
{
    //focus = QApplication::focusWidget();

    if(focus == edit){
        if(zoom_first_window >= 0){
            edit->zoomOut();
            --zoom_first_window;
        }
    }else if(focus == second_edit){
            if(zoom_second_window >= 0){
                second_edit->zoomOut();
                --zoom_second_window;
            }
    }
}

void MainWindow::splitDisplay()
{
    if(splitter->count() == 1){
        second_edit = new QTextEdit();
        splitter->addWidget(second_edit);
    }
    else
        second_edit->deleteLater();

}

void MainWindow::playSnake()
{
    Snake s;
    s.show();
}

void MainWindow::clear()
{
    //focus = QApplication::focusWidget();

    QMessageBox::StandardButton reply = QMessageBox::question(
    this,"Clear the file?","Do you want to clear the file?",QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes){
        if(focus == edit)
            edit->clear();
        else if(focus == second_edit)
            second_edit->clear();
    }
}
