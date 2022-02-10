#include "tablemodel.h"

#include <QTime>
#include "keys.h"

using modelKeys = Keys::Model;

ResourceTableModel::ResourceTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    connect(&mTimer, &QTimer::timeout, this, &ResourceTableModel::updateTime);
    mTimer.setInterval(1000);
    mTimer.start();
}

int ResourceTableModel::rowCount(const QModelIndex &) const
{
    return mResours.size();
}

int ResourceTableModel::columnCount(const QModelIndex &) const
{
    return LAST;
}

QVariant ResourceTableModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid() && mResours.size() > index.row())
    {
        if(role == Qt::TextAlignmentRole)
            return Qt::AlignCenter;

        if(role == Qt::DisplayRole)
            return mResours[index.row()][Column(index.column())];
    }
    return {};
}

bool ResourceTableModel::setData(const QModelIndex &index, const QVariant &value, int)
{
    if(!index.isValid() || mResours.size() <= index.row())
        return false;

    // FIXME: Check this
    mResours[index.row()][Column(index.column())] = value;
    emit dataChanged(index, index);

    return true;
}

QVariant ResourceTableModel::headerData(int column, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return {};

    if(orientation == Qt::Vertical)
        return column;

    switch (column)
    {
        case RES:
            return "Resourse";
        case USER:
            return "Owner";
        case TIME:
            return "Used time";
    }

    return {};
}

Qt::ItemFlags ResourceTableModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return {};

    // FIXME: Check this flags. maybe need change it
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

// FIXME: I don't know how to do it normally
void ResourceTableModel::updateTime()
{
    int idx = 0;
    for(const auto& i : qAsConst(mResours))
    {
        if(i[USER].toString() == "-")
            continue;

        const auto& curTime = i[TIME].toString();
        auto h = curTime.leftRef(2).toInt();
        auto m = curTime.midRef(3,2).toInt();
        auto s = curTime.rightRef(2).toInt();
        auto newTime = QTime(h, m, s).addSecs(1).toString("hh:mm:ss");
        setData(QAbstractItemModel::createIndex(idx, TIME), newTime, Qt::DisplayRole);
        idx++;
    }
}




IpTableModel::IpTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int IpTableModel::rowCount(const QModelIndex &) const
{
    return mIP.size();
}

int IpTableModel::columnCount(const QModelIndex &) const
{
    return LAST;
}

QVariant IpTableModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid() && mIP.size() > index.row())
    {
        if(role == Qt::TextAlignmentRole)
            return Qt::AlignCenter;

        if(role == Qt::DisplayRole)
            return mIP[index.row()][Column(index.column())];
    }

    return {};
}

bool IpTableModel::setData(const QModelIndex &index, const QVariant &value, int)
{
    if(!index.isValid() || mIP.size() <= index.row())
        return false;

    // FIXME: Check this
    mIP[index.row()][Column(index.column())] = value;
    emit dataChanged(index, index);

    return true;
}

QVariant IpTableModel::headerData(int column, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return {};

    if(orientation == Qt::Vertical)
        return column;

    switch (column)
    {
        case IP:
            return "IP Address";
    }

    return {};
}

Qt::ItemFlags IpTableModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return {};

    // FIXME: Check this flags. maybe need change it
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


