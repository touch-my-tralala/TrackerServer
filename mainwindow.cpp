#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>

#include "server/servercontroller.h"
#include "server/modelcontroller.h"
#include "logger/logger.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("BPOS tracker server");

    mServerCtrl = QSharedPointer<ServerController>(new ServerController);
    mModelCtrl = mServerCtrl->modelController();

    mLoggerW = new LoggerWidget;
    connect(mServerCtrl.data(), &ServerController::signalLogEvent, mLoggerW, &LoggerWidget::output);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::init()
{
    ui->loggerLayout->addWidget(mLoggerW);
    ui->resTableView->setModel(mServerCtrl->model().data());

    QString settingsPath = QDir::currentPath() + "/settings.ini";
    QFileInfo fInfo;

    if(!fInfo.exists() || !fInfo.isFile())
        mLoggerW->output("WARNING -> File settngs.ini not found on path : " + settingsPath);

    if(!mServerCtrl->init(settingsPath))
        return false;

    return true;
}

void MainWindow::on_addResBtn_clicked()
{

}


void MainWindow::on_rmvResBtn_clicked()
{

}


void MainWindow::on_addIpBtn_clicked()
{
    auto ip = ui->ipLineEdit->text();
    mModelCtrl->addIP(ip);
    ui->ipLineEdit->clear();
}


void MainWindow::on_rmvIpBtn_clicked()
{

}


void MainWindow::on_sendReportBtn_clicked()
{

}


void MainWindow::on_actChangePort_triggered()
{

}

void MainWindow::on_actionHelp_triggered()
{

}


void MainWindow::on_actMinimizeTray_changed()
{

}

