#include "Skeleton.h"

static int zoomFirstWindow = 0;
static int zoomSecondWindow = 0;

Skeleton::Skeleton(QWidget *parent) : QMainWindow(parent)
{
    editFont.setFamily("Source Code Pro");

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

    firstEdit = new QTextEdit(this);
    firstEdit->setTabStopDistance(30);

    secondEdit = new QTextEdit(this);
    secondEdit->setTabStopDistance(30);

    firstEdit->setFont(editFont);
    secondEdit->setFont(editFont);

    firstEditList.push_back(firstEdit);
    secondEditList.push_back(secondEdit);

    fileNameFirstEdit.emplace(firstEdit, "Untilited");
    fileNameSecondEdit.emplace(secondEdit, "Untilited");

    /* -------Tabs--------- */
    firstTab = new QTabWidget(this);
    firstTab->setTabsClosable(true);
    firstTab->setMovable(true);
    firstTab->addTab(firstEdit, "Untilited");

    secondTab = new QTabWidget(this);
    secondTab->setTabsClosable(true);
    secondTab->setMovable(true);
    secondTab->addTab(secondEdit, "Untilited");

    connect(firstTab, &QTabWidget::tabCloseRequested, this, &Skeleton::closeFirstTab);
    connect(secondTab, &QTabWidget::tabCloseRequested, this, &Skeleton::closeSecondTab);

    splitter->addWidget(firstTab);
    splitter->addWidget(secondTab);
    secondTab->hide();

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
    connect(clearDisplayAction, &QAction::triggered, this, &Skeleton::clear);
    connect(newFileAction, &QAction::triggered, this, &Skeleton::newFile);
    connect(openFileAction, &QAction::triggered, this, &Skeleton::openFile);
    connect(saveFileAction, &QAction::triggered, this, &Skeleton::saveFile);
    connect(zoomInAction, &QAction::triggered, this, &Skeleton::zoomTextIn);
    connect(zoomOutAction, &QAction::triggered, this, &Skeleton::zoomTextOut);
    connect(splitAction, &QAction::triggered, this, &Skeleton::splitDisplay);
    connect(settingsAction, &QAction::triggered, this, &Skeleton::goToSettings);
    connect(undoAction, &QAction::triggered, this, &Skeleton::undoText);
    connect(redoAction, &QAction::triggered, this, &Skeleton::redoText);
    connect(pdfAction, &QAction::triggered, this, &Skeleton::saveToPdf);

    /* -----------Settings Panel----------- */
    settingsPanel();

    /* --------Shortcuts-------- */
    shortcuts();

    /* ---------load settings----------- */
    loadSettings();

    /* ----------Central widget----------- */
    setCentralWidget(stackedWindows);
}

Skeleton::~Skeleton()
{
    saveSettings();
}

void Skeleton::doNewFile(QTabWidget *focusTab, QList<QTextEdit *> &editList, std::multimap<QTextEdit *, QString> &fileNameMap)
{
    auto newEdit = new QTextEdit(this);

    newEdit->setFont(editFont);

    focusTab->addTab(newEdit, "Untilited");

    focusTab->setCurrentWidget(newEdit);

    editList.push_back(newEdit);

    newEdit->setFocus();

    fileNameMap.emplace(newEdit, "Untilited");
}

void Skeleton::newFile()
{
    focusEdit = static_cast<QTextEdit *>(QApplication::focusWidget());

    if (firstEditList.count() == 0)
    {
        doNewFile(firstTab, firstEditList, fileNameFirstEdit);

        firstTab->show();
    }
    else
    {
        if (firstTab->currentWidget() == focusEdit)
        {
            doNewFile(firstTab, firstEditList, fileNameFirstEdit);
        }
        else if (secondTab->currentWidget() == focusEdit)
        {
            doNewFile(secondTab, secondEditList, fileNameSecondEdit);
        }
    }
}

void Skeleton::openFile()
{
    focusEdit = static_cast<QTextEdit *>(QApplication::focusWidget());

    QString gotFilePath = QFileDialog::getOpenFileName();
    QString filePath = gotFilePath.simplified();

    QString fileName;
    for (auto var = filePath.end() - 1; *var != "/"; --var)
    {
        fileName.push_front(*var);
    }

    QTextEdit *newEdit;
    if (not filePath.isEmpty())
    {
        newEdit = new QTextEdit(this);
        QFile file(filePath);

        if (file.open(QIODevice::ReadOnly))
        {
            if (firstTab->currentWidget() == focusEdit)
            {
                firstTab->addTab(newEdit, fileName);

                firstTab->setCurrentWidget(newEdit);

                firstEditList.push_back(newEdit);

                fileNameFirstEdit.emplace(newEdit, filePath);
            }
            else if (secondTab->currentWidget() == focusEdit)
            {
                secondTab->addTab(newEdit, fileName);

                secondTab->setCurrentWidget(newEdit);

                secondEditList.push_back(newEdit);

                fileNameSecondEdit.emplace(newEdit, filePath);
            }

            newEdit->append(QString(file.readAll()));
            newEdit->setFont(editFont);
        }
        file.close();
    }
}

void Skeleton::saveFile()
{
    focusEdit = static_cast<QTextEdit *>(QApplication::focusWidget());

    QString gotFilePath = QFileDialog::getSaveFileName();
    QString filePath = gotFilePath.simplified();

    if (not filePath.isEmpty())
    {
        QFile file(filePath);
        file.open(QIODevice::WriteOnly);
        file.write(focusEdit->toPlainText().toUtf8());
        file.close();
    }

    /* -------------getting file name-------------- */
    QString fileName;
    for (auto var = filePath.end() - 1; *var != "/"; --var)
    {
        fileName.push_front(*var);
    }

    if (fileName.count() < 100)
    {
        if (firstTab->currentWidget() == focusEdit)
        {
            int index = firstTab->indexOf(focusEdit);

            firstTab->setTabText(index, fileName);

            auto it = fileNameFirstEdit.find(focusEdit);
            it->second = filePath;
        }
        else if (secondTab->currentWidget() == focusEdit)
        {
            int index = secondTab->indexOf(focusEdit);

            secondTab->setTabText(index, fileName);

            auto it = fileNameSecondEdit.find(focusEdit);
            it->second = filePath;
        }
    }
}

void Skeleton::saveToPdf()
{
    if (firstEditList.count() > 0 or firstEditList.count() > 0)
    {
        focusEdit = static_cast<QTextEdit *>(QApplication::focusWidget());

        QString gotFilePath = QFileDialog::getSaveFileName();
        QString filePath = gotFilePath.simplified();
        if (not filePath.isEmpty())
        {
            QPrinter printer(QPrinter::HighResolution);
            printer.setOutputFormat(QPrinter::PdfFormat);
            printer.setOutputFileName(QString("%1.pdf").arg(filePath));

            focusEdit->document()->print(&printer);
        }
    }
}

void Skeleton::setFontEdit()
{
    focusEdit = static_cast<QTextEdit *>(QApplication::focusWidget());

    bool OK;
    int comma = 0;

    QString str;
    str.reserve(30);

    editFont = QFontDialog::getFont(&OK);

    if (OK)
    {
        for (auto &editItem : firstEditList)
            editItem->setFont(editFont);
        for (auto &editItem : secondEditList)
            editItem->setFont(editFont);
    }
    for (auto it = editFont.toString().begin(); comma != 2; ++it)
    {
        if (*it == ",")
            ++comma;
        if (comma != 2)
            str.append(*it);
    }
    zoomFirstWindow = editFont.pointSize();
    zoomSecondWindow = editFont.pointSize();
    fontLineEdit->setText(str);
}

void Skeleton::closeFirstTab(int index)
{
    if (firstEditList.count() != 0)
    {
        firstTab->removeTab(index);
        firstEditList.at(index)->deleteLater();
        firstEditList.removeAt(index);
        firstEditList.at(--index)->setFocus();
    }
    else
    {
        firstTab->hide();
    }
}

void Skeleton::closeSecondTab(int index)
{
    if (secondEditList.count() != 0)
    {
        secondTab->removeTab(index);
        secondEditList.at(index)->deleteLater();
        secondEditList.removeAt(index);
        secondEditList.at(--index)->setFocus();
    }
    else
    {
        secondTab->hide();
    }
}

QString Skeleton::getTheme()
{
    QString currentTheme;

    if (defaultThemeButton->isChecked())
    {
        currentTheme = "default";
    }
    else if (blackThemeButton->isChecked())
    {
        currentTheme = "black";
    }
    else if (whiteThemeButton->isChecked())
    {
        currentTheme = "white";
    }
    else if (spybotThemeButton->isChecked())
    {
        currentTheme = "spybot";
    }
    else if (transparentThemeButton->isChecked())
    {
        currentTheme = "transparent";
    }
    else if (bubbleThemeButton->isChecked())
    {
        currentTheme = "bubble";
    }

    return currentTheme;
}

void Skeleton::setTheme(QString theme, int color)
{
    auto it = themes.find(theme);
    setStyleSheet(it->second);

    if (color == BLACK_ICONS)
        iconChangeToBlack();
    if (color == WHITE_ICONS)
        iconChangeToWhite();
}

int Skeleton::getThemeIcons()
{
    QString th = getTheme();
    int color;

    if (th == "default" or th == "black" or th == "spybot" or th == "transparent")
        color = 2;
    else
        color = 1;

    return color;
}

void Skeleton::setRadionButtonChecked(QString radioButton)
{
    if (radioButton == "default")
        defaultThemeButton->setChecked(true);
    else if (radioButton == "black")
        blackThemeButton->setChecked(true);
    else if (radioButton == "white")
        whiteThemeButton->setChecked(true);
    else if (radioButton == "transparent")
        transparentThemeButton->setChecked(true);
    else if (radioButton == "spybot")
        spybotThemeButton->setChecked(true);
    else if (radioButton == "bubble")
        bubbleThemeButton->setChecked(true);
}

void Skeleton::zoomTextIn()
{
    focusEdit = static_cast<QTextEdit *>(QApplication::focusWidget());

    focusEdit->zoomIn();
}

void Skeleton::zoomTextOut()
{
    focusEdit = static_cast<QTextEdit *>(QApplication::focusWidget());

    focusEdit->zoomOut();
}

void Skeleton::splitDisplay()
{
    if (firstTab->isHidden())
    {
        if (firstEditList.count() == 0)
        {
            doNewFile(firstTab, firstEditList, fileNameFirstEdit);

            firstTab->show();
            firstTab->currentWidget()->setFocus();
        }
        else
        {
            firstTab->show();
        }
    }
    else if (secondTab->isHidden())
    {
        if (secondEditList.count() != 0)
        {
            secondTab->show();
            secondTab->currentWidget()->setFocus();
        }
        else if (secondEditList.count() == 0)
        {
            doNewFile(secondTab, secondEditList, fileNameSecondEdit);
        }
    }
    else if (focusEdit = static_cast<QTextEdit *>(QApplication::focusWidget()), firstTab->currentWidget() == focusEdit)
    {
        secondTab->hide();
    }
    else if (focusEdit = static_cast<QTextEdit *>(QApplication::focusWidget()), secondTab->currentWidget() == focusEdit)
    {
        firstTab->hide();
    }
}

void Skeleton::clear()
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

void Skeleton::goToSettings()
{
    if (stackedWindows->currentIndex() == 0)
    {
        disconnect(clearDisplayAction, &QAction::triggered, this, &Skeleton::clear);
        disconnect(newFileAction, &QAction::triggered, this, &Skeleton::newFile);
        disconnect(openFileAction, &QAction::triggered, this, &Skeleton::openFile);
        disconnect(saveFileAction, &QAction::triggered, this, &Skeleton::saveFile);
        disconnect(zoomInAction, &QAction::triggered, this, &Skeleton::zoomTextIn);
        disconnect(zoomOutAction, &QAction::triggered, this, &Skeleton::zoomTextOut);
        disconnect(splitAction, &QAction::triggered, this, &Skeleton::splitDisplay);
        disconnect(undoAction, &QAction::triggered, this, &Skeleton::undoText);
        disconnect(redoAction, &QAction::triggered, this, &Skeleton::redoText);
        disconnect(pdfAction, &QAction::triggered, this, &Skeleton::saveToPdf);

        stackedWindows->setCurrentIndex(1);
    }
    else
    {
        connect(clearDisplayAction, &QAction::triggered, this, &Skeleton::clear);
        connect(newFileAction, &QAction::triggered, this, &Skeleton::newFile);
        connect(openFileAction, &QAction::triggered, this, &Skeleton::openFile);
        connect(saveFileAction, &QAction::triggered, this, &Skeleton::saveFile);
        connect(zoomInAction, &QAction::triggered, this, &Skeleton::zoomTextIn);
        connect(zoomOutAction, &QAction::triggered, this, &Skeleton::zoomTextOut);
        connect(splitAction, &QAction::triggered, this, &Skeleton::splitDisplay);
        connect(undoAction, &QAction::triggered, this, &Skeleton::undoText);
        connect(redoAction, &QAction::triggered, this, &Skeleton::redoText);
        connect(pdfAction, &QAction::triggered, this, &Skeleton::saveToPdf);

        stackedWindows->setCurrentIndex(0);
    }
}

void Skeleton::themeChanging()
{
    if (defaultThemeButton->isChecked())
    {
        setTheme("default", WHITE_ICONS);
    }
    else if (blackThemeButton->isChecked())
    {
        setTheme("black", WHITE_ICONS);
    }
    else if (whiteThemeButton->isChecked())
    {
        setTheme("white", BLACK_ICONS);
    }
    else if (spybotThemeButton->isChecked())
    {
        setTheme("spybot", WHITE_ICONS);
    }
    else if (transparentThemeButton->isChecked())
    {
        setTheme("transparent", WHITE_ICONS);
    }
    else if (bubbleThemeButton->isChecked())
    {
        setTheme("bubble", BLACK_ICONS);
    }
}

void Skeleton::undoText()
{
    focusEdit = static_cast<QTextEdit *>(QApplication::focusWidget());
    focusEdit->undo();
}

void Skeleton::redoText()
{
    focusEdit = static_cast<QTextEdit *>(QApplication::focusWidget());
    focusEdit->redo();
}

void Skeleton::loadIcons()
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

/* ---------------------------------------------------------------------------------- */

void Skeleton::loadSettings()
{
    setGeometry(settings->value("geometry", QRect(1000, 700, 1000, 500)).toRect());

    if (not settings->value("theme").toString().isEmpty())
    {
        setTheme(settings->value("theme").toString(), settings->value("themeIcons").toInt());
        setRadionButtonChecked(settings->value("theme").toString());
    }
    else
    {
        setTheme();
        setRadionButtonChecked("default");
    }

    QFont font(settings->value("font").toString());
    zoomFirstWindow = font.pointSize();
    zoomSecondWindow = font.pointSize();
    firstEdit->setFont(font);
}

void Skeleton::saveSettings()
{
    settings->setValue("geometry", geometry());
    settings->setValue("theme", getTheme());
    settings->setValue("themeIcons", getThemeIcons());
    settings->setValue("font", editFont.toString());
}

void Skeleton::settingsPanel()
{
    /* ---------themes in settings ---------*/
    themesGroupBox = new QGroupBox("\t\tThemes:");

    defaultThemeButton = new QRadioButton("Default", this);
    transparentThemeButton = new QRadioButton("Transparent", this);
    blackThemeButton = new QRadioButton("Black", this);
    whiteThemeButton = new QRadioButton("White", this);
    spybotThemeButton = new QRadioButton("SpyBot", this);
    bubbleThemeButton = new QRadioButton("Bubble", this);
    defaultThemeButton->setChecked(true);

    /* ------collect theme buttons-------- */
    auto vboxLayoutThemes = new QVBoxLayout();
    vboxLayoutThemes->addWidget(defaultThemeButton);
    vboxLayoutThemes->addWidget(blackThemeButton);
    vboxLayoutThemes->addWidget(whiteThemeButton);
    vboxLayoutThemes->addWidget(spybotThemeButton);
    vboxLayoutThemes->addWidget(bubbleThemeButton);

    auto kvantumLabel = new QLabel("Able only with Kvantum theme:");
    vboxLayoutThemes->addWidget(kvantumLabel);
    vboxLayoutThemes->addWidget(transparentThemeButton);

    vboxLayoutThemes->addSpacing(10);
    themesGroupBox->setLayout(vboxLayoutThemes);

    /* ---------Font settings------------------ */
    auto fontGroupBox = new QGroupBox("\t\tFont");
    auto hboxFontLayout = new QHBoxLayout();
    fontLineEdit = new QLineEdit();
    fontLineEdit->setReadOnly(true);
    fontLineEdit->setMaximumWidth(300);

    setFontButton = new QPushButton("set");
    setFontButton->setFixedWidth(70);
    connect(setFontButton, &QPushButton::pressed, this, &Skeleton::setFontEdit);

    hboxFontLayout->addWidget(fontLineEdit);
    hboxFontLayout->addWidget(setFontButton);
    hboxFontLayout->addStretch(1);

    fontGroupBox->setLayout(hboxFontLayout);
    /* ------------Settings layout----------- */
    auto vboxSetting = new QVBoxLayout();
    vboxSetting->addWidget(themesGroupBox);

    vboxSetting->addWidget(fontGroupBox);
    vboxSetting->addStretch(1);

    auto settingsGroupBox = new QGroupBox("Settings");
    settingsGroupBox->setLayout(vboxSetting);

    stackedWindows->addWidget(settingsGroupBox);

    /* ----------connection theme changing in settingd----------- */
    connect(defaultThemeButton, &QRadioButton::clicked, this, &Skeleton::themeChanging);
    connect(whiteThemeButton, &QRadioButton::clicked, this, &Skeleton::themeChanging);
    connect(blackThemeButton, &QRadioButton::clicked, this, &Skeleton::themeChanging);
    connect(spybotThemeButton, &QRadioButton::clicked, this, &Skeleton::themeChanging);
    connect(transparentThemeButton, &QRadioButton::clicked, this, &Skeleton::themeChanging);
    connect(bubbleThemeButton, &QRadioButton::clicked, this, &Skeleton::themeChanging);
}

void Skeleton::shortcuts()
{
    /* ------------------Shotscuts---------------- */
    QShortcut *setting = new QShortcut(QKeySequence("Ctrl+,"), this);

    QShortcut *switchLeftTab = new QShortcut(QKeySequence("Alt+Left"), this);

    QShortcut *switchRightTab = new QShortcut(QKeySequence("Alt+Right"), this);

    QShortcut *zoomInStc = new QShortcut(QKeySequence("Ctrl+="), this);

    QShortcut *zoomOutStc = new QShortcut(QKeySequence("Ctrl+-"), this);

    QShortcut *splitStc = new QShortcut(QKeySequence("Ctrl+\\"), this);

    QShortcut *openFileStc = new QShortcut(QKeySequence("Ctrl+o"), this);

    QShortcut *saveFileStc = new QShortcut(QKeySequence("Ctrl+Shift+s"), this);

    QShortcut *saveStc = new QShortcut(QKeySequence("Ctrl+s"), this);

    QShortcut *saveFileToPdfStc = new QShortcut(QKeySequence("Ctrl+p"), this);

    QShortcut *clearFileStc = new QShortcut(QKeySequence("Ctrl+r"), this);

    QShortcut *newFileStc = new QShortcut(QKeySequence("Ctrl+n"), this);

    QShortcut *rightTabSplitter = new QShortcut(QKeySequence("Ctrl+Alt+Right"), this);

    QShortcut *leftTabSplitter = new QShortcut(QKeySequence("Ctrl+Alt+Left"), this);

    QShortcut *closeTabStc = new QShortcut(QKeySequence("Ctrl+Shift+q"), this);

    QShortcut *moveTab = new QShortcut(QKeySequence("Ctrl+Shift+m"), this);

    /* -----------------------Connection-------------------------- */
    connect(setting, &QShortcut::activated, this, &Skeleton::goToSettings);
    connect(zoomInStc, &QShortcut::activated, this, &Skeleton::zoomTextIn);
    connect(zoomOutStc, &QShortcut::activated, this, &Skeleton::zoomTextOut);
    connect(splitStc, &QShortcut::activated, this, &Skeleton::splitDisplay);
    connect(openFileStc, &QShortcut::activated, this, &Skeleton::openFile);
    connect(saveFileStc, &QShortcut::activated, this, &Skeleton::saveFile);
    connect(saveFileToPdfStc, &QShortcut::activated, this, &Skeleton::saveToPdf);
    connect(clearFileStc, &QShortcut::activated, this, &Skeleton::clear);
    connect(clearFileStc, &QShortcut::activated, this, &Skeleton::clear);
    connect(newFileStc, &QShortcut::activated, this, &Skeleton::newFile);

    connect(switchLeftTab, &QShortcut::activated, this, [&]() {
        focusEdit = static_cast<QTextEdit *>(QApplication::focusWidget());

        if (firstTab->currentWidget() == focusEdit)
        {
            int index = firstTab->indexOf(focusEdit);
            if (index > 0)
                firstTab->setCurrentIndex(--index);
        }
        else if (secondTab->currentWidget() == focusEdit)
        {
            int index = secondTab->indexOf(focusEdit);
            if (index > 0)
                secondTab->setCurrentIndex(--index);
        }
    });

    connect(switchRightTab, &QShortcut::activated, this, [&]() {
        focusEdit = static_cast<QTextEdit *>(QApplication::focusWidget());

        if (firstTab->currentWidget() == focusEdit)
        {
            int index = firstTab->indexOf(focusEdit);
            if (index < firstEditList.count())
                firstTab->setCurrentIndex(++index);
        }
        else if (secondTab->currentWidget() == focusEdit)
        {
            int index = secondTab->indexOf(focusEdit);
            if (index < secondEditList.count())
                secondTab->setCurrentIndex(++index);
        }
    });

    connect(saveStc, &QShortcut::activated, this, [&]() {
        focusEdit = static_cast<QTextEdit *>(QApplication::focusWidget());

        if (firstTab->currentWidget() == focusEdit)
        {
            int index = firstTab->indexOf(focusEdit);

            if (firstTab->tabText(index) != "Untilited")
            {
                auto fileName = fileNameFirstEdit.find(focusEdit);

                QFile file(fileName->second);
                file.open(QIODevice::WriteOnly);
                file.write(focusEdit->toPlainText().toUtf8());
                file.close();
            }
            else
                saveFile();
        }
        else if (secondTab->currentWidget() == focusEdit)
        {
            int index = secondTab->indexOf(focusEdit);

            if (secondTab->tabText(index) != "Untilited")
            {
                auto fileName = fileNameSecondEdit.find(focusEdit);

                QFile file(fileName->second);
                file.open(QIODevice::WriteOnly);
                file.write(focusEdit->toPlainText().toUtf8());
                file.close();
            }
            else
                saveFile();
        }
    });

    connect(rightTabSplitter, &QShortcut::activated, this, [&]() {
        focusEdit = static_cast<QTextEdit *>(QApplication::focusWidget());

        if (firstTab->currentWidget() == focusEdit and not secondTab->isHidden())
        {
            secondTab->currentWidget()->setFocus();
        }
    });

    connect(leftTabSplitter, &QShortcut::activated, this, [&]() {
        focusEdit = static_cast<QTextEdit *>(QApplication::focusWidget());

        if (secondTab->currentWidget() == focusEdit and not firstTab->isHidden())
        {
            firstTab->currentWidget()->setFocus();
        }
    });

    connect(closeTabStc, &QShortcut::activated, this, [&]() {
        focusEdit = static_cast<QTextEdit *>(QApplication::focusWidget());

        if (firstTab->currentWidget() == focusEdit)
        {
            int index = firstTab->indexOf(focusEdit);
            closeFirstTab(index);
        }
        else if (secondTab->currentWidget() == focusEdit)
        {
            int index = secondTab->indexOf(focusEdit);
            closeSecondTab(index);
        }
    });

    connect(moveTab, &QShortcut::activated, this, [&]() {
        focusEdit = static_cast<QTextEdit *>(QApplication::focusWidget());

        if (firstTab->currentWidget() == focusEdit)
        {
            int index = firstTab->indexOf(focusEdit);
            secondTab->addTab(focusEdit, firstTab->tabText(index));
            secondTab->setCurrentWidget(focusEdit);

            secondEditList.push_back(focusEdit);

            focusEdit->setFocus();

            firstTab->removeTab(index);
            firstEditList.removeAt(index);

            if (firstEditList.count() == 0)
            {
                firstTab->hide();
            }

            fileNameSecondEdit.emplace(focusEdit, firstTab->tabText(index));

            if (secondTab->isHidden())
                secondTab->show();
        }
        else if (secondTab->currentWidget() == focusEdit)
        {
            int index = secondTab->indexOf(focusEdit);
            firstTab->addTab(focusEdit, secondTab->tabText(index));
            firstTab->setCurrentWidget(focusEdit);

            firstEditList.push_back(focusEdit);

            focusEdit->setFocus();

            secondTab->removeTab(index);
            secondEditList.removeAt(index);

            if (secondEditList.count() == 0)
            {
                secondTab->hide();
            }

            fileNameSecondEdit.emplace(focusEdit, secondTab->tabText(index));

            if (firstTab->isHidden())
                firstTab->show();
        }
    });
}

/* ---------------------------------------------------------------------------------- */

void Skeleton::wheelEvent(QWheelEvent *event)
{
    focusEdit = static_cast<QTextEdit *>(QApplication::focusWidget());

    if (QApplication::keyboardModifiers() == Qt::ControlModifier)
    {
        if (event->delta() > 0)
        {
            focusEdit->zoomIn();
        }
        else
        {
            focusEdit->zoomOut();
        }
    }
}

void Skeleton::iconChangeToBlack()
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

void Skeleton::iconChangeToWhite()
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

void Skeleton::loadThemes()
{
    QFile styleSheetFile;

    std::vector<QString> themeNames = {
        "default", "black", "white", "spybot", "bubble", "transparent"};

    for (const auto &theme : themeNames)
    {
        styleSheetFile.setFileName(":/" + theme + ".qss");
        styleSheetFile.open(QFile::ReadOnly);
        themes.emplace(theme, QLatin1String(styleSheetFile.readAll()));
        styleSheetFile.close();
    }
}