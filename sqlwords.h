#ifndef SQL_H
#define SQL_H

#include <QObject>
#include <QtWidgets>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>

class SQL : public QObject
{
    Q_OBJECT
public:
    explicit SQL(QObject *parent = nullptr);
    QList<QPair<QString, QString>> getLines();
    void insertWords(QString first, QString sec, int Id, QTextBrowser* tb);
    void deleteWords(int Id,  QTextBrowser*);
    void createTable(QString name, QTextBrowser* tb);
    void deleteTable(QString name, QTextBrowser* tb);
    void deleteChainLine(QString, QString);
    void saveTxtToChainTable(QString tableName, QString enTxt, QString ruTxt, QTextBrowser* tb);
    QVector<QPair<QString, QString>> getChainTable(QString name);
private:
    void checkAndCorrectStr(QString& str);
    QSqlDatabase db;
signals:

};

#endif // SQL_H
