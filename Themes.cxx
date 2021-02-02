#include "Themes.h"

Themes::Themes(QWidget *parent) : QWidget(parent)
{
    themesGroupBox = new QGroupBox("   Theme");

    defaultThemeButton = new QRadioButton("Default", this);
    blackThemeButton = new QRadioButton("Black", this);
    whiteThemeButton = new QRadioButton("White", this);
    spybotThemeButton  = new QRadioButton("SpyBot", this);
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

    connect(defaultThemeButton,&QRadioButton::clicked,this,&Themes::themeChanging);
    connect(whiteThemeButton,&QRadioButton::clicked,this,&Themes::themeChanging);
    connect(blackThemeButton,&QRadioButton::clicked,this,&Themes::themeChanging);
    connect(spybotThemeButton,&QRadioButton::clicked,this,&Themes::themeChanging);
    connect(obitThemeButton,&QRadioButton::clicked,this,&Themes::themeChanging);
}

void Themes::themeChanging()
{
    if (defaultThemeButton->isChecked())
    {
        QFile styleSheetFile(":/default.qss");
        styleSheetFile.open(QFile::ReadOnly);
        QString styleSheetDefault = QLatin1String(styleSheetFile.readAll());
        setStyleSheet(styleSheetDefault);
    }
    else if (blackThemeButton->isChecked())
    {
        QFile styleSheetFile(":/black.qss");
        styleSheetFile.open(QFile::ReadOnly);
        QString styleSheetBlack = QLatin1String(styleSheetFile.readAll());
        setStyleSheet(styleSheetBlack);
    }
    else if (whiteThemeButton->isChecked())
    {
        QFile styleSheetFile(":/white.qss");
        styleSheetFile.open(QFile::ReadOnly);
        QString styleSheetWhite = QLatin1String(styleSheetFile.readAll());
        setStyleSheet(styleSheetWhite);
    }
    else if (spybotThemeButton->isChecked())
    {
        QFile styleSheetFile(":/SpyBot.qss");
        styleSheetFile.open(QFile::ReadOnly);
        QString styleSheetSpybot = QLatin1String(styleSheetFile.readAll());
        setStyleSheet(styleSheetSpybot);
    }
    else if (obitThemeButton->isChecked())
    {
        QFile styleSheetFile(":/Obit.qss");
        styleSheetFile.open(QFile::ReadOnly);
        QString styleSheetSpybot = QLatin1String(styleSheetFile.readAll());
        setStyleSheet(styleSheetSpybot);
    }

}

