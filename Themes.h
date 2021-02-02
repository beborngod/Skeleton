#pragma once

#include <QtWidgets>

class Themes : public QWidget
{
    Q_OBJECT
    QRadioButton *blackThemeButton;
    QRadioButton *whiteThemeButton;
    QRadioButton *defaultThemeButton;
    QRadioButton *spybotThemeButton;
    QRadioButton *obitThemeButton;

    QGroupBox *themesGroupBox;
public:
    explicit Themes(QWidget *parent = nullptr);
    void themeChanging();
    //void signalToBlack();
    //void signalToWhite();
};

