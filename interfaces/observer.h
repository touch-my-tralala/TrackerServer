#pragma once

#include <QMap>
#include <QPair>
#include <QTime>


class Observer
{
public:
    virtual ~Observer() = default;
    virtual void update(const QMap<QString, QPair<QString, QString>>&) = 0;
};


class Observable
{
public:
    virtual void registerObserver(Observer*) = 0;
    virtual void unregisterObserver(Observer*) = 0;
    virtual void notifyObservers() = 0;
};
