#include "sqlwords.h"
#include <QtWidgets>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "line.h"
#include "mainwindow.h"

SQL::SQL(QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");

    #ifdef QT_NO_DEBUG
        QString dbSource("rl.db");
    #else
        QString dbSource("debug.db");
    #endif

    db.setDatabaseName(dbSource);
    if(db.open()){
        qDebug() << "sqlwords.cpp DB open";
    } else {
        qDebug() << "sqlwords.cpp Couldn't open db";
    }
}


QList<QPair<QString, QString>> SQL::getLines()
{
    QList<QPair<QString, QString>> table = QList<QPair<QString, QString>>();
    QSqlQuery query = QSqlQuery(db);
    while(query.next()){
          table.push_back(QPair<QString, QString>(query.value(0).toString(), query.value(1).toString()));
    }
    return table;
}


void SQL::insertWords(QString first, QString sec, int Id, QTextBrowser* tb)
{
    checkAndCorrectStr(first);
    checkAndCorrectStr(sec);
    if (first.isEmpty()){
        first = " ";
    }
    else if (sec.isEmpty()){
        sec = " ";
    }

    QSqlQuery query = QSqlQuery(db);

    // delete object from SQL table to avoid repeating lines
    QString queryDeleteTxt = QString("DELETE FROM words WHERE Id='%1'")
            .arg(Id);

    if(!query.exec(queryDeleteTxt)){
        tb->setText(QString("Insert failed"));
        return;
    }

    auto queryInsertTxt = QString("INSERT INTO words (Russian, English, Id) VALUES ('%1', '%2', %3)").
            arg(first).arg(sec).arg(Id);

    if (query.exec(queryInsertTxt)){
        tb->setText(QString("Insert succsess"));
    }
    else {
        tb->setText(QString("Insert failed"));
        return;
    }
}


void SQL::deleteWords(int Id, QTextBrowser* tb)
{
    QSqlQuery query = QSqlQuery(db);
    // common delete func object from table
    QString queryDeleteTxt = QString("DELETE FROM words WHERE Id='%1'")
            .arg(Id);

    if(query.exec(queryDeleteTxt)){
        tb->setText(QString("Delete success! Id : %1").arg(Id));
    } else {
        tb->setText(QString("Delete failed! Id : %1").arg(Id));
        return;
    }

    QString querySelectTxt = QString("SELECT Id FROM words");
    if(!query.exec(querySelectTxt)){
        return;
    }

    // Ids is a QList with new ids to replace the old ones.
    // Values is a QList with old ids.
    // Algorithm: firstly we get old ids from SQL table and write them to QList Values,
    // then we fill QList ids with values from 1 to ... according to s_numberOfLineObjects.
    // Finally we create QMap valuesMap with pair (old ids, new ids) and
    // change old to new ids in the Sql table.
    QList<int> ids;
    QList<int> values;
    while(query.next()){
        int value = query.value(0).toInt();
        values.push_back(value);
    }
    for (int i = 1; i < MainWindow::s_listLines.size() + 1; ++i){
        ids.push_back(i);
    }
    QMap<int, int> valuesMap;
    for (int i = 0; i < MainWindow::s_listLines.size(); ++i){
        valuesMap[values[i]] = ids[i];
    }

    for (int i = 0; i < MainWindow::s_listLines.size(); ++i){
        QString queryTxtUpdate = QString("UPDATE words SET Id = %1 WHERE Id = %2").
                arg(ids[i]).arg(values[i]);
                if(query.exec(queryTxtUpdate)){
                    tb->setText(QString("Update success! Id : %1 to Id : %2").arg(values[i]).arg(ids[i]));
                } else {
                     tb->setText(QString("Update failed!"));
                    return;
                }
        MainWindow::s_listLines[i]->number = ids[i];
    }
}


void SQL::createTable(QString name, QTextBrowser* tb)
{
    checkAndCorrectStr(name);
    QSqlQuery query = QSqlQuery(db);

    QString createTableTxt = QString("CREATE TABLE %1 (word TEXT, tr TEXT);").arg(name);
    if(query.exec(createTableTxt)){
        tb->setText(QString("Insert words success!"));
    } else {
        tb->setText(QString("Insert words failed!"));
        return;
    }
}


void SQL::deleteTable(QString name, QTextBrowser* tb)
{
    checkAndCorrectStr(name);
    QSqlQuery query = QSqlQuery(db);

    checkAndCorrectStr(name);

    QString createTableTxt = QString("DROP TABLE %1").arg(name);
    if(query.exec(createTableTxt)){
        tb->setText(QString("Table deleted!"));
    } else {
        tb->setText(QString("Couldn't delete table!"));
        return;
    }
}


void SQL::deleteChainLine(QString table, QString word)
{
    checkAndCorrectStr(table);
    QSqlQuery query = QSqlQuery(db);

    QString deleteChainTxt = QString("DELETE FROM %1 WHERE word='%2'").arg(table).arg(word);
    if(!query.exec(deleteChainTxt)){
        return;
    }
}


void SQL::saveTxtToChainTable(QString tableName, QString enTxt, QString ruTxt, QTextBrowser* tb)
{
    QSqlQuery query = QSqlQuery(db);
    checkAndCorrectStr(tableName);
    checkAndCorrectStr(enTxt);
    checkAndCorrectStr(ruTxt);
    QString insertToChainTableTxt = QString("INSERT INTO %1 (word, tr) VALUES ('%2', '%3')").arg(tableName).arg(enTxt).arg(ruTxt);
    if(query.exec(insertToChainTableTxt)){
        tb->setText(QString("Chain saved! %1 : %2").arg(enTxt).arg(ruTxt));
    } else {
        tb->setText(QString("Chain save failed! %1 : %2").arg(enTxt).arg(ruTxt));
    }
}


QVector<QPair<QString, QString>> SQL::getChainTable(QString name)
{
    QSqlQuery query = QSqlQuery(db);

    checkAndCorrectStr(name);

    QString getTableTxt = QString("SELECT * FROM '%1'").arg(name);

    QVector<QPair<QString, QString>> table;
    while(query.next()){
        table.push_back(QPair<QString, QString>(query.value(0).toString(), query.value(1).toString()));
    }
    return table;
}


void SQL::checkAndCorrectStr(QString &str)
{
    for (auto it = str.begin(); it != str.end(); ++it){
        if (!it->isLetter()){
            *it = '_';
        }
    }
}
