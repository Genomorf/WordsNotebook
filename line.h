#ifndef LINE_H
#define LINE_H

#include <QWidget>
#include <QtWidgets>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "sqlwords.h"

class Line : public QWidget
{
    Q_OBJECT
public:
    explicit Line(int num, QWidget *parent = nullptr);
    QHBoxLayout* createLine(QString first_str = "", QString second_str = "");
    QLineEdit* firstField;
    QLineEdit* secondField;
    void connectToSql(SQL* sql_input);
    int number;

private:
    //constants
    const int MIN_WIDTH_BTN = 140;
    const int MIN_HEIGHT_BTN = 31;
    const int FONT_SIZE = 20;


    QFont mainTextFont;
    QVBoxLayout* chainTableLayout;
    QPoint lastPoint;
    bool b_move;
    QFrame* chainFrame;
    bool isTxtChanged;
    bool isChainPressed = false;
    QPushButton* chainBtn;
    QPushButton* addToChainBtn;
    QTextBrowser* tbrowser;
    QPushButton* deleteBtn;
    QHBoxLayout* horizLay;
    QLineEdit* chainRuField;
    QLineEdit* chainEngField;
    QString engFieldTxt;
    QString ruFieldTxt;

    void openChainField();
    void EngFieldSaveTxt(const QString str);
    void RuFieldSaveTxt(const QString str);
    void saveAddToChainTxtToSQL();
    void deleteLine();
    void saveLine();
    void finishFirstField(const QString str);
    void finishSecondField(const QString str);
    SQL* sql;
signals:

};

#endif // LINE_H
