#pragma once

#include <QMainWindow>
#include <QSharedPointer>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class LoggerWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool init();

private slots:
    void on_addResBtn_clicked();
    void on_rmvResBtn_clicked();
    void on_addIpBtn_clicked();
    void on_rmvIpBtn_clicked();
    void on_sendReportBtn_clicked();
    void on_actChangePort_triggered();
    void on_actionHelp_triggered();
    void on_actMinimizeTray_changed(); // CHECK this

private:
    QSharedPointer<class ServerController> mServerCtrl;
    QSharedPointer<class ModelController> mModelCtrl;
    LoggerWidget* mLoggerW;
    Ui::MainWindow *ui;
};
