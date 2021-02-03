#include <QtWidgets>
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

    QPixmap newPixBlack;
    QPixmap openPixBlack;
    QPixmap savePixBlack;
    QPixmap minusPixBlack;
    QPixmap plusPixBlack;
    QPixmap clearPixBlack;
    QPixmap splitPixBlack;
    QPixmap settingsPixBlack;
    QPixmap undoPixBlack;
    QPixmap redoPixBlack;

    QPixmap newPixWhite;
    QPixmap openPixWhite;
    QPixmap savePixWhite;
    QPixmap minusPixWhite;
    QPixmap plusPixWhite;
    QPixmap clearPixWhite;
    QPixmap splitPixWhite;
    QPixmap settingsPixWhite;
    QPixmap undoPixWhite;
    QPixmap redoPixWhite;

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

    QGroupBox *themesGroupBox;
    QRadioButton *blackThemeButton;
    QRadioButton *whiteThemeButton;
    QRadioButton *defaultThemeButton;
    QRadioButton *spybotThemeButton;
    QRadioButton *obitThemeButton;
    QRadioButton *aquaThemeButton;
    QRadioButton *macosThemeButton;

    std::map<QString,QString>themes;
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
private:
    void iconChangeToBlack();
    void iconChangeToWhite();
    void loadThemes();
    void loadIcons();
};
