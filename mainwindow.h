#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "sqlwords.h"
#include "line.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static int numberOfLinesInLayout;
    static QList<Line*> s_listLines;
private:
    //constantss
    const int FONT_SIZE = 20;
    const int TITLE_LAYOUT_SPACING = 30;
    enum class SORT_BTN_SIZE {height = 30, width = 30 };
    enum class ADD_BTN_SIZE {height = 150, width = 60};

    QFrame* createTitleFrame();
    QFrame* createAddButtonFrame();
    QScrollArea* createCentralWidgetScrollArea();
    void createCentralWidgetLayout();
    void fillWordsLines();
    void pressArrowRu();
    void pressArrowEn();
    void addNewWordBtn();
    QPushButton* sortFirstBtn;
    QPushButton* sortSecBtn;
    QString arrowDown = "\u02C5";
    bool isArrowRuPressed = false;
    QString arrowUp = "\u02C4";
    bool isArrowEnPressed = false;
    SQL* sql;

    Ui::MainWindow *ui;
    QFont mainTextFont;
    QVBoxLayout* wordsLinesLayout;
    enum sortType {Asc, Desc};
    enum lanType {En, Ru};
    void sortWidgetLines(sortType, lanType);

};

#endif // MAINWINDOW_H
