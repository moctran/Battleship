#ifndef BASESCREEN_H
#define BASESCREEN_H

#include <QWidget>
#include <QStackedWidget>
#include <QTcpSocket>
#include <QJsonDocument>

class baseScreen : public QWidget
{
    Q_OBJECT
public:
    explicit baseScreen(QWidget *parent = nullptr);
    QByteArray sendRequest(const QJsonObject &requestData, int timeout);
    QString token;
    void setToken(const QString &newToken); // Set token dynamically
signals:

private:
    QStackedWidget *stackedWidget;
};

#endif // BASESCREEN_H
