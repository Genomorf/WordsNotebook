#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QObject>
#include <QtWidgets>
#include "sqlwords.h"

class ClickableLabel : public QLabel
{
Q_OBJECT
public:
    explicit ClickableLabel(QFrame* line, SQL* sql_input, QString table, QWidget* parent=0);
    ~ClickableLabel();
    QVBoxLayout* setToLayout();
signals:
    void clicked();
protected:
    void mousePressEvent(QMouseEvent* event);
private:
    QFrame* parentLine;
    QVBoxLayout* chainLineLayout;
    SQL* sql;
    void createDeleteWindow();
    void connectToSql();
    QPoint lastPoint;
    bool b_move;
    QString tableName;
};

#endif // CLICKABLELABEL_H
