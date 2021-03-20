#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include "sqlwords.h"
#include "line.h"
#include "mainwindow.h"

#define MAX_WIDTH_LINE 150

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mainTextFont.setPixelSize(FONT_SIZE);

    createCentralWidgetLayout();

    fillWordsLines();

    qDebug() << "sqlword.cpp Line::s_numberOfLineObjects: " << MainWindow::s_listLines.size() << '\n'
             << "MainWindow::s_listLines.size: " << MainWindow::s_listLines.size();

}

QList<Line*> MainWindow::s_listLines;
int MainWindow::numberOfLinesInLayout = 1;

MainWindow::~MainWindow()
{
    delete ui;
}



QFrame *MainWindow::createTitleFrame()
{
    qDebug() << "mainw.cpp Creating title...";

    QFrame* titleWidget = new QFrame();
    titleWidget->setFrameShape(QFrame::Box);


    sortFirstBtn = new QPushButton();
    sortFirstBtn->setMaximumSize(static_cast<int>(SORT_BTN_SIZE::height), static_cast<int>(SORT_BTN_SIZE::width));
    sortFirstBtn->setText(arrowDown);
    connect(sortFirstBtn, &QPushButton::clicked, this, &MainWindow::pressArrowRu);

    sortSecBtn = new QPushButton();
    sortSecBtn->setMaximumSize(static_cast<int>(SORT_BTN_SIZE::height), static_cast<int>(SORT_BTN_SIZE::width));
    sortSecBtn->setText(arrowDown);
    connect(sortSecBtn, &QPushButton::clicked, this, &MainWindow::pressArrowEn);

    QLabel* title1 = new QLabel();
    title1->setFont(mainTextFont);
    title1->setText("RUSSIAN");

    QLabel* title2 = new QLabel();
    title2->setText("ENGLISH");
    title2->setFont(mainTextFont);

    QHBoxLayout* titleLayout = new QHBoxLayout();
    titleLayout->addWidget(title1);
    titleLayout->setAlignment(Qt::AlignLeft);
    titleLayout->addWidget(sortFirstBtn);
    titleLayout->addSpacing(TITLE_LAYOUT_SPACING);
    titleLayout->addWidget(title2);
    titleLayout->addWidget(sortSecBtn);

    titleWidget->setLayout(titleLayout);
    return titleWidget;
}



QFrame *MainWindow::createAddButtonFrame()
{
    qDebug() << "mainw.cpp Creating add btn...";

    QFrame* addButtonFrame = new QFrame();

    QVBoxLayout* addButtonLayout = new QVBoxLayout();

    QPushButton* addButton = new QPushButton();
    addButton->setText("Add new word");
    addButton->setMinimumSize(static_cast<int>(ADD_BTN_SIZE::height),static_cast<int>(ADD_BTN_SIZE::width));

//    QPalette pal = addButton->palette();
//    pal.setBrush(QPalette::Window, QColor(255, 0, 0)); //red color
//    pal.setBrush(QPalette::Button, QColor(128, 223, 255)); //blue color (hex representation)
//    addButton->setPalette(pal);
//    addButton->update();
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addNewWordBtn);

    addButtonLayout->addWidget(addButton);
    addButtonLayout->setAlignment(Qt::AlignRight);

    addButtonFrame->setLayout(addButtonLayout);

    return addButtonFrame;
}


QScrollArea *MainWindow::createCentralWidgetScrollArea()
{
    qDebug() << "mainw.cpp Creating scroll area...";

    QFrame* wordsLines = new QFrame();
    wordsLines->setFrameShape(QFrame::Box);

    wordsLinesLayout = new QVBoxLayout();

    wordsLinesLayout->setAlignment(Qt::AlignTop);
    wordsLines->setLayout(wordsLinesLayout);

    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(wordsLines);
    return scrollArea;
}


void MainWindow::createCentralWidgetLayout()
{
    qDebug() << "mainw.cpp Creating central widget...";

    QFrame* titleWidget = createTitleFrame();

    QFrame* addButtonFrame = createAddButtonFrame();

    QScrollArea* scrollArea = createCentralWidgetScrollArea();

    QVBoxLayout* centralWidgetLayout = new QVBoxLayout();

    centralWidgetLayout->addWidget(titleWidget);
    centralWidgetLayout->addWidget(scrollArea);
    centralWidgetLayout->addWidget(addButtonFrame);

    centralWidgetLayout->setAlignment(Qt::AlignTop);

    ui->centralwidget->setLayout(centralWidgetLayout);
}

void MainWindow::fillWordsLines()
{
    qDebug() << "mainw.cpp Fill lines_f. Connection to SQL...";
    sql = new SQL();

    // words pairs table: English -- Russian
    auto table = sql->getLines();

    for (auto it = table.begin(); it != table.end(); ++it){

        // create new line
        Line* line = new Line(numberOfLinesInLayout);

        // give to the object pointer to SQL
        line->connectToSql(sql);

        // create layout and fill two fields with words from the above table
        // it->first = Russian, it->second = English
        auto lineLayout = line->createLine(it->first, it->second);
        wordsLinesLayout->addLayout(lineLayout);

        // increment number of lines in layout
        ++numberOfLinesInLayout;

        // add to static QList object
        s_listLines.push_back(line);
    }
}

void MainWindow::pressArrowRu()
{
    if (isArrowRuPressed){
        sortFirstBtn->setText(arrowDown);
        isArrowRuPressed = false;
        sortWidgetLines(Asc, Ru);
    } else {
        sortFirstBtn->setText(arrowUp);
        isArrowRuPressed = true;
        sortWidgetLines(Desc, Ru);
    }

}

void MainWindow::pressArrowEn()
{
    if (isArrowEnPressed){
        sortSecBtn->setText(arrowDown);
        isArrowEnPressed = false;
        sortWidgetLines(Asc, En);
    } else {
        sortSecBtn->setText(arrowUp);
        isArrowEnPressed = true;
        sortWidgetLines(Desc, En);
    }
}

void MainWindow::addNewWordBtn()
{
    Line* line = new Line(numberOfLinesInLayout);
    ++numberOfLinesInLayout;
    line->connectToSql(sql);
    wordsLinesLayout->addLayout(line->createLine());
}

void MainWindow::sortWidgetLines(sortType type, lanType lan)
{

    const int fieldStretch = lan == En ? 1 : 0;

    int pos = 0;
    for (int i = 0; i < wordsLinesLayout->count() ; ++i){

        auto l_current = wordsLinesLayout->itemAt(i)->layout();
        QLineEdit* fieldCurrent = qobject_cast<QLineEdit*>(l_current->itemAt(0 + fieldStretch)->widget());

        for (int j = i; j < wordsLinesLayout->count(); ++j){

            auto l_next = wordsLinesLayout->itemAt(j)->layout();
            QLineEdit* fieldNext = qobject_cast<QLineEdit*>(l_next->itemAt(0 + fieldStretch)->widget());

            if (type == Asc){
                if (fieldCurrent->text()[0] > fieldNext->text()[0]){
                    qDebug() << "It:" << pos << fieldCurrent->text() << "> " << fieldNext->text();
                    fieldCurrent = fieldNext;
                    l_current = l_next;
                }
            }
            else if (type == Desc){
                if (fieldCurrent->text()[0] < fieldNext->text()[0]){
                    qDebug() << "It:" << pos << fieldCurrent->text() << "< " << fieldNext->text();
                    fieldCurrent = fieldNext;
                    l_current = l_next;
                }
            }

        wordsLinesLayout->removeItem(l_current);
        wordsLinesLayout->insertLayout(i, l_current);
        }
        ++pos;
    }
}



