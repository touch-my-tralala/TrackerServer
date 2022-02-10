#pragma once

#include <QPlainTextEdit>

class LoggerWidget : public QPlainTextEdit
{
    Q_OBJECT
public:
    LoggerWidget();
    void buildInterface();
    void output(const QString &);
};

