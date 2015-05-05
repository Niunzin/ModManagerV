#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->btn_tr_br->setVisible(false);
    ui->btn_tr_en->setVisible(false);
    VERSION = "2";
    APP_VERSION = "0.9.0 (Beta)";
    APP_LANG = "en";
    manager = new Manager(ui->logs);
    this->boot();
    ui->logs->append(tr("<span style=\"color: white\"><b>•</b> Looking for updates...</span>"));

    nw_versioncheck = new QNetworkAccessManager(this);
    connect(nw_versioncheck, SIGNAL(finished(QNetworkReply*)), this, SLOT(nw_versioncheck_reply(QNetworkReply*)));
    QNetworkRequest nw_version_request(QUrl("http://ifanger.com.br/modmanagerv/version"));
    nw_version_request.setRawHeader("User-Agent" , "Mozilla Firefox");
    nw_versioncheck->get(nw_version_request);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::nw_versioncheck_reply(QNetworkReply *reply)
{
    QByteArray nw_versioncheck_data = reply->readAll();
    QString nw_versioncheck_str(nw_versioncheck_data);

    if(!nw_versioncheck_str.contains('\n'))
    {
        ui->logs->append(tr("<span style=\"color: #ea7878\"><b>•</b> Failed to get data from server. - v%1</span>").arg(APP_VERSION));
        return;
    }

    if(nw_versioncheck_str.split('\n')[0] != VERSION)
    {
        ui->logs->append(tr("<span style=\"color: #ea7878\"><b>•</b> You are using an outdated version! - v%1</span>").arg(APP_VERSION));
        ui->logs->append(tr("<span style=\"color: white\"><b>•</b> %1</span>").arg(nw_versioncheck_str.split('\n')[1]));
    }
    else
        ui->logs->append(tr("<span style=\"color: white\"><b>•</b> No updates available! - v%1</span>").arg(APP_VERSION));

}

void MainWindow::unboot()
{
    lastindex = ui->list_mods->currentRow();
    mods.clear();
    ui->list_mods->clear();
}

void MainWindow::reboot()
{
    this->unboot();
    this->boot();
}

void MainWindow::boot()
{
    this->setWindowTitle(tr("Mod Manager V - v%1").arg(APP_VERSION));
    this->GTA_path = "";

    if(!QFile("MMV-conf.ini").exists())
    {
        QMessageBox::information(this, tr("Welcome!"), tr("Please, let me know your GTA V location folder."));

        QFileDialog fd;
        fd.setFileMode(QFileDialog::Directory);
        fd.setOption(QFileDialog::ShowDirsOnly);
        int result = fd.exec();
        if (result)
        {
            this->GTA_path = fd.selectedFiles()[0];

            if(QFile(QString("%1/%2").arg(GTA_path).arg("GTA5.exe")).exists())
            {
                QSettings settings("MMV-conf.ini", QSettings::IniFormat);
                settings.setValue("ModManager/gtavlocation", GTA_path);
            } else {
                QMessageBox::critical(this, tr("Error"), tr("This isn't your GTA path."));
                this->boot();
                return;
            }
        } else { return; }
    } else {
        QSettings settings("MMV-conf.ini", QSettings::IniFormat);
        GTA_path = settings.value("ModManager/gtavlocation", "").toString();

        if(GTA_path == "")
        {
            QMessageBox::critical(this, tr("Error"), tr("Failed to read GTA V location. Reload the application as administrator."));
            QFile("MMV-conf.ini").remove();
            return;
        }
    }

    mods = manager->getLocalMods();
    foreach(QString mod, mods)
    {
        QDir dir(mod);
        bool done = false;
        if(manager->modInstalled(mod, GTA_path))
            done = true;

        QString msg = QString("%1 - %2").arg(dir.dirName()).arg((done) ? "Enabled" : "Disabled");
        ui->list_mods->addItem(msg);
    }

    if(lastindex < 0) { if(ui->list_mods->count() > 0) ui->list_mods->setCurrentRow(0); } else ui->list_mods->setCurrentRow(lastindex);
}

void MainWindow::on_list_mods_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
}

void MainWindow::translate(QString language)
{
    if(language == "en")
    {
    }
}

void MainWindow::on_list_mods_currentRowChanged(int currentRow)
{
    if(currentRow >= 0)
    {
        if(currentRow > mods.count()) return;
        QString curmod = mods[currentRow];

        ui->txt_Title->setText(QDir(curmod).dirName());
        if(manager->modInstalled(curmod, GTA_path))
        {
            ui->txt_Status->setText(tr("<b>Enabled</b>"));
            ui->btn_ToggleMod->setText(tr("Disable"));
        } else {
            ui->txt_Status->setText(tr("<b>Disabled</b>"));
            ui->btn_ToggleMod->setText(tr("Enable"));
        }
    }
}

void MainWindow::on_btn_ToggleMod_clicked()
{
    if(ui->list_mods->currentRow() == -1) return;
    int currentRow = ui->list_mods->currentRow();
    if(currentRow > mods.count()) return;
    if(!manager->modInstalled(mods[ui->list_mods->currentRow()], GTA_path))
    {
        manager->copyRecursively(mods[currentRow], GTA_path);
    } else {
        manager->remove(mods[currentRow], GTA_path);
    }

    this->reboot();
}

void MainWindow::on_btn_Uninstall_clicked()
{
    if(ui->list_mods->currentRow() == -1) return;
    if(!manager->modInstalled(mods[ui->list_mods->currentRow()], GTA_path))
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Warning!"), tr("Do you really want to uninstall this mod from your machine? This action can not be reversed."),
            QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            int currentRow = ui->list_mods->currentRow();
            if(currentRow > mods.count()) return;
            QDir* ae = new QDir(mods[currentRow]);
            ae->removeRecursively();
        } else {

        }
    } else {
        QMessageBox::critical(this, tr("Error"), tr("Disable the mod before continue!"));
    }

    this->reboot();
}

void MainWindow::on_pushButton_clicked()
{
    this->reboot();
}

void MainWindow::on_btn_tr_en_clicked()
{

}
