#include "logger.h"

#include <QTime>

LoggerWidget::LoggerWidget()
{
    setMaximumBlockCount(10000);
    setFocusPolicy(Qt::WheelFocus);
    buildInterface();
}

void LoggerWidget::buildInterface()
{
    setStyleSheet("QPlainTextEdit{"
                                  "background-color: rgba(255, 255, 255, 1);"
                                  "color: rgba(47, 204, 10, 0.95);"
                                  //"background-image: url(:/resources/img/Console_bg.png);"
                                  "background-repeat: no-repeat;"
                                  "background-position: top center;"
                                  "font-weight: 400;"
                                  "font-style: normal;"
                                  "font-size: 13px;"
                                  "}"
                   "QMenu:disabled{"
                                  "background-color: #08a4bfa3;"
                                  "color:grey;"
                                  "}"
                   "QMenu:enabled{"
                                  "background-color: #08a4bfd6;"
                                  "color:white;"
                                  "}");
    setReadOnly(true);
}

void LoggerWidget::output(const QString &msg)
{
    QString font_color = "<font color=#6495ED>"; //blue
    QString font_end = "</font>";

    if(msg.contains("ERROR", Qt::CaseInsensitive))
        font_color =  "<font color=#d00c0c>"; //red

    if(msg.contains("WARNING", Qt::CaseInsensitive))
        font_color =  "<font color=#FFFF00>"; //yellow

    appendHtml(font_color + QTime::currentTime().toString("HH:mm:ss.  ") + msg + font_end);
}
