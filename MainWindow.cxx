#include "MainWindow.h"
#include "ui_MainWindow.h"

static int zoom_first_window = 0;
static int zoom_second_window = 0;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    splitter = new QSplitter(Qt::Horizontal, this);

    edit = new QTextEdit(this);

    splitter->addWidget(edit);

    stacked_windows = new QStackedWidget(this);

    stacked_windows->addWidget(splitter);
    stacked_windows->setCurrentIndex(0);

    setCentralWidget(stacked_windows);

    QPixmap newPix(":/file.svg");
    QPixmap openPix(":/045-file.svg");
    QPixmap savePix(":/012-edit.svg");
    QPixmap minusPix(":/zoom_out.svg");
    QPixmap plusPix(":/zoom_in.svg");
    QPixmap clearPix(":/017-trash.svg");
    QPixmap splitPix(":/013-layers.svg");
    QPixmap settingsPix(":/050-settings.svg");

    newFileAction = ui->toolbar->addAction(QIcon(newPix), "New file");
    openFileAction = ui->toolbar->addAction(QIcon(openPix), "Open file");
    saveFileAction = ui->toolbar->addAction(QIcon(savePix), "Save file");
    zoomInAction = ui->toolbar->addAction(QIcon(plusPix), "Zoom in");
    zoomOutAction = ui->toolbar->addAction(QIcon(minusPix), "Zoom out");
    splitAction = ui->toolbar->addAction(QIcon(splitPix), "Split display");
    clearDisplayAction = ui->toolbar->addAction(QIcon(clearPix), "Clear");

    ui->toolbar->addSeparator();

    settingsAction = ui->toolbar->addAction(QIcon(settingsPix), "Settings");

    connect(clearDisplayAction, &QAction::triggered, this, &MainWindow::clear);
    connect(newFileAction, &QAction::triggered, this, &MainWindow::newFile);
    connect(openFileAction, &QAction::triggered, this, &MainWindow::openFile);
    connect(saveFileAction, &QAction::triggered, this, &MainWindow::saveFile);

    connect(zoomInAction, &QAction::triggered, this, &MainWindow::zoomTextIn);
    connect(zoomOutAction, &QAction::triggered, this, &MainWindow::zoomTextOut);

    connect(splitAction, &QAction::triggered, this, &MainWindow::splitDisplay);

    connect(settingsAction, &QAction::triggered, this, &MainWindow::settings);

    edit_color_gb = new QGroupBox("   Theme");

    default_rb = new QRadioButton("Default", this);
    black_rb = new QRadioButton("Black", this);
    white_rb = new QRadioButton("White", this);
    spy_bot_rb = new QRadioButton("SpyBot", this);
    default_rb->setChecked(true);

    auto vbox_layout_gb = new QVBoxLayout();
    vbox_layout_gb->addWidget(default_rb);
    vbox_layout_gb->addWidget(black_rb);
    vbox_layout_gb->addWidget(white_rb);
    vbox_layout_gb->addWidget(spy_bot_rb);

    vbox_layout_gb->addSpacing(10);
    vbox_layout_gb->addStretch(1);

    edit_color_gb->setLayout(vbox_layout_gb);
    stacked_windows->addWidget(edit_color_gb);


    connect(default_rb,&QRadioButton::clicked,this,&MainWindow::themeChanging);
    connect(white_rb,&QRadioButton::clicked,this,&MainWindow::themeChanging);
    connect(black_rb,&QRadioButton::clicked,this,&MainWindow::themeChanging);
    connect(spy_bot_rb,&QRadioButton::clicked,this,&MainWindow::themeChanging);

    QFile styleSheetFile(":/default.qss");
    styleSheetFile.open(QFile::ReadOnly);
    QString styleSheetDefault = QLatin1String(styleSheetFile.readAll());
    setStyleSheet(styleSheetDefault);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newFile()
{
    focus_edit = static_cast<QTextEdit *>(QApplication::focusWidget());

    focus_edit->clear();

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Save the file?", "Do you want to save the file?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        QString c_text = QFileDialog::getSaveFileName();
        QString s_text = c_text.simplified();

        if (not s_text.isEmpty())
        {
            QFile file(s_text);
            file.open(QIODevice::WriteOnly);
            file.write(focus_edit->toPlainText().toUtf8());
            file.close();
        }
    }
}

void MainWindow::openFile()
{
    focus_edit = static_cast<QTextEdit *>(QApplication::focusWidget());

    QString c_text = QFileDialog::getOpenFileName();
    QString s_text = c_text.simplified();

    if (not s_text.isEmpty())
    {
        QFile file(s_text);
        if (file.open(QIODevice::ReadOnly))
        {
            focus_edit->clear();
            focus_edit->append(QString(file.readAll()));
        }
        file.close();
    }
}

void MainWindow::saveFile()
{
    focus_edit = static_cast<QTextEdit *>(QApplication::focusWidget());

    QString c_text = QFileDialog::getSaveFileName();
    QString s_text = c_text.simplified();

    if (not s_text.isEmpty())
    {
        QFile file(s_text);
        file.open(QIODevice::WriteOnly);
        file.write(focus_edit->toPlainText().toUtf8());
        file.close();
    }
}

void MainWindow::zoomTextIn()
{
    focus_edit = static_cast<QTextEdit *>(QApplication::focusWidget());

    if (focus_edit == edit)
    {
        edit->zoomIn();
        ++zoom_first_window;
    }
    else if (focus_edit == second_edit)
    {
        second_edit->zoomIn();
        ++zoom_second_window;
    }
}

void MainWindow::zoomTextOut()
{
    focus_edit = static_cast<QTextEdit *>(QApplication::focusWidget());

    if (focus_edit == edit)
    {
        if (zoom_first_window >= 0)
        {
            edit->zoomOut();
            --zoom_first_window;
        }
    }
    else if (focus_edit == second_edit)
    {
        if (zoom_second_window >= 0)
        {
            second_edit->zoomOut();
            --zoom_second_window;
        }
    }
}

void MainWindow::splitDisplay()
{
    if (splitter->count() == 1)
    {
        second_edit = new QTextEdit();
        splitter->addWidget(second_edit);
    }
    else
        second_edit->deleteLater();
}

void MainWindow::clear()
{
    focus_edit = static_cast<QTextEdit *>(QApplication::focusWidget());

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Clear the file?", "Do you want to clear the file?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        focus_edit->clear();
    }
}

void MainWindow::settings()
{
    if (stacked_windows->currentIndex() == 0)
        stacked_windows->setCurrentIndex(1);
    else
        stacked_windows->setCurrentIndex(0);
}

void MainWindow::themeChanging()
{
    if (default_rb->isChecked())
    {
        QFile styleSheetFile(":/default.qss");
        styleSheetFile.open(QFile::ReadOnly);
        QString styleSheetDefault = QLatin1String(styleSheetFile.readAll());
        setStyleSheet(styleSheetDefault);
    }
    else if (black_rb->isChecked())
    {
        QFile styleSheetFile(":/black.qss");
        styleSheetFile.open(QFile::ReadOnly);
        QString styleSheetBlack = QLatin1String(styleSheetFile.readAll());
        setStyleSheet(styleSheetBlack);
    }
    else if (white_rb->isChecked())
    {
        QFile styleSheetFile(":/white.qss");
        styleSheetFile.open(QFile::ReadOnly);
        QString styleSheetWhite = QLatin1String(styleSheetFile.readAll());
        setStyleSheet(styleSheetWhite);
    }
    else if (spy_bot_rb->isChecked())
    {
        QFile styleSheetFile(":/SpyBot.qss");
        styleSheetFile.open(QFile::ReadOnly);
        QString styleSheetSpybot = QLatin1String(styleSheetFile.readAll());
        setStyleSheet(styleSheetSpybot);
    }
    
}
