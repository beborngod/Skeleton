#include "Syntaxhighlighter.h"
#include <QtWidgets>
#include <QPrinter>
#include <vector>
#include <map>
class MainWindow : public QMainWindow
{
    Q_OBJECT

    QToolBar *toolbar;

    QSplitter *splitter;
    QTextEdit *mainEdit;
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
    QRadioButton *macosThemeButton;
    QRadioButton *transparentThemeButton;

    QRadioButton *syntaxPartisanerButton;
    QRadioButton *syntaxExpanButton;

    std::map<QString,QString>themes;

    SyntaxPartisaner *syntaxPartisanerMainEdit;
    SyntaxPartisaner *syntaxPartisanerSecondEdit;

    SyntaxExpan *syntaxExpanMainEdit;
    SyntaxExpan *syntaxExpanSecondEdit;
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected slots:
    void newFile();
    void openFile();
    void saveFile();
    void zoomTextIn();
    void zoomTextOut();
    void splitDisplay();
    void clear();
    void settings();
    void themeChanging();
    void undoText();
    void redoText();
    void saveToPdf();
    void setSyntaxHighlight();
private:
    void iconChangeToBlack();
    void iconChangeToWhite();
    void loadThemes();
    void loadIcons();
};
