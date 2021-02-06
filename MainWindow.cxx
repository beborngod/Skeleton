#include "MainWindow.h"

static int zoom_first_window = 0;
static int zoom_second_window = 0;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    qApp->setOrganizationName("Partisaner");
    qApp->setApplicationName("Skeleton");

    settings = new QSettings(this);

    /* -------Sizes----------- */
    QSize iconActionSize(20, 20);
    setMinimumHeight(395);
    setMinimumWidth(300);

    /* ---------Creating toolbar--------- */
    toolbar = new QToolBar(this);
    toolbar->setOrientation(Qt::Horizontal);
    toolbar->setMovable(false);
    toolbar->setFixedWidth(45);
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
    
    /* ------------Load Icons, Themes and Settings-------- */
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
    connect(settingsAction, &QAction::triggered, this, &MainWindow::goToSettings);
    connect(undoAction, &QAction::triggered, this, &MainWindow::undoText);
    connect(redoAction, &QAction::triggered, this, &MainWindow::redoText);
    connect(pdfAction, &QAction::triggered, this, &MainWindow::saveToPdf);

    /* ---------themes in settings ---------*/
    themesGroupBox = new QGroupBox("        Themes");

    defaultThemeButton = new QRadioButton("Default", this);
    transparentThemeButton = new QRadioButton("Transparent", this);
    blackThemeButton = new QRadioButton("Black", this);
    whiteThemeButton = new QRadioButton("White", this);
    spybotThemeButton = new QRadioButton("SpyBot", this);
    aquaThemeButton = new QRadioButton("Aqua", this);
    macosThemeButton = new QRadioButton("MacOS", this);
    defaultThemeButton->setChecked(true);

    auto labelBetaTheme = new QLabel(this);
    labelBetaTheme->setText("Beta themes:");

    auto vboxLayoutThemes = new QVBoxLayout();
    vboxLayoutThemes->addWidget(defaultThemeButton);
    vboxLayoutThemes->addWidget(transparentThemeButton);
    vboxLayoutThemes->addWidget(blackThemeButton);
    vboxLayoutThemes->addWidget(whiteThemeButton);
    vboxLayoutThemes->addWidget(labelBetaTheme);
    vboxLayoutThemes->addWidget(spybotThemeButton);
    vboxLayoutThemes->addWidget(aquaThemeButton);
    vboxLayoutThemes->addWidget(macosThemeButton);

    vboxLayoutThemes->addSpacing(10);
    themesGroupBox->setLayout(vboxLayoutThemes);

    /* ----------syntax highlighter in settings----------- */
    
    syntaxPartisanerButton = new QRadioButton("Partisaner", this);
    syntaxExpanButton = new QRadioButton("ExPan", this);
    noHighlightButton = new QRadioButton("No highlight", this);
    noHighlightButton->setChecked(true);

    auto vboxLayoutSyntax = new QVBoxLayout();
    vboxLayoutSyntax->addWidget(syntaxPartisanerButton);
    vboxLayoutSyntax->addWidget(syntaxExpanButton);
    vboxLayoutSyntax->addWidget(noHighlightButton);
    vboxLayoutSyntax->addStretch(1);

    auto syntaxGroupBox = new QGroupBox("       Syntax Highlighter for C++:");
    syntaxGroupBox->setLayout(vboxLayoutSyntax);

    auto vboxSetting = new QVBoxLayout();
    vboxSetting->addWidget(themesGroupBox);
    vboxSetting->addWidget(syntaxGroupBox);


    auto settingsGroupBox = new QGroupBox("Settings");
    settingsGroupBox->setLayout(vboxSetting);

    stackedWindows->addWidget(settingsGroupBox);

    /* ----------connection theme changing in settingd----------- */
    connect(defaultThemeButton, &QRadioButton::clicked, this, &MainWindow::themeChanging);
    connect(whiteThemeButton, &QRadioButton::clicked, this, &MainWindow::themeChanging);
    connect(blackThemeButton, &QRadioButton::clicked, this, &MainWindow::themeChanging);
    connect(spybotThemeButton, &QRadioButton::clicked, this, &MainWindow::themeChanging);
    connect(transparentThemeButton, &QRadioButton::clicked, this, &MainWindow::themeChanging);
    connect(aquaThemeButton, &QRadioButton::clicked, this, &MainWindow::themeChanging);
    connect(macosThemeButton, &QRadioButton::clicked, this, &MainWindow::themeChanging);


    /* ----------connection syntax highlighter changing in settingd */
    connect(syntaxPartisanerButton, &QRadioButton::clicked, this, &MainWindow::setSyntaxHighlight);
    connect(syntaxExpanButton, &QRadioButton::clicked, this, &MainWindow::setSyntaxHighlight);
    connect(noHighlightButton, &QRadioButton::clicked, this, &MainWindow::setSyntaxHighlight);

    /* --------Shortcuts-------- */
    QShortcut *settingShortcut = new QShortcut(QKeySequence("Ctrl+,"), this);
    connect(settingShortcut, &QShortcut::activated, this, &MainWindow::goToSettings);

    /* ----------setting Default theme----------- */
    //setTheme("default",WHITE_ICONS);
    
    setSyntaxHighlight();

    /* ---------load settings----------- */
    loadSettings();

    /* ----------Central widget----------- */
    setCentralWidget(stackedWindows);
}

MainWindow::~MainWindow()
{
    saveSettings();
}

void MainWindow::newFile()
{
    focusEdit = static_cast<QTextEdit *>(QApplication::focusWidget());

    focusEdit->clear();

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Save the file?", "Do you want to save the file?", QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::Yes)
    {
        QString c_text = QFileDialog::getSaveFileName();
        QString s_text = c_text.simplified();

        if(not s_text.isEmpty())
        {
            QFile file(s_text);
            file.open(QIODevice::WriteOnly);
            file.write(focusEdit->toPlainText().toUtf8());
            file.close();
        }
    }
}

void MainWindow::openFile()
{
    focusEdit = static_cast<QTextEdit *>(QApplication::focusWidget());

    QString c_text = QFileDialog::getOpenFileName();
    QString s_text = c_text.simplified();
    
    QString tmp;
    for(auto var = s_text.end()-1; *var != "."; --var){
        tmp.push_front(*var);
    }

    if(tmp == "cxx" or tmp == "cpp" or tmp == "hxx" or tmp == "hpp" or tmp == "h"){
        if(not syntaxPartisanerButton->isChecked() and not syntaxExpanButton->isChecked()){
            syntaxPartisanerButton->setChecked(true);   
            setSyntaxHighlight();
        }
    }

    if(not s_text.isEmpty())
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

void MainWindow::saveFile()
{
    focusEdit = static_cast<QTextEdit *>(QApplication::focusWidget());

    QString c_text = QFileDialog::getSaveFileName();
    QString s_text = c_text.simplified();

    if(not s_text.isEmpty())
    {
        QFile file(s_text);
        file.open(QIODevice::WriteOnly);
        file.write(focusEdit->toPlainText().toUtf8());
        file.close();
    }
}

void MainWindow::saveToPdf()
{
    focusEdit = static_cast<QTextEdit *>(QApplication::focusWidget());

    QString c_text = QFileDialog::getSaveFileName();
    QString s_text = c_text.simplified();
    if(not s_text.isEmpty())
    {
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(QString("%1.pdf").arg(s_text));

        focusEdit->document()->print(&printer);
    }
}

void MainWindow::setSyntaxHighlight() 
{
    if(syntaxPartisanerButton->isChecked())
    {
        syntax = new SyntaxPartisaner(mainEdit->document(),
                        Qt::cyan,QColor("#e500f8"),Qt::darkGray,Qt::darkGray,
                        Qt::darkGreen,QColor("#00f8a2"));

        syntax = new SyntaxPartisaner(secondEdit->document(),
                        Qt::cyan,QColor("#e500f8"),Qt::darkGray,Qt::darkGray,
                        Qt::darkGreen,QColor("#00f8a2"));
    }
    else if(syntaxExpanButton->isChecked())
    {
        syntax = new SyntaxPartisaner(mainEdit->document(),
                        QColor("#f73618"),QColor("#fcd900"),Qt::darkGray,Qt::darkGray,
                        QColor("#409ffe"),Qt::cyan);

        syntax = new SyntaxPartisaner(secondEdit->document(),
                        QColor("#f73618"),QColor("#fcd900"),Qt::darkGray,Qt::darkGray,
                        QColor("#409ffe"),Qt::cyan);
    }
    else if(noHighlightButton->isChecked())
    {
        syntax = new SyntaxPartisaner(mainEdit->document());
        syntax = new SyntaxPartisaner(secondEdit->document());
    }
}

QString MainWindow::getTheme() 
{
    QString currentTheme; 

    if(defaultThemeButton->isChecked())
    {
        currentTheme = "default";
    }
    else if(blackThemeButton->isChecked())
    {
        currentTheme = "black";
    }
    else if(whiteThemeButton->isChecked())
    {
        currentTheme = "white";
    }
    else if(spybotThemeButton->isChecked())
    {
        currentTheme = "spybot";
    }
    else if(transparentThemeButton->isChecked())
    {
        currentTheme = "transparent";
    }
    else if(aquaThemeButton->isChecked())
    {
        currentTheme = "aqua";
    }
    else if(macosThemeButton->isChecked())
    {
        currentTheme = "macos";
    }

    return currentTheme;
}

void MainWindow::setTheme(QString theme,int color) 
{
    auto it = themes.find(theme);
    setStyleSheet(it->second);

    if(color == BLACK_ICONS)
        iconChangeToBlack();
    if(color == WHITE_ICONS)
        iconChangeToWhite();
}

int MainWindow::getThemeIcons() 
{
    QString th = getTheme();
    int color;

    if(th == "default" or th == "black" or th == "spybot" or th == "transparent")
        color = 2;
    else
        color = 1;

    return color;
}

void MainWindow::zoomTextIn()
{
    focusEdit = static_cast<QTextEdit *>(QApplication::focusWidget());

    if(focusEdit == mainEdit)
    {
        mainEdit->zoomIn();
        ++zoom_first_window;
    }
    else if(focusEdit == secondEdit)
    {
        secondEdit->zoomIn();
        ++zoom_second_window;
    }
}

void MainWindow::zoomTextOut()
{
    focusEdit = static_cast<QTextEdit *>(QApplication::focusWidget());

    if(focusEdit == mainEdit)
    {
        if(zoom_first_window >= 0)
        {
            mainEdit->zoomOut();
            --zoom_first_window;
        }
    }
    else if(focusEdit == secondEdit)
    {
        if(zoom_second_window >= 0)
        {
            secondEdit->zoomOut();
            --zoom_second_window;
        }
    }
}

void MainWindow::splitDisplay()
{
    if(secondEdit->isHidden())
    {
        secondEdit->show();
    }
    else
        secondEdit->hide();
}

void MainWindow::clear()
{

    focusEdit = static_cast<QTextEdit *>(QApplication::focusWidget());
    
    if(not focusEdit->document()->isEmpty())
    {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this, "Clear the file?", "Do you want to clear the file?", QMessageBox::Yes | QMessageBox::No);

        if(reply == QMessageBox::Yes)
        {
            focusEdit->clear();
        }
    }
}

void MainWindow::goToSettings()
{
    if(stackedWindows->currentIndex() == 0)
    {
        disconnect(clearDisplayAction, &QAction::triggered, this, &MainWindow::clear);
        disconnect(newFileAction, &QAction::triggered, this, &MainWindow::newFile);
        disconnect(openFileAction, &QAction::triggered, this, &MainWindow::openFile);
        disconnect(saveFileAction, &QAction::triggered, this, &MainWindow::saveFile);
        disconnect(zoomInAction, &QAction::triggered, this, &MainWindow::zoomTextIn);
        disconnect(zoomOutAction, &QAction::triggered, this, &MainWindow::zoomTextOut);
        disconnect(splitAction, &QAction::triggered, this, &MainWindow::splitDisplay);
        disconnect(undoAction, &QAction::triggered, this, &MainWindow::undoText);
        disconnect(redoAction, &QAction::triggered, this, &MainWindow::redoText);
        disconnect(pdfAction, &QAction::triggered, this, &MainWindow::saveToPdf);
        
        stackedWindows->setCurrentIndex(1);
    }
    else
    {
        connect(clearDisplayAction, &QAction::triggered, this, &MainWindow::clear);
        connect(newFileAction, &QAction::triggered, this, &MainWindow::newFile);
        connect(openFileAction, &QAction::triggered, this, &MainWindow::openFile);
        connect(saveFileAction, &QAction::triggered, this, &MainWindow::saveFile);
        connect(zoomInAction, &QAction::triggered, this, &MainWindow::zoomTextIn);
        connect(zoomOutAction, &QAction::triggered, this, &MainWindow::zoomTextOut);
        connect(splitAction, &QAction::triggered, this, &MainWindow::splitDisplay);
        connect(undoAction, &QAction::triggered, this, &MainWindow::undoText);
        connect(redoAction, &QAction::triggered, this, &MainWindow::redoText);
        connect(pdfAction, &QAction::triggered, this, &MainWindow::saveToPdf);
        
        stackedWindows->setCurrentIndex(0);
    }
}

void MainWindow::themeChanging()
{
    if(defaultThemeButton->isChecked())
    {
        setTheme("default",WHITE_ICONS);
    }
    else if(blackThemeButton->isChecked())
    {
        setTheme("black",WHITE_ICONS);
    }
    else if(whiteThemeButton->isChecked())
    {
        setTheme("white",BLACK_ICONS);
    }
    else if(spybotThemeButton->isChecked())
    {
        setTheme("spybot",WHITE_ICONS);
    }
    else if(transparentThemeButton->isChecked())
    {
        setTheme("transparent",WHITE_ICONS);
    }
    else if(aquaThemeButton->isChecked())
    {
        setTheme("aqua",BLACK_ICONS);
    }
    else if(macosThemeButton->isChecked())
    {
        setTheme("macos",BLACK_ICONS);
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

void MainWindow::loadSettings() 
{
    setGeometry(settings->value("geometry",QRect(1000,700,1000,500)).toRect());
    setTheme(settings->value("theme").toString(),settings->value("themeIcons").toInt());
}

void MainWindow::saveSettings() 
{
    settings->setValue("geometry",geometry());
    settings->setValue("theme",getTheme());
    settings->setValue("themeIcons",getThemeIcons());
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
        ":/Aqua.qss",
        ":/MacOS.qss",
        ":/transparent.qss"};

    std::vector<QString> themeNames = {
        "default", "black", "white", "spybot", "aqua", "macos", "transparent"};

    for(size_t var = 0; var < styleSheetFilenames.size(); var++)
    {
        styleSheetFile.setFileName(styleSheetFilenames[var]);
        styleSheetFile.open(QFile::ReadOnly);
        themes.emplace(themeNames[var], QLatin1String(styleSheetFile.readAll()));
        styleSheetFile.close();
    }
}