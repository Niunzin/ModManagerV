#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "manager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QString APP_VERSION;
    QString APP_LANG;
    QString VERSION;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Manager *manager;
    QString GTA_path;
    QStringList mods;
    int lastindex;
    QNetworkAccessManager *nw_versioncheck;

private slots:
    void boot();
    void unboot();
    void reboot();
    void on_list_mods_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void on_list_mods_currentRowChanged(int currentRow);
    void on_btn_ToggleMod_clicked();
    void on_btn_Uninstall_clicked();
    void on_pushButton_clicked();
    void nw_versioncheck_reply(QNetworkReply *reply);
    void on_btn_tr_en_clicked();
    void translate(QString language);
};

#endif // MAINWINDOW_H
