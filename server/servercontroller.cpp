#include "servercontroller.h"

#include <QtCore>
#include <QtNetwork>

#include "modelcontroller.h"
#include "resources/resourcemodel.h"

ServerController::ServerController()
{
    mModel = QSharedPointer<ResourceModel>(new ResourceModel);
    mModelController = QSharedPointer<ModelController>(new ModelController(mModel));

    connect(mModel.data(), &ResourceModel::signalLogEvent, this, &ServerController::signalLogEvent);
    connect(mModelController.data(), &ModelController::signalLogEvent, this, &ServerController::signalLogEvent);
}

ServerController::~ServerController()
{
    saveSettings();
}

bool ServerController::init(const QString &path)
{
    mSettingsPath = path;
    auto params = readSettings(path);

    if(!mModelController->init(params))
        return false;

    if(!mModel->init(params))
        return false;

    mServer.setMaxPendingConnections(params.value(Keys::Config().max_user).toInt());

    auto port = params.value(Keys::Config().port, 9999).toInt();
    if(!mServer.listen(QHostAddress::Any, port))
    {
        emit signalLogEvent("ERR -> Server not listening");
        return false;
    }

    connect(&mServer, &QTcpServer::newConnection, this, &ServerController::slotNewConnection);

    mStartDateTime = QDateTime::currentDateTime().toString("dd.MM.yyy hh:mm:ss");

    emit signalLogEvent("INFO -> server port : " + QString::number(port));
    emit signalLogEvent("INFO -> server start time : " + mStartDateTime);

    return true;
}

QSharedPointer<QAbstractTableModel> ServerController::model()
{
    return qSharedPointerCast<QAbstractTableModel>(mModel->model());
}

QSharedPointer<ModelController> ServerController::modelController()
{
    return mModelController;
}

QVariantMap ServerController::readSettings(const QString &path)
{
    QFileInfo finfo(path);

    if(!finfo.exists() || !finfo.isFile())
        return {};

    QSettings sett(path, QSettings::IniFormat);
    sett.setIniCodec("UTF-8");

    QVariantMap out;
    sett.beginGroup(Keys::Group().server);
    out.insert(Keys::Config().port, sett.value(Keys::Config().port, 9999));
    out.insert(Keys::Config().max_user, sett.value(Keys::Config().max_user, 10));
    sett.endGroup();

    // TODO: change to beginReadArray();
    QStringList resList;
    sett.beginGroup(Keys::Group().resources);
    auto iniList = sett.childKeys();
    for(const auto& i : qAsConst(iniList))
    {
        auto name = sett.value(i, "no_data").toString().toLower();
        resList << name;
    }
    sett.endGroup();
    out.insert(Keys::Group().resources, resList);

    QStringList ipList;
    sett.beginGroup(Keys::Group().IPs);
    iniList = sett.childKeys();
    for(const auto& i : qAsConst(iniList))
    {
        auto ip = sett.value(i, "").toString().toLower();
        if(ip.size() > 0)
            ipList << ip;
    }
    sett.endGroup();
    out.insert(Keys::Group().IPs, ipList);

    out.insert(Keys::Config().sattings_path, path);

    return out;
}

void ServerController::saveSettings()
{
    QSettings sett(mSettingsPath, QSettings::IniFormat);
    sett.beginGroup(Keys::Group().server);
    sett.setValue(Keys::Config().port, mServer.serverPort());
    sett.setValue(Keys::Config().max_user, mServer.maxPendingConnections());
    sett.endGroup();
}

void ServerController::slotNewConnection()
{
    QTcpSocket* clientSocket = mServer.nextPendingConnection();

    connect(clientSocket, &QTcpSocket::disconnected, this, &ServerController::slotDisconnected);
    connect(clientSocket, &QTcpSocket::readyRead, this, &ServerController::slotReadyRead);

}

void ServerController::slotDisconnected()
{
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    mModelController->removeClient(clientSocket);

    if (clientSocket != nullptr)
    {
        clientSocket->abort();
        clientSocket->deleteLater();
    }
}

void ServerController::slotReadyRead()
{
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    QDataStream readStream(clientSocket);
    readStream.setVersion(QDataStream::Qt_5_12);

    while (!readStream.atEnd())
    {
        if (!mDataSize)
        {
            qint32 header_size = sizeof(quint32);
            if (clientSocket->bytesAvailable() < header_size)
                return;
            readStream >> mDataSize;
        }

        if (clientSocket->bytesAvailable() < mDataSize)
            return;

        quint8 byte;
        for (quint32 i = 0; i < mDataSize; i++)
        {
            readStream >> byte;
            mBuff.append(byte);
        }

        QJsonParseError jsonErr;
        auto jDoc = QJsonDocument::fromJson(mBuff, &jsonErr);
        if (jsonErr.error == QJsonParseError::NoError)
            mModelController->handle(jDoc.object().toVariantMap(), clientSocket);
        else
            emit signalLogEvent("ERR -> Json error : " + jsonErr.errorString());

        mBuff.clear();
        mDataSize = 0;
    }
}
