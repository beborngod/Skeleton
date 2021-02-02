#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::MainWindow *ui;
    QSplitter *splitter;
    QTextEdit *edit;
    QTextEdit *second_edit;
    QTextEdit *focus_edit;

    QStackedWidget *stacked_windows;

    QPixmap newPixBlack;
    QPixmap openPixBlack;
    QPixmap savePixBlack;
    QPixmap minusPixBlack;
    QPixmap plusPixBlack;
    QPixmap clearPixBlack;
    QPixmap splitPixBlack;
    QPixmap settingsPixBlack;

    QPixmap newPixWhite;
    QPixmap openPixWhite;
    QPixmap savePixWhite;
    QPixmap minusPixWhite;
    QPixmap plusPixWhite;
    QPixmap clearPixWhite;
    QPixmap splitPixWhite;
    QPixmap settingsPixWhite;

    QAction *newFileAction;
    QAction *openFileAction;
    QAction *saveFileAction;
    QAction *zoomInAction;
    QAction *zoomOutAction;
    QAction *splitAction;
    QAction *settingsAction;
    QAction *clearDisplayAction;

    QGroupBox *themesGroupBox;
    QRadioButton *blackThemeButton;
    QRadioButton *whiteThemeButton;
    QRadioButton *defaultThemeButton;
    QRadioButton *spybotThemeButton;
    QRadioButton *obitThemeButton;
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void iconChangeToBlack();
    void iconChangeToWhite();
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
};
