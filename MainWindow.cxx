#include "MainWindow.h"

static int zoom_first_window = 0;
static int zoom_second_window = 0;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    /* ---------Creating toolbar--------- */
    toolbar = new QToolBar(this);
    toolbar->setOrientation(Qt::Horizontal);
    toolbar->setMovable(false);
    toolbar->setFixedWidth(45);
    addToolBar(Qt::LeftToolBarArea, toolbar);

    splitter = new QSplitter(Qt::Horizontal, this);
    mainEdit = new QTextEdit(this);
    secondEdit = new QTextEdit(this);

    splitter->addWidget(mainEdit);
    splitter->addWidget(secondEdit);
    secondEdit->hide();

    stackedWindows = new QStackedWidget(this);

    stackedWindows->addWidget(splitter);
    stackedWindows->setCurrentIndex(0);

    /* --Load Icons and Themes */
    loadIcons();
    loadThemes();

    /* -------setting Action for Tool bar----------- */
    newFileAction = toolbar->addAction(QIcon(newPixWhite), "New file");
    openFileAction = toolbar->addAction(QIcon(openPixWhite), "Open file");
    saveFileAction = toolbar->addAction(QIcon(savePixWhite), "Save file");
    zoomInAction = toolbar->addAction(QIcon(plusPixWhite), "Zoom in");
    zoomOutAction = toolbar->addAction(QIcon(minusPixWhite), "Zoom out");
    splitAction = toolbar->addAction(QIcon(splitPixWhite), "Split display");
    undoAction = toolbar->addAction(QIcon(undoPixWhite), "Undo");
    redoAction = toolbar->addAction(QIcon(redoPixWhite), "Redo");
    clearDisplayAction = toolbar->addAction(QIcon(clearPixWhite), "Clear");

    QWidget *emptyPlaceForToolBar = new QWidget();
    emptyPlaceForToolBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    toolbar->addWidget(emptyPlaceForToolBar);

    settingsAction = toolbar->addAction(QIcon(settingsPixWhite), "Settings");

    /* --------Tool bar Sognal-Slot connection------------*/
    connect(clearDisplayAction, &QAction::triggered, this, &MainWindow::clear);
    connect(newFileAction, &QAction::triggered, this, &MainWindow::newFile);
    connect(openFileAction, &QAction::triggered, this, &MainWindow::openFile);
    connect(saveFileAction, &QAction::triggered, this, &MainWindow::saveFile);
    connect(zoomInAction, &QAction::triggered, this, &MainWindow::zoomTextIn);
    connect(zoomOutAction, &QAction::triggered, this, &MainWindow::zoomTextOut);
    connect(splitAction, &QAction::triggered, this, &MainWindow::splitDisplay);
    connect(settingsAction, &QAction::triggered, this, &MainWindow::settings);
    connect(undoAction, &QAction::triggered, this, &MainWindow::undoText);
    connect(redoAction, &QAction::triggered, this, &MainWindow::redoText);

    /* ---------themes in settings ---------*/
    themesGroupBox = new QGroupBox("   Theme");

    defaultThemeButton = new QRadioButton("Default", this);
    blackThemeButton = new QRadioButton("Black", this);
    whiteThemeButton = new QRadioButton("White", this);
    spybotThemeButton = new QRadioButton("SpyBot", this);
    obitThemeButton = new QRadioButton("Obit", this);
    aquaThemeButton = new QRadioButton("Aqua", this);
    macosThemeButton = new QRadioButton("MacOS", this);
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
    vboxLayoutThemes->addWidget(aquaThemeButton);
    vboxLayoutThemes->addWidget(macosThemeButton);

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
    connect(aquaThemeButton, &QRadioButton::clicked, this, &MainWindow::themeChanging);
    connect(macosThemeButton, &QRadioButton::clicked, this, &MainWindow::themeChanging);

    /* --------Shortcuts-------- */
    QShortcut *settingShortcut = new QShortcut(QKeySequence("Ctrl+,"), this);
    connect(settingShortcut, &QShortcut::activated,this,&MainWindow::settings);

        /* ----------setting Default theme----------- */
        auto it = themes.find("default");
    setStyleSheet(it->second);

    /* ----------Central widget----------- */
    setCentralWidget(stackedWindows);
}

MainWindow::~MainWindow() {}

void MainWindow::newFile()
{
    if (stackedWindows->currentIndex() == 0)
    {
        focusEdit = static_cast<QTextEdit *>(QApplication::focusWidget());

        focusEdit->clear();

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
                file.write(focusEdit->toPlainText().toUtf8());
                file.close();
            }
        }
    }
}

void MainWindow::openFile()
{
    if (stackedWindows->currentIndex() == 0)
    {
        focusEdit = static_cast<QTextEdit *>(QApplication::focusWidget());

        QString c_text = QFileDialog::getOpenFileName();
        QString s_text = c_text.simplified();

        if (not s_text.isEmpty())
        {
            QFile file(s_text);
            if (file.open(QIODevice::ReadOnly))
            {
                focusEdit->clear();
                focusEdit->append(QString(file.readAll()));
            }
            file.close();
        }
    }
}

void MainWindow::saveFile()
{
    if (stackedWindows->currentIndex() == 0)
    {
        focusEdit = static_cast<QTextEdit *>(QApplication::focusWidget());

        QString c_text = QFileDialog::getSaveFileName();
        QString s_text = c_text.simplified();

        if (not s_text.isEmpty())
        {
            QFile file(s_text);
            file.open(QIODevice::WriteOnly);
            file.write(focusEdit->toPlainText().toUtf8());
            file.close();
        }
    }
}

void MainWindow::zoomTextIn()
{
    if (stackedWindows->currentIndex() == 0)
    {
        focusEdit = static_cast<QTextEdit *>(QApplication::focusWidget());

        if (focusEdit == mainEdit)
        {
            mainEdit->zoomIn();
            ++zoom_first_window;
        }
        else if (focusEdit == secondEdit)
        {
            secondEdit->zoomIn();
            ++zoom_second_window;
        }
    }
}

void MainWindow::zoomTextOut()
{
    if (stackedWindows->currentIndex() == 0)
    {
        focusEdit = static_cast<QTextEdit *>(QApplication::focusWidget());

        if (focusEdit == mainEdit)
        {
            if (zoom_first_window >= 0)
            {
                mainEdit->zoomOut();
                --zoom_first_window;
            }
        }
        else if (focusEdit == secondEdit)
        {
            if (zoom_second_window >= 0)
            {
                secondEdit->zoomOut();
                --zoom_second_window;
            }
        }
    }
}

void MainWindow::splitDisplay()
{
    if (stackedWindows->currentIndex() == 0)
    {
        if (secondEdit->isHidden())
        {
            secondEdit->show();
        }
        else
            secondEdit->hide();
    }
}

void MainWindow::clear()
{
    if (stackedWindows->currentIndex() == 0)
    {
        focusEdit = static_cast<QTextEdit *>(QApplication::focusWidget());

        QMessageBox::StandardButton reply = QMessageBox::question(
            this, "Clear the file?", "Do you want to clear the file?", QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes)
        {
            focusEdit->clear();
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
        iconChangeToWhite();
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
    else if (aquaThemeButton->isChecked())
    {
        auto it = themes.find("aqua");
        setStyleSheet(it->second);
        iconChangeToBlack();
    }
    else if (macosThemeButton->isChecked())
    {
        auto it = themes.find("macos");
        setStyleSheet(it->second);
        iconChangeToBlack();
    }
}

void MainWindow::undoText()
{
    focusEdit = static_cast<QTextEdit *>(QApplication::focusWidget());
    focusEdit->undo();
}

void MainWindow::redoText()
{
    focusEdit = static_cast<QTextEdit *>(QApplication::focusWidget());
    focusEdit->redo();
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
    undoAction->setIcon(undoPixBlack);
    redoAction->setIcon(redoPixBlack);
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
    undoAction->setIcon(undoPixWhite);
    redoAction->setIcon(redoPixWhite);
}

void MainWindow::loadThemes()
{
    QFile styleSheetFile;
    std::vector<QString> styleSheetFilenames = {
        ":/default.qss",
        ":/black.qss",
        ":/white.qss",
        ":/SpyBot.qss",
        ":/Obit.qss",
        ":/Aqua.qss",
        ":/MacOS.qss"};

    std::vector<QString> themeNames = {
        "default", "black", "white", "spybot", "obit", "aqua", "macos"};

    for (size_t var = 0; var < styleSheetFilenames.size(); var++)
    {
        styleSheetFile.setFileName(styleSheetFilenames[var]);
        styleSheetFile.open(QFile::ReadOnly);
        themes.emplace(themeNames[var], QLatin1String(styleSheetFile.readAll()));
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
    undoPixBlack.load(":/035-return.png");
    redoPixBlack.load(":/035-return_reversed.png");

    /* -------load White icons for darker themes in Tool bar------ */
    newPixWhite.load(":/file_white.png");
    openPixWhite.load(":/045-file_white.png");
    savePixWhite.load(":/012-edit_white.png");
    minusPixWhite.load(":/zoom_out_white.png");
    plusPixWhite.load(":/zoom_in_white.png");
    clearPixWhite.load(":/017-trash_white.png");
    splitPixWhite.load(":/013-layers_white.png");
    settingsPixWhite.load(":/050-settings_white.png");
    undoPixWhite.load(":/035-return_white.png");
    redoPixWhite.load(":/035-return_reversed_white.png");
}
