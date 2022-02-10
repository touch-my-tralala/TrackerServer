#pragma once

#include <QAbstractTableModel>
#include <QTimer>

class ResourceTableModel : public QAbstractTableModel
{
    Q_OBJECT

    using TableMap = QMap<QString, QPair<QString, QString>>;

public:
    ResourceTableModel(QObject* = nullptr);

    enum Column
    {
        RES = 0,
        USER = 1,
        TIME = 2,
        LAST
    };

    //! Abstract Table Model interface
    int           rowCount(const QModelIndex& parent) const override;
    int           columnCount(const QModelIndex& parent) const override;
    QVariant      data(const QModelIndex& index, int role) const override;
    bool          setData(const QModelIndex& index, const QVariant& value, int role) override;
    QVariant      headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

private slots:
    void updateTime();

private:
    QTimer mTimer;
    typedef QHash<Column, QVariant> ResData;
    QList<ResData>                  mResours;
};


class IpTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    IpTableModel(QObject* = nullptr);

    enum Column
    {
        IP = 0,
        LAST
    };

    //! Abstract Table Model interface
    int           rowCount(const QModelIndex& parent) const override;
    int           columnCount(const QModelIndex& parent) const override;
    QVariant      data(const QModelIndex& index, int role) const override;
    bool          setData(const QModelIndex& index, const QVariant& value, int role) override;
    QVariant      headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

private:
    typedef QHash<Column, QVariant> IpData;
    QList<IpData> mIP;
};

