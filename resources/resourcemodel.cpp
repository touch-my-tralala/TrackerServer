#include "resourcemodel.h"

#include <QSettings>
#include <QSharedPointer>
#include <QAbstractTableModel>

#include "keys.h"

using modelKeys = Keys::Model;

ResourceModel::ResourceModel()
{
    mModel = QSharedPointer<ResourceTableModel>(new ResourceTableModel);
}

bool ResourceModel::init(const QVariantMap &params)
{
    auto resList = params.value(Keys::Group().resources, {}).value<QStringList>();    
    mModel->insertRows(0, resList.count());

    int idx = 0;
    for(const auto& res : qAsConst(resList))
    {
        mModel->setData(QAbstractItemModel::createIndex(idx, ResourceTableModel::RES));
        mModel->setData(res);
    }


    mSettingsPath = params.value(Keys::Config().sattings_path, "resources.ini").toString();
    return true;
}

void ResourceModel::registerObserver(Observer *obs)
{
    if(!mObservers.contains(obs))
        mObservers.append(obs);
}

void ResourceModel::unregisterObserver(Observer *obs)
{
    mObservers.removeOne(obs);
}

void ResourceModel::notifyObservers()
{

}

bool ResourceModel::addElement(const QVariantMap &map)
{
    if(!map.contains(modelKeys().resName) || hasAlready(map[modelKeys().resName]))
        return false;
}

bool ResourceModel::rmvElement(const QVariantMap &)
{

}

bool ResourceModel::setElement(const QVariantMap &)
{

}

QVariantMap ResourceModel::elementInfo(const QVariantMap &) const
{

}

QAbstractTableModel *ResourceModel::qtModel()
{

}

QString ResourceModel::owner(const QString &resName)
{
    return mModel->owner(resName);
}

bool ResourceModel::takeRes(const QString &resName, const QString &usrName)
{
    bool ok = mModel->setOwner(resName, usrName);
    notifyObservers();
    return ok;
}

bool ResourceModel::dropRes(const QString &resName)
{
    bool ok = mModel->setOwner(resName, "-");
    notifyObservers();
    return ok;
}

bool ResourceModel::addRes(const QString &name)
{
    bool ok = mModel->addResource(name);
    notifyObservers();

    if(ok)
    {
        saveSattings();
        emit("INFO -> A new resource named " + name + " has been added.");
    }
    else
    {
        emit("WARNING -> A new resource named " + name + " has't been added.");
    }

    return ok;
}

bool ResourceModel::removeRes(const QString &name)
{
    bool ok = mModel->removeResource(name);
    notifyObservers();

    if(ok)
    {
        saveSattings();
        emit("INFO -> A new resource named " + name + " has been removed.");
    }
    else
    {
        emit("WARNING -> A new resource named " + name + " has't been removed.");
    }

    return ok;
}

QSharedPointer<ResourceTableModel> ResourceModel::model() const
{
    return mModel;
}

bool ResourceModel::hasAlready(const QVariant &) const
{

}

void ResourceModel::saveSattings()
{
    auto resMap = mModel->fullData();
    QSettings sett(mSettingsPath, QSettings::IniFormat);

    sett.beginWriteArray(Keys::Group().resources);
    quint32 idx = 0;
    for(auto i = resMap.begin(), e = resMap.end(); i != e; ++i)
    {
        sett.setArrayIndex(idx);
        sett.setValue("res", i.key());
        ++idx;
    }
    sett.endArray();
}



IpModel::IpModel()
{
    mModel = QSharedPointer<IpTableModel>(new IpTableModel);
}

bool IpModel::init(const QVariantMap &params)
{
    auto ipList = params.value(Keys::Group().IPs, {}).value<QStringList>();
    for(const auto& ip : qAsConst(ipList))
        mModel->addIP(ip);

    mSettingsPath = params.value(Keys::Config().sattings_path, "resources.ini").toString();
    return true;
}

void IpModel::registerObserver(Observer *obs)
{
    if(!mObservers.contains(obs))
        mObservers.append(obs);
}

void IpModel::unregisterObserver(Observer *obs)
{
    mObservers.removeOne(obs);
}

void IpModel::notifyObservers()
{
    for(const auto& observer : qAsConst(mObservers))
        observer->update(mModel->fullData());
}

bool IpModel::addIP(const QString &)
{

}

bool IpModel::removeIP(const QString &)
{

}

QSharedPointer<IpTableModel> IpModel::model() const
{

}

void IpModel::saveSattings()
{

}


