#include "line.h"
#include <QtWidgets>
#include <sqlwords.h>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "mainwindow.h"
//#include "chain.h"
#include <QSizePolicy>
#include "clickablelabel.h"

Line::Line(int num, QWidget *parent) : QWidget(parent), number(num)
{

}


QHBoxLayout* Line::createLine(QString firstStr, QString secondStr)
{

    mainTextFont.setPixelSize(FONT_SIZE);

    /// ----------------
    /// | RUSSIAN      |
    /// ----------------
    ///
    QLineEdit* field1 = new QLineEdit();
    QSizePolicy sizeForFields(QSizePolicy::Fixed, QSizePolicy::Fixed);
    field1->setSizePolicy(sizeForFields);
    field1->setMinimumWidth(MIN_WIDTH_BTN);
    field1->setMaximumWidth(MIN_WIDTH_BTN);
    field1->setFont(mainTextFont);
    field1->setText(firstStr);
    firstField = field1;

    connect(field1, &QLineEdit::editingFinished, this, &Line::saveLine);
    connect(field1, &QLineEdit::textChanged, this, &Line::finishFirstField);


    /// ----------------
    /// |  ENGLISH      |
    /// ----------------
    ///
    QLineEdit* field2 = new QLineEdit();
    field2->setSizePolicy(sizeForFields);
    field2->setFont(mainTextFont);
    field2->setMinimumWidth(MIN_WIDTH_BTN);
    field2->setMaximumWidth(MIN_WIDTH_BTN);
    field2->setText(secondStr);
    secondField = field2;

    connect(field2, &QLineEdit::editingFinished, this, &Line::saveLine);
    connect(field2, &QLineEdit::textChanged, this, &Line::finishSecondField);


    /// ----------------
    /// |  Chain...     |
    /// ----------------
    ///
    chainBtn = new QPushButton();
    chainBtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    chainBtn->setText("Chain...");
    chainBtn->setMinimumSize(MIN_WIDTH_BTN, MIN_HEIGHT_BTN);

    connect(chainBtn, &QPushButton::clicked, this, &Line::openChainField);


    /// ------------------
    /// | ...            |
    /// ------------------
    ///
    tbrowser = new QTextBrowser();
    tbrowser->setText("");
    tbrowser->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    tbrowser->setMaximumSize(MIN_WIDTH_BTN, MIN_HEIGHT_BTN);


    /// ----------------
    /// |  Delete      |
    /// ----------------
    ///
    deleteBtn = new QPushButton();
    deleteBtn->setText("Delete");
    deleteBtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    deleteBtn->setMinimumSize(MIN_WIDTH_BTN, MIN_HEIGHT_BTN);

    connect(deleteBtn, &QPushButton::clicked, this, &Line::deleteLine);

    /// ------------------
    /// | Add to chain   |
    /// ------------------
    ///
    addToChainBtn = new QPushButton();
    addToChainBtn->setText("Add to chain");
    addToChainBtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    addToChainBtn->setMinimumSize(MIN_WIDTH_BTN, MIN_HEIGHT_BTN);

    connect(addToChainBtn, &QPushButton::clicked, this, &Line::saveAddToChainTxtToSQL);

    ///
    /// ---------------  ----------------
    /// | EngField     | |   RuField     |
    /// ---------------  ----------------
    chainEngField = new QLineEdit();
    chainEngField->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    chainEngField->setMinimumSize(MIN_WIDTH_BTN, MIN_HEIGHT_BTN);
    chainEngField->setFont(mainTextFont);

    chainRuField = new QLineEdit();
    chainRuField->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    chainRuField->setMinimumSize(MIN_WIDTH_BTN, MIN_HEIGHT_BTN);
    chainRuField->setFont(mainTextFont);

    QHBoxLayout* EngRuFieldsLayout = new QHBoxLayout();
    EngRuFieldsLayout->setAlignment(Qt::AlignLeft);
    EngRuFieldsLayout->addWidget(chainEngField);
    EngRuFieldsLayout->addWidget(chainRuField);

    connect(chainEngField, &QLineEdit::textChanged, this, &Line::EngFieldSaveTxt);
    connect(chainRuField, &QLineEdit::textChanged, this, &Line::RuFieldSaveTxt);

    chainFrame = new QFrame();
    chainFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chainFrame->setFrameShape(QFrame::Box);
    auto chainTable = sql->getChainTable(secondField->text());

    chainTableLayout = new QVBoxLayout();
    chainTableLayout->setAlignment(Qt::AlignTop);

    for (auto it = chainTable.begin(); it !=chainTable.end(); ++it){
        QFrame* chainLine = new QFrame();

        chainLine->setFrameShape(QFrame::Box);
        chainLine->setMinimumHeight(MIN_HEIGHT_BTN + 10);
        chainLine->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

        ClickableLabel* chainLineTxt = new ClickableLabel(chainLine, sql, secondField->text());

        chainLineTxt->setText(it->first);
        chainLineTxt->setFont(mainTextFont);
        chainLineTxt->setToolTip(it->second);

        QVBoxLayout* chainLineLayout = chainLineTxt->setToLayout();
        chainLine->setLayout(chainLineLayout);

        chainTableLayout->addWidget(chainLine);
    }

    chainTableLayout->addLayout(EngRuFieldsLayout);

    chainTableLayout->addWidget(addToChainBtn);
    chainFrame->setLayout(chainTableLayout);
    chainFrame->hide();

    QVBoxLayout* l = new QVBoxLayout();
    l->addWidget(chainFrame);

    horizLay = new QHBoxLayout();
    horizLay->addWidget(field1);
    horizLay->addWidget(field2);
    horizLay->addWidget(chainBtn);
    horizLay->addWidget(tbrowser);
    horizLay->addWidget(deleteBtn);
    horizLay->addLayout(l);

    horizLay->setAlignment(l, Qt::AlignCenter);
    horizLay->setAlignment(field1, Qt::AlignTop);
    horizLay->setAlignment(field2, Qt::AlignTop);
    horizLay->setAlignment(chainBtn, Qt::AlignTop);
    horizLay->setAlignment(tbrowser, Qt::AlignTop);
    horizLay->setAlignment(deleteBtn, Qt::AlignTop);

    return horizLay;
}

void Line::connectToSql(SQL* sqlInput)
{
    sql = sqlInput;
}

void Line::openChainField()
{
    if (!isChainPressed)
    {
        tbrowser->hide();
        deleteBtn->hide();
        chainFrame->show();
        isChainPressed = true;
    }
    else {
        tbrowser->show();
        deleteBtn->show();
        chainFrame->hide();
        isChainPressed = false;
    }
}

void Line::EngFieldSaveTxt(const QString txt)
{
    engFieldTxt = txt;
    qDebug() << engFieldTxt;
}

void Line::RuFieldSaveTxt(const QString txt)
{
    ruFieldTxt = txt;
    qDebug() << ruFieldTxt;
}

void Line::saveAddToChainTxtToSQL()
{
    if (!engFieldTxt.isEmpty() && !ruFieldTxt.isEmpty()){
        sql->saveTxtToChainTable(secondField->text(), engFieldTxt, ruFieldTxt, tbrowser);

        int pos = chainTableLayout->count();

        QFrame* chainLineLocal = new QFrame();
        chainLineLocal->setFrameShape(QFrame::Box);
        chainLineLocal->setMinimumHeight(MIN_HEIGHT_BTN + 10);
        chainLineLocal->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

        ClickableLabel* chainLineTxt = new ClickableLabel(chainLineLocal, sql, secondField->text());

        chainLineTxt->setText(engFieldTxt);
        chainLineTxt->setFont(mainTextFont);
        chainLineTxt->setToolTip(ruFieldTxt);

        QVBoxLayout* chainLineLayout = chainLineTxt->setToLayout();

        chainLineLocal->setLayout(chainLineLayout);
        chainTableLayout->insertWidget(pos - 2, chainLineLocal);

        chainEngField->clear();
        chainRuField->clear();
    } else {
        QMessageBox::warning(this, "Warning", "Type text in both fields before saving!");
    }
}

void Line::deleteLine()
{
    if (MainWindow::s_listLines.removeOne(this)){
        qDebug() << "line.cpp removed from s_listLines! \n"
                 << "s_numberOfLineObjects: " << MainWindow::s_listLines.size() << '\n'
                 << "s_listLines.size: " <<  MainWindow::s_listLines.size();
    }
    sql->deleteWords(number, tbrowser);
    sql->deleteTable(secondField->text(), tbrowser);

    --MainWindow::numberOfLinesInLayout;
    firstField->hide();
    secondField->hide();
    tbrowser->hide();
    deleteBtn->hide();
    chainBtn->hide();
    delete this;
}

void Line::saveLine()
{
    QString ff_text = firstField->text();
    QString sf_text = secondField->text();

    if (!ff_text.isEmpty() && !sf_text.isEmpty() && isTxtChanged){
        if(!MainWindow::s_listLines.contains(this)){
            MainWindow::s_listLines.push_back(this);
        }
        sql->insertWords(ff_text, sf_text, number, tbrowser);
        sql->createTable(sf_text, tbrowser);
    }
    isTxtChanged = false;
}

void Line::finishFirstField(const QString str)
{
    firstField->setText(str);
    isTxtChanged = true;
}

void Line::finishSecondField(const QString str)
{
    secondField->setText(str);
    isTxtChanged = true;
}
