#pragma once

#include <QJsonObject>
#include <QTcpSocket>

#include "interfaces/observer.h"
#include "keys.h"

class ResourceModel;

class ModelController :  public QObject, public Observer
{
    Q_OBJECT

public:
    ModelController(QSharedPointer<ResourceModel>);
    ~ModelController();
    bool init(const QVariantMap&);
    void handle(const QVariantMap&, QTcpSocket*);
    void removeClient(QTcpSocket*);

    void addIP(const QString &);
    void removeIP(const QString &);

    //! Observer interface
    void update(const QMap<QString, QPair<QString, QString>>&) override;

signals:
    void signalLogEvent(QString msg);

private:
    void registerClient(const QVariantMap&, QTcpSocket*);
    void requestResource(const QVariantMap&);
    void sendGoose(const QVariantMap&);

    void takeResource(const QString&, const QList<QVariant>&, bool);
    void dropResource(const QString&, const QList<QVariant>&);

    void responceResHandle(const QString&, const QMap<QString, bool>&);
    void intercepResHandle(const QMap<QString, QStringList>&);

    void sendToClient(const QVariantMap&, QTcpSocket*);

private:
    QSharedPointer<ResourceModel> mModel;
    Keys::Json mKeys;
    QStringList mAllowedIPs;
    QMap<QString, QTcpSocket*> mUsers; ///< <name, socket>
};

