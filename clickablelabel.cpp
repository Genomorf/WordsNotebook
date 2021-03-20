#include "clickablelabel.h"
#include <QtWidgets>
#include "sqlwords.h"

ClickableLabel::ClickableLabel(QFrame* line, SQL* sql_input, QString table, QWidget* parent)
    : QLabel(parent), parentLine(line), sql(sql_input), tableName(table)
{
}

ClickableLabel::~ClickableLabel()
{
}

QVBoxLayout *ClickableLabel::setToLayout()
{
    chainLineLayout = new QVBoxLayout();
    chainLineLayout->addWidget(this);
    return chainLineLayout;
}

void ClickableLabel::mousePressEvent( QMouseEvent* event)
{
    if (event->button() == Qt::RightButton){
        createDeleteWindow();
    }
    emit clicked();

}

void ClickableLabel::createDeleteWindow()
{
    QPoint globalCursorPos = QCursor::pos();
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText("Delete?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setGeometry(globalCursorPos.x(), globalCursorPos.y() ,msgBox.width(), msgBox.height());
    int ret = msgBox.exec();

    switch (ret) {

      case QMessageBox::Ok:
        connectToSql();
        break;

      case QMessageBox::Cancel:
        break;
    }
}

void ClickableLabel::connectToSql()
{
    sql->deleteChainLine(tableName, this->text());
    this->hide();
    delete chainLineLayout;
    delete this;
    parentLine->hide();
    delete parentLine;
}

