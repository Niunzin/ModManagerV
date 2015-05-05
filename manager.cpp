#include "manager.h"

#include <QDebug>
#include <QTranslator>

Manager::Manager(QTextEdit *logs)
{
    this->logs = logs;
}

Manager::~Manager()
{

}

QStringList Manager::getLocalMods()
{
    try
    {
        QStringList localMods;
        QDirIterator localIterator(QDir().currentPath(), QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
        while (localIterator.hasNext()) { localMods.append(localIterator.next()); }
        return localMods;
    } catch(QException* error)
    {
        logs->append(QApplication::tr("<span style=\"color: #ea7878\"><b>•</b> Error in function getLocalMods: %1</span>").arg(error->what()));
        return QStringList();
    }
}

QStringList Manager::getAllFiles(QString dir, bool fulllocation)
{
    try
    {
        QStringList files;
        QDirIterator localIterator(dir, QDir::Files);
        while (localIterator.hasNext()) {
            QString filestr(localIterator.next());

            if(!fulllocation)
            {
                if(filestr.contains("/"))
                {
                    int lenght = filestr.split("/").count() - 1;
                    QString filename = QString(filestr.split("/")[lenght]);
                    files.append(filename);
                }
            } else files.append(filestr);
        }
        return files;
    } catch(QException* error)
    {
        logs->append(QApplication::tr("<span style=\"color: #ea7878\"><b>•</b> Error in function getAllfiles: %1</span>").arg(error->what()));
        return QStringList();
    }
}

QStringList Manager::getAllFolders(QString dir)
{
    try
    {
        QStringList folders;
        QDirIterator localIterator(dir, QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
        while (localIterator.hasNext()) { folders.append(localIterator.next()); qDebug() << localIterator.next(); }
        return folders;
    } catch(QException* error)
    {
        logs->append(QApplication::tr("<span style=\"color: #ea7878\"><b>•</b> Error in function getAllFolders: %1</span>").arg(error->what()));
        return QStringList();
    }
}

bool Manager::modInstalled(QString localDir, QString GTADir)
{
    try
    {
        QDirIterator it(localDir, QStringList() << "*", QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            QString fileName = it.next();
            QString smallFileName = fileName.split(localDir)[1].mid(1);
            QString pathgtafile = QString("%1/%2").arg(GTADir).arg(smallFileName);
            QFile gtaFile(pathgtafile);
            if(!gtaFile.exists()) return false;

            //qDebug() << smallFileName;
        }

        return true;
    } catch(QException* error)
    {
        logs->append(QApplication::tr("<span style=\"color: #ea7878\"><b>•</b> Error in function modInstalled: %1</span>").arg(error->what()));
        return false;
    }
}

bool Manager::move(QString folder, QString dir, bool remove)
{
    try
    {
        QStringList files = this->getAllFiles(folder);
        QStringList folders = this->getAllFolders(folder);

        foreach(QString file, files)
        {
            QFile rfile(QString("%1/%2").arg(folder).arg(file));
            if(!rfile.exists()) return false;

            QString nfile = QString("%1/%2").arg(dir).arg(file);
            if(QFile(nfile).exists()) { logs->append(QApplication::tr("<span style=\"color: #ea7878\"><b>•</b> File %1 already exists, replacing...</span>").arg(nfile)); QFile(nfile).remove(); }
            rfile.copy(nfile);
            logs->append(QApplication::tr("<span style=\"color: #ced0f3\"><b>•</b> File %1 moved to %2.</span>").arg(file).arg(dir));

            if(remove)
            {
                rfile.remove();
                logs->append(QApplication::tr("<span style=\"color: #ced0f3\"><b>•</b> File %1 has successfully removed.</span>").arg(file));
            }
        }

        foreach(QString path, folders)
        {
            QDir directory(path);
            //qDebug() << path;
            //qDebug() << directory.dirName();

        }
    } catch(QException* error)
    {
        logs->append(QApplication::tr("<span style=\"color: #ea7878\"><b>•</b> Error in function move: %1</span>").arg(error->what()));
        return false;
    }

    return true;
}

bool Manager::remove(QString folder, QString gta)
{
    try
    {
        QDirIterator it(folder, QStringList() << "*", QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            QString fileName = it.next();
            QString smallFileName = fileName.split(folder)[1].mid(1);
            QString pathgtafile = QString("%1/%2").arg(gta).arg(smallFileName);
            QFile gtaFile(pathgtafile);
            if(gtaFile.exists())
            {
                gtaFile.remove();
                logs->append(QApplication::tr("<span style=\"color: #ced0f3\"><b>•</b> File %1 has successfully removed.</span>").arg(gtaFile.fileName()));

                int lenght = gtaFile.fileName().split("/").count()-1;
                QString gtaFileNameCompact = gtaFile.fileName().split("/")[lenght];
                QString gtaFileLocation = gtaFile.fileName().split(gtaFileNameCompact)[0];
                gtaFileLocation = gtaFileLocation.mid(0, gtaFileLocation.length()-1);
                if(gtaFileLocation != gta)
                {
                    if(QDir(gtaFileLocation).entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries).count() == 0)
                    {
                        QDir().rmpath(gtaFileLocation);
                        logs->append(QApplication::tr("<span style=\"color: #ced0f3\"><b>•</b> <u>Empty folder</u> %1 has successfully removed.</span>").arg(gtaFileLocation));
                    }
                }
            }
        }
        return true;
    } catch(QException* error)
    {
        logs->append(QApplication::tr("<span style=\"color: #ea7878\"><b>•</b> Error in function remove: %1</span>").arg(error->what()));
        return false;
    }
    return true;
}


bool Manager::copyRecursively(const QString& sourceFolder,const QString& destFolder)
{
    QQueue< QPair<QString, QString> > queue;
    queue.enqueue(qMakePair(sourceFolder, destFolder));

    while (!queue.isEmpty())
    {
        QPair<QString, QString> pair = queue.dequeue();
        QDir sourceDir(pair.first);
        QDir destDir(pair.second);

        if(!sourceDir.exists())
          continue;

        if(!destDir.exists())
          destDir.mkpath(pair.second);

        QStringList files = sourceDir.entryList(QDir::Files);
        for(int i = 0; i < files.count(); i++)
        {
            QString srcName = pair.first + "/" + files.at(i);
            QString destName = pair.second + "/" + files.at(i);
            if(QFile(destName).exists())
            {
                QFile(destName).remove();
                logs->append(QApplication::tr("<span style=\"color: #f0f0f0\"><b>•</b> File <u>%1</u> already exists, replacing...</span>").arg(destName));
            }
            QFile::copy(srcName, destName);
            logs->append(QApplication::tr("<span style=\"color: #f0f0f0\"><b>•</b> Move file: %1</span>").arg(destName));
        }

        QStringList dirs = sourceDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
        for(int i = 0; i < dirs.count(); i++)
        {
            QString srcName = pair.first + "/" + dirs.at(i);
            QString destName = pair.second + "/" + dirs.at(i);
            queue.enqueue(qMakePair(srcName, destName));
            logs->append(QApplication::tr("<span style=\"color: #f0f0f0\"><b>•</b> Move folder: %1</span>").arg(destName));
        }
    }
    return true;
}
