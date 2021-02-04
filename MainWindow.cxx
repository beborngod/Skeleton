#include "MainWindow.h"

static int zoom_first_window = 0;
static int zoom_second_window = 0;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    QSize iconActionSize(20, 20);
    /* ---------Creating toolbar--------- */
    toolbar = new QToolBar(this);
    toolbar->setOrientation(Qt::Horizontal);
    toolbar->setMovable(false);
    toolbar->setFixedWidth(50);
    toolbar->setIconSize(iconActionSize);
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
    newFileAction = toolbar->addAction("New file");
    openFileAction = toolbar->addAction("Open file");
    saveFileAction = toolbar->addAction("Save file");
    pdfAction = toolbar->addAction("PDF");
    zoomInAction = toolbar->addAction("Zoom in");
    zoomOutAction = toolbar->addAction("Zoom out");
    splitAction = toolbar->addAction("Split display");
    undoAction = toolbar->addAction("Undo");
    redoAction = toolbar->addAction("Redo");
    clearDisplayAction = toolbar->addAction("Clear");

    QWidget *emptyPlaceForToolBar = new QWidget();
    emptyPlaceForToolBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    toolbar->addWidget(emptyPlaceForToolBar);

    settingsAction = toolbar->addAction("Settings");

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
    connect(pdfAction, &QAction::triggered, this, &MainWindow::saveToPdf);

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
    connect(settingShortcut, &QShortcut::activated, this, &MainWindow::settings);

    /* ----------setting Default theme----------- */
    auto it = themes.find("default");
    setStyleSheet(it->second);
    iconChangeToWhite();

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

void MainWindow::saveToPdf()
{
    if (stackedWindows->currentIndex() == 0)
    {
        focusEdit = static_cast<QTextEdit *>(QApplication::focusWidget());

        QString c_text = QFileDialog::getSaveFileName();
        QString s_text = c_text.simplified();
        if (not s_text.isEmpty())
        {
            QPrinter printer(QPrinter::HighResolution);
            printer.setOutputFormat(QPrinter::PdfFormat);
            printer.setOutputFileName(QString("%1.pdf").arg(s_text));

            focusEdit->document()->print(&printer);
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
        
        if (not focusEdit->document()->isEmpty())
        {
            QMessageBox::StandardButton reply = QMessageBox::question(
                this, "Clear the file?", "Do you want to clear the file?", QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::Yes)
            {
                focusEdit->clear();
            }
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

void MainWindow::loadIcons()
{
    /* -------load black icons for light themes in Tool bar------ */
    newPix.first.load(":/file.svg");
    openPix.first.load(":/045-file.svg");
    savePix.first.load(":/012-edit.svg");
    minusPix.first.load(":/zoom_out.svg");
    plusPix.first.load(":/zoom_in.svg");
    clearPix.first.load(":/017-trash.svg");
    splitPix.first.load(":/013-layers.svg");
    settingsPix.first.load(":/050-settings.svg");
    undoPix.first.load(":/035-return.png");
    redoPix.first.load(":/035-return_reversed.png");
    pdfPix.first.load(":/document.svg");

    /* -------load White icons for darker themes in Tool bar------ */
    newPix.second.load(":/file_white.png");
    openPix.second.load(":/045-file_white.png");
    savePix.second.load(":/012-edit_white.png");
    minusPix.second.load(":/zoom_out_white.png");
    plusPix.second.load(":/zoom_in_white.png");
    clearPix.second.load(":/017-trash_white.png");
    splitPix.second.load(":/013-layers_white.png");
    settingsPix.second.load(":/050-settings_white.png");
    undoPix.second.load(":/035-return_white.png");
    redoPix.second.load(":/035-return_reversed_white.png");
    pdfPix.second.load(":/document_white.png");
}

void MainWindow::iconChangeToBlack()
{
    newFileAction->setIcon(newPix.first);
    openFileAction->setIcon(openPix.first);
    saveFileAction->setIcon(savePix.first);
    zoomInAction->setIcon(plusPix.first);
    zoomOutAction->setIcon(minusPix.first);
    splitAction->setIcon(splitPix.first);
    clearDisplayAction->setIcon(clearPix.first);
    settingsAction->setIcon(settingsPix.first);
    undoAction->setIcon(undoPix.first);
    redoAction->setIcon(redoPix.first);
    pdfAction->setIcon(pdfPix.first);
}

void MainWindow::iconChangeToWhite()
{
    newFileAction->setIcon(newPix.second);
    openFileAction->setIcon(openPix.second);
    saveFileAction->setIcon(savePix.second);
    zoomInAction->setIcon(plusPix.second);
    zoomOutAction->setIcon(minusPix.second);
    splitAction->setIcon(splitPix.second);
    clearDisplayAction->setIcon(clearPix.second);
    settingsAction->setIcon(settingsPix.second);
    undoAction->setIcon(undoPix.second);
    redoAction->setIcon(redoPix.second);
    pdfAction->setIcon(pdfPix.second);
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
