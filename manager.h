#ifndef MANAGER_H
#define MANAGER_H

#include <QStringList>
#include <QDir>
#include <QDirIterator>
#include <QTextEdit>
#include <QApplication>
#include <QPushButton>
#include <QTranslator>
#include <QException>
#include <QQueue>
#include <QPair>
#include <QCryptographicHash>

class Manager
{
public:
    Manager(QTextEdit *logs);
    ~Manager();

private:
    QTextEdit *logs;

public slots:
    QStringList getLocalMods();
    QStringList getAllFiles(QString dir = "", bool fulllocation = false);
    QStringList getAllFolders(QString dir);
    bool modInstalled(QString localDir, QString GTADir);
    bool move(QString folder, QString dir, bool remove);
    bool remove(QString folder, QString gta);
    bool copyRecursively(const QString &srcFilePath, const QString &tgtFilePath);
};

#endif // MANAGER_H
