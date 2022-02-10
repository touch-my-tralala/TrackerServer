#pragma once

#include <QList>
#include <QTime>
#include <QSharedPointer>

#include "interfaces/observer.h"
#include "resources/tablemodel.h"

class ITableModel : public Observable
{
public:
    virtual bool addElement(const QVariantMap &) = 0;
    virtual bool rmvElement(const QVariantMap &) = 0;
    virtual bool setElement(const QVariantMap &) = 0;
    virtual QVariantMap elementInfo(const QVariantMap&)const = 0;
    virtual QAbstractTableModel* qtModel() = 0;
};


class ResourceModel : public QObject,
                      public ITableModel
{
    Q_OBJECT

public:
    ResourceModel();

    bool init(const QVariantMap &);
    //! Observable interface
    void registerObserver(Observer*) override;
    void unregisterObserver(Observer*) override;
    void notifyObservers() override;

    //! ITableModel interface
    bool addElement(const QVariantMap &) override;
    bool rmvElement(const QVariantMap &) override;
    bool setElement(const QVariantMap &) override;
    QVariantMap elementInfo(const QVariantMap&) const override;
    QAbstractTableModel* qtModel() override;

signals:
    void signalLogEvent(QString msg);

private:
    bool hasAlready(const QVariant &) const;
    void saveSattings();

private:
    QString mSettingsPath;
    QSharedPointer<ResourceTableModel> mModel;
    QList<Observer*> mObservers;
};


class IpModel : public QObject, public Observable
{
    Q_OBJECT

public:
    IpModel();
    bool init(const QVariantMap &);
    //! Observable interface
    void registerObserver(Observer*) override;
    void unregisterObserver(Observer*) override;
    void notifyObservers() override;

    bool addIP(const QString &);
    bool removeIP(const QString &);

    QSharedPointer<IpTableModel> model() const;

signals:
    void signalLogEvent(QString msg);

private:
    void saveSattings();

private:
    QString mSettingsPath;
    QSharedPointer<IpTableModel> mModel;
    QList<Observer*> mObservers;
};
