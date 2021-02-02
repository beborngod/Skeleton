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

    stackedWindows = new QStackedWidget(this);

    stackedWindows->addWidget(splitter);
    stackedWindows->setCurrentIndex(0);

    setCentralWidget(stackedWindows);

    /* --Load Icons and Themes */
    loadIcons();
    loadThemes();

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

    auto labelBetaTheme = new QLabel(this);
    labelBetaTheme->setText("Beta:");

    auto vboxLayoutThemes = new QVBoxLayout();
    vboxLayoutThemes->addWidget(defaultThemeButton);
    vboxLayoutThemes->addWidget(blackThemeButton);
    vboxLayoutThemes->addWidget(whiteThemeButton);
    vboxLayoutThemes->addWidget(labelBetaTheme);
    vboxLayoutThemes->addWidget(spybotThemeButton);
    vboxLayoutThemes->addWidget(obitThemeButton);

    vboxLayoutThemes->addSpacing(10);
    vboxLayoutThemes->addStretch(1);

    themesGroupBox->setLayout(vboxLayoutThemes);
    stackedWindows->addWidget(themesGroupBox);

    /* ----------connection theme changing in settingd----------- */
    connect(defaultThemeButton, &QRadioButton::clicked, this, &MainWindow::themeChanging);
    connect(whiteThemeButton, &QRadioButton::clicked, this, &MainWindow::themeChanging);
    connect(blackThemeButton, &QRadioButton::clicked, this, &MainWindow::themeChanging);
    connect(spybotThemeButton, &QRadioButton::clicked, this, &MainWindow::themeChanging);
    connect(obitThemeButton, &QRadioButton::clicked, this, &MainWindow::themeChanging);

    /* ----------setting Default theme----------- */
    auto it = themes.find("default");
    setStyleSheet(it->second);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newFile()
{
    if (stackedWindows->currentIndex() == 0)
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
}

void MainWindow::openFile()
{
    if (stackedWindows->currentIndex() == 0)
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
}

void MainWindow::saveFile()
{
    if (stackedWindows->currentIndex() == 0)
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
}

void MainWindow::zoomTextIn()
{
    if (stackedWindows->currentIndex() == 0)
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
}

void MainWindow::zoomTextOut()
{
    if (stackedWindows->currentIndex() == 0)
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
}

void MainWindow::splitDisplay()
{
    if (stackedWindows->currentIndex() == 0)
    {
        if (splitter->count() == 1)
        {
            second_edit = new QTextEdit();
            splitter->addWidget(second_edit);
        }
        else
            second_edit->deleteLater();
    }
}

void MainWindow::clear()
{
    if (stackedWindows->currentIndex() == 0)
    {
        focus_edit = static_cast<QTextEdit *>(QApplication::focusWidget());

        QMessageBox::StandardButton reply = QMessageBox::question(
            this, "Clear the file?", "Do you want to clear the file?", QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes)
        {
            focus_edit->clear();
        }
    }
}

void MainWindow::settings()
{
    if (stackedWindows->currentIndex() == 0)
    {
        stackedWindows->setCurrentIndex(1);
    }
    else
    {
        stackedWindows->setCurrentIndex(0);
    }
}

void MainWindow::themeChanging()
{
    if (defaultThemeButton->isChecked())
    {
        auto it = themes.find("default");
        setStyleSheet(it->second);
        iconChangeToBlack();
    }
    else if (blackThemeButton->isChecked())
    {
        auto it = themes.find("black");
        setStyleSheet(it->second);
        iconChangeToWhite();
    }
    else if (whiteThemeButton->isChecked())
    {
        auto it = themes.find("white");
        setStyleSheet(it->second);
        iconChangeToBlack();
    }
    else if (spybotThemeButton->isChecked())
    {
        auto it = themes.find("spybot");
        setStyleSheet(it->second);
        iconChangeToWhite();
    }
    else if (obitThemeButton->isChecked())
    {
        auto it = themes.find("obit");
        setStyleSheet(it->second);
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

/* void MainWindow::loadThemes()
{
    QFile styleSheetFile(":/default.qss");
    styleSheetFile.open(QFile::ReadOnly);
    styleSheetDefault = QLatin1String(styleSheetFile.readAll());
    styleSheetFile.close();

    styleSheetFile.setFileName(":/black.qss");
    styleSheetFile.open(QFile::ReadOnly);
    styleSheetBlack = QLatin1String(styleSheetFile.readAll());
    styleSheetFile.close();

    styleSheetFile.setFileName(":/white.qss");
    styleSheetFile.open(QFile::ReadOnly);
    styleSheetWhite = QLatin1String(styleSheetFile.readAll());
    styleSheetFile.close();

    styleSheetFile.setFileName(":/SpyBot.qss");
    styleSheetFile.open(QFile::ReadOnly);
    styleSheetSpybot = QLatin1String(styleSheetFile.readAll());
    styleSheetFile.close();

    styleSheetFile.setFileName(":/Obit.qss");
    styleSheetFile.open(QFile::ReadOnly);
    styleSheetObit = QLatin1String(styleSheetFile.readAll());
    styleSheetFile.close();
} */

void MainWindow::loadThemes()
{
    QFile styleSheetFile;
    std::vector<QString> styleSheetFilenames = {
        ":/default.qss",
        ":/black.qss",
        ":/white.qss",
        ":/SpyBot.qss",
        ":/Obit.qss"};

    std::vector<QString> themeNames = {"default","black","white","spybot","obit"};

    for (size_t var = 0; var < styleSheetFilenames.size(); var++)
    {
        styleSheetFile.setFileName(styleSheetFilenames[var]);
        styleSheetFile.open(QFile::ReadOnly);
        themes.emplace(themeNames[var],QLatin1String(styleSheetFile.readAll()));
        styleSheetFile.close();
    }
    
}

void MainWindow::loadIcons()
{
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
}
