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

    /* -------load black icons for light themes in Tool bar------ */
    newPixBlack.load(":/file.svg");
    openPixBlack.load(":/045-file.svg");
    savePixBlack.load(":/012-edit.svg");
    minusPixBlack.load(":/zoom_out.svg");
    plusPixBlack.load(":/zoom_in.svg");
    clearPixBlack.load(":/017-trash.svg");
    splitPixBlack.load(":/013-layers.svg");
    settingsPixBlack.load(":/050-settings.svg");

    /* -------load White icons for darker themes in Tool bar------ */
    newPixWhite.load(":/file_white.png");
    openPixWhite.load(":/045-file_white.png");
    savePixWhite.load(":/012-edit_white.png");
    minusPixWhite.load(":/zoom_out_white.png");
    plusPixWhite.load(":/zoom_in_white.png");
    clearPixWhite.load(":/017-trash_white.png");
    splitPixWhite.load(":/013-layers_white.png");
    settingsPixWhite.load(":/050-settings_white.png");

    /* -------setting Action for Tool bar----------- */
    newFileAction = ui->toolbar->addAction(QIcon(newPixBlack), "New file");
    openFileAction = ui->toolbar->addAction(QIcon(openPixBlack), "Open file");
    saveFileAction = ui->toolbar->addAction(QIcon(savePixBlack), "Save file");
    zoomInAction = ui->toolbar->addAction(QIcon(plusPixBlack), "Zoom in");
    zoomOutAction = ui->toolbar->addAction(QIcon(minusPixBlack), "Zoom out");
    splitAction = ui->toolbar->addAction(QIcon(splitPixBlack), "Split display");
    clearDisplayAction = ui->toolbar->addAction(QIcon(clearPixBlack), "Clear");

    ui->toolbar->addSeparator();

    settingsAction = ui->toolbar->addAction(QIcon(settingsPixBlack), "Settings");

    /* --------Tool bar Sognal-Slot connection------------*/
    connect(clearDisplayAction, &QAction::triggered, this, &MainWindow::clear);
    connect(newFileAction, &QAction::triggered, this, &MainWindow::newFile);
    connect(openFileAction, &QAction::triggered, this, &MainWindow::openFile);
    connect(saveFileAction, &QAction::triggered, this, &MainWindow::saveFile);
    connect(zoomInAction, &QAction::triggered, this, &MainWindow::zoomTextIn);
    connect(zoomOutAction, &QAction::triggered, this, &MainWindow::zoomTextOut);
    connect(splitAction, &QAction::triggered, this, &MainWindow::splitDisplay);
    connect(settingsAction, &QAction::triggered, this, &MainWindow::settings);

    /* ---------themes in settings */
    themesGroupBox = new QGroupBox("   Theme");

    defaultThemeButton = new QRadioButton("Default", this);
    blackThemeButton = new QRadioButton("Black", this);
    whiteThemeButton = new QRadioButton("White", this);
    spybotThemeButton = new QRadioButton("SpyBot", this);
    obitThemeButton = new QRadioButton("Obit", this);
    defaultThemeButton->setChecked(true);

    auto vboxLayoutThemes = new QVBoxLayout();
    vboxLayoutThemes->addWidget(defaultThemeButton);
    vboxLayoutThemes->addWidget(blackThemeButton);
    vboxLayoutThemes->addWidget(whiteThemeButton);
    vboxLayoutThemes->addWidget(spybotThemeButton);
    vboxLayoutThemes->addWidget(obitThemeButton);

    vboxLayoutThemes->addSpacing(10);
    vboxLayoutThemes->addStretch(1);

    themesGroupBox->setLayout(vboxLayoutThemes);
    stacked_windows->addWidget(themesGroupBox);

    /* ----------connection theme changing in settingd----------- */
    connect(defaultThemeButton,&QRadioButton::clicked,this,&MainWindow::themeChanging);
    connect(whiteThemeButton,&QRadioButton::clicked,this,&MainWindow::themeChanging);
    connect(blackThemeButton,&QRadioButton::clicked,this,&MainWindow::themeChanging);
    connect(spybotThemeButton,&QRadioButton::clicked,this,&MainWindow::themeChanging);
    connect(obitThemeButton,&QRadioButton::clicked,this,&MainWindow::themeChanging);


    /* ----------setting Default theme----------- */
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
    if(stacked_windows->currentIndex() == 0){
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
    if (stacked_windows->currentIndex() == 0){
        stacked_windows->setCurrentIndex(1);
    }else{
        stacked_windows->setCurrentIndex(0);
    }
}

void MainWindow::themeChanging()
{
    if (defaultThemeButton->isChecked())
    {
        QFile styleSheetFile(":/default.qss");
        styleSheetFile.open(QFile::ReadOnly);
        QString styleSheetDefault = QLatin1String(styleSheetFile.readAll());
        setStyleSheet(styleSheetDefault);

        iconChangeToBlack();
    }
    else if (blackThemeButton->isChecked())
    {
        QFile styleSheetFile(":/black.qss");
        styleSheetFile.open(QFile::ReadOnly);
        QString styleSheetBlack = QLatin1String(styleSheetFile.readAll());
        setStyleSheet(styleSheetBlack);

        iconChangeToWhite();
    }
    else if (whiteThemeButton->isChecked())
    {
        QFile styleSheetFile(":/white.qss");
        styleSheetFile.open(QFile::ReadOnly);
        QString styleSheetWhite = QLatin1String(styleSheetFile.readAll());
        setStyleSheet(styleSheetWhite);

        iconChangeToBlack();
    }
    else if (spybotThemeButton->isChecked())
    {
        QFile styleSheetFile(":/SpyBot.qss");
        styleSheetFile.open(QFile::ReadOnly);
        QString styleSheetSpybot = QLatin1String(styleSheetFile.readAll());
        setStyleSheet(styleSheetSpybot);

        iconChangeToWhite();
    }
    else if (obitThemeButton->isChecked())
    {
        QFile styleSheetFile(":/Obit.qss");
        styleSheetFile.open(QFile::ReadOnly);
        QString styleSheetSpybot = QLatin1String(styleSheetFile.readAll());
        setStyleSheet(styleSheetSpybot);

        iconChangeToWhite();
    }
    
}

void MainWindow::iconChangeToBlack()
{
    newFileAction->setIcon(newPixBlack);
    openFileAction->setIcon(openPixBlack);
    saveFileAction->setIcon(savePixBlack);
    zoomInAction->setIcon(plusPixBlack);
    zoomOutAction->setIcon(minusPixBlack);
    splitAction->setIcon(splitPixBlack);
    clearDisplayAction->setIcon(clearPixBlack);
    settingsAction->setIcon(settingsPixBlack);
}

void MainWindow::iconChangeToWhite()
{
    newFileAction->setIcon(newPixWhite);
    openFileAction->setIcon(openPixWhite);
    saveFileAction->setIcon(savePixWhite);
    zoomInAction->setIcon(plusPixWhite);
    zoomOutAction->setIcon(minusPixWhite);
    splitAction->setIcon(splitPixWhite);
    clearDisplayAction->setIcon(clearPixWhite);
    settingsAction->setIcon(settingsPixWhite);
}
