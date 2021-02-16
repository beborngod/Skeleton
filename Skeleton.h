#include "SyntaxPartisaner.h"
#include <QStackedWidget>
#include <QApplication>
#include <QRadioButton>
#include <QMainWindow>
#include <QPushButton>
#include <QMessageBox>
#include <QFontDialog>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QShortcut>
#include <QSplitter>
#include <QTextEdit>
#include <QGroupBox>
#include <QSettings>
#include <QLineEdit>
#include <QToolBar>
#include <QPrinter>
#include <QPixmap>
#include <QThread>
#include <thread>
#include <QLabel>
#include <vector>
#include <list>
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

    std::map<QString,QString>themes;
    
    QList<QTextEdit *> firstEditList;
    QList<QTextEdit *> secondEditList;

    QSettings *settings;

    QFont editFont;
    QLineEdit *fontLineEdit;
    QPushButton *setFontButton;

    enum{ BLACK_ICONS = 1, WHITE_ICONS = 2};

    QTabWidget *firstTab;
    QTabWidget *secondTab;
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
    void setFontEdit();
    void closeFirstTab(int index);
    void closeSecondTab(int index);
private:
    QString getTheme();
    void setTheme(QString theme = "default",int color = WHITE_ICONS);
    int getThemeIcons();
    void setRadionButtonChecked(QString radioButton);
    void iconChangeToBlack();
    void iconChangeToWhite();
    void loadThemes();
    void loadIcons();
    void loadSettings();
    void saveSettings();
    void settingsPanel();

    void wheelEvent(QWheelEvent *event) override;
};
