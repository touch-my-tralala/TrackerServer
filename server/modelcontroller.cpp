#include "modelcontroller.h"

#include <QtNetwork>

#include "resources/resourcemodel.h"

ModelController::ModelController(QSharedPointer<ResourceModel> model)
{
    mModel = model;
}

ModelController::~ModelController()
{
    mModel->unregisterObserver(this);
}

bool ModelController::init(const QVariantMap &params)
{
    auto ipList = params.value(Keys::Group().IPs, {}).value<QStringList>();
    for(const auto& ip : qAsConst(ipList))
        mAllowedIPs << ip;

    mModel->registerObserver(this);
    return true;
}

void ModelController::handle(const QVariantMap &params, QTcpSocket *sock)
{
    auto ip = sock->peerAddress().toString();
    if(!mAllowedIPs.contains(ip))
        return;

    auto action = params[mKeys.action].toString();

    if(action == mKeys.authorization)
    {
        registerClient(params, sock);
        return;
    }

    if(action == mKeys.request)
    {
        requestResource(params);
        return;
    }

    if(action == mKeys.goose)
    {
        sendGoose(params);
        return;
    }

    //TODO:: Send fail responce
}

void ModelController::removeClient(QTcpSocket *sock)
{
    for(auto i = mUsers.begin(), e = mUsers.end(); i != e; i++)
    {
        if(i.value() == sock)
        {
            mUsers.remove(i.key());
            return;
        }
    }
}

void ModelController::update(const QMap<QString, QPair<QString, QString> > &res)
{
    QVariantList resList;
    for(auto i = res.begin(), e = res.end(); i != e; i++)
    {
        resList.push_back(QVariantMap({ {mKeys.res_name, i.key()},
                                        {mKeys.usr_name, i.value().first},
                                        {mKeys.time, i.value().second}
                                      }));
    }

    QVariantMap out({ {mKeys.action, mKeys.info_update},
                      {mKeys.resources, resList}
                    });

    for(const auto& client : qAsConst(mUsers))
        sendToClient(out, client);
}

void ModelController::registerClient(const QVariantMap &params, QTcpSocket *sock)
{
    auto usrName = params.value(mKeys.name, "").toString();

    if(mUsers.contains(usrName) || usrName.size() > 1) // TODO: handle exception and responce
        return;

    mUsers[usrName] = sock;
    QVariantMap responce({
                             {mKeys.action, mKeys.authorization},
                             {mKeys.status, true}
                         });
    sendToClient(responce, sock);

    // NORM?
    mModel->notifyObservers();
}

void ModelController::requestResource(const QVariantMap &params)
{
    auto usrName = params.value(mKeys.usr_name, "").toString();
    auto resArr = params.value(mKeys.resources, {}).toList();
    auto action = params.value(mKeys.action).toString();
    auto hook = params.value(mKeys.hook, false).toBool();

    if(action == mKeys.take_res)
        takeResource(usrName, resArr, hook);

    if(action == mKeys.drop_res)
        dropResource(usrName, resArr);
}

void ModelController::sendGoose(const QVariantMap &params)
{
    auto name = params[mKeys.usr_name].toString();

    if(mUsers.contains(name))
        sendToClient(params, mUsers[name]);
}

void ModelController::takeResource(const QString &usrName, const QList<QVariant> &resArr, bool hook)
{
    QMap<QString, bool> mainResponce;
    QMap<QString, QStringList> intercepReasponce;

    for(const auto& res : qAsConst(resArr))
    {
        auto resName = res.toString();
        auto curOwner = mModel->owner(resName);
        if(curOwner == "-" || hook)
        {
            mModel->takeRes(resName, usrName);
            mainResponce[resName] = true;
            intercepReasponce[curOwner].append(resName);
        }
        else
        {
            mainResponce[resName] = false;
        }
    }

    responceResHandle(usrName, mainResponce);
    intercepResHandle(intercepReasponce);
}

void ModelController::dropResource(const QString &usrName, const QList<QVariant> &resArr)
{

    for(const auto& res : qAsConst(resArr))
    {
        auto resName = res.toString();
        auto curOwner = mModel->owner(resName);
        if(curOwner == usrName)
            mModel->dropRes(resName);
    }

    // TODO: maybe sdelat' opoveshenie
}

void ModelController::responceResHandle(const QString &usrName, const QMap<QString, bool> &val)
{
    QVariantMap out({
                        {mKeys.request, mKeys.take_res},
                        {mKeys.resources, QVariant::fromValue(val)}
                    });

    sendToClient(out, mUsers[usrName]);
}

void ModelController::intercepResHandle(const QMap<QString, QStringList> &list)
{
    for(auto i = list.begin(), e = list.end(); i != e; i++)
    {
        auto usrName = i.key();
        QVariantMap out({
                            {mKeys.action, mKeys.interception},
                            {mKeys.resources, QVariant::fromValue(i.value())}
                        });
        sendToClient(out, mUsers[usrName]);
    }
}

void ModelController::sendToClient(const QVariantMap &params, QTcpSocket *sock)
{
    if(sock->state() == QAbstractSocket::ConnectedState)
    {
        QDataStream sendStream(sock);
        sendStream.setVersion(QDataStream::Qt_5_12);
        sendStream << QJsonDocument::fromVariant(params).toJson(QJsonDocument::Compact);
    }
    else
    {
        qDebug() << "Client not connected state";
    }
}
