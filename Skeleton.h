#include "SyntaxPartisaner.h"
#include <QtWidgets>
#include <QtCore>
#include <QtGui>
#include <QPrinter>
#include <vector>
#include <map>
class Skeleton final : public QMainWindow
{
    Q_OBJECT

    QToolBar *toolbar;

    QSplitter *splitter;
    QTextEdit *firstEdit;
    QTextEdit *secondEdit;
    QTextEdit *focusEdit;

    QStackedWidget *stackedWindows;

    std::pair<QPixmap,QPixmap> newPix;
    std::pair<QPixmap,QPixmap> openPix;
    std::pair<QPixmap,QPixmap> savePix;
    std::pair<QPixmap,QPixmap> minusPix;
    std::pair<QPixmap,QPixmap> plusPix;
    std::pair<QPixmap,QPixmap> clearPix;
    std::pair<QPixmap,QPixmap> splitPix;
    std::pair<QPixmap,QPixmap> settingsPix;
    std::pair<QPixmap,QPixmap> undoPix;
    std::pair<QPixmap,QPixmap> redoPix;
    std::pair<QPixmap,QPixmap> pdfPix;

    QAction *newFileAction;
    QAction *openFileAction;
    QAction *saveFileAction;
    QAction *zoomInAction;
    QAction *zoomOutAction;
    QAction *splitAction;
    QAction *settingsAction;
    QAction *clearDisplayAction;
    QAction *undoAction;
    QAction *redoAction;
    QAction *pdfAction;

    QGroupBox *themesGroupBox;
    QRadioButton *blackThemeButton;
    QRadioButton *whiteThemeButton;
    QRadioButton *defaultThemeButton;
    QRadioButton *spybotThemeButton;
    QRadioButton *aquaThemeButton;
    QRadioButton *bubbleThemeButton;
    QRadioButton *transparentThemeButton;

    QRadioButton *syntaxPartisanerButton;
    QRadioButton *syntaxExpanButton;
    QRadioButton *noHighlightButton;

    std::map<QString,QString>themes;

    SyntaxPartisaner *syntax;

    QSettings *settings;

    enum{ BLACK_ICONS = 1, WHITE_ICONS = 2 };
public:
    explicit Skeleton(QWidget *parent = nullptr);
    ~Skeleton();
protected slots:
    void newFile();
    void openFile();
    void saveFile();
    void zoomTextIn();
    void zoomTextOut();
    void splitDisplay();
    void clear();
    void goToSettings();
    void themeChanging();
    void undoText();
    void redoText();
    void saveToPdf();
    void setSyntaxHighlight();
    void setFontEdit();
private:
    QString getTheme();
    void setTheme(QString theme = "default",int color = WHITE_ICONS);
    int getThemeIcons();
    void getHighlight();
    void setRadionButtonChecked(QString radioButton);

    void iconChangeToBlack();
    void iconChangeToWhite();
    void loadThemes();
    void loadIcons();
    void loadSettings();
    void saveSettings();
};
