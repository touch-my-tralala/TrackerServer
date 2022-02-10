#pragma once

#include <QObject>
#include <QTcpServer>
#include <QAbstractTableModel>


#include "keys.h"

class ModelController;

class ServerController : public QObject
{
    Q_OBJECT
public:
    ServerController();
    ~ServerController();
    //! \param[path] - absolute path to sett.ini
    bool init(const QString &path);
    QSharedPointer<QAbstractTableModel> model();
    QSharedPointer<ModelController> modelController();

signals:
    void signalLogEvent(QString msg); // mozhno li peredavati ssilku?

private:
    QVariantMap readSettings(const QString&);
    void saveSettings();

private slots:
    void slotNewConnection();
    void slotDisconnected();
    void slotReadyRead();

private:
    quint32 mDataSize;
    QString mStartDateTime;
    QString mSettingsPath;
    QSharedPointer<class ResourceModel> mModel;
    QSharedPointer<ModelController> mModelController;
    QTcpServer mServer;
    QByteArray mBuff;
};

