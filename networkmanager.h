#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QObject>

class NetworkManager : public QObject {
    Q_OBJECT

private:
    QTcpSocket *socket;

public:
    explicit NetworkManager(QObject *parent = nullptr);

    void connectToServer(const QString &host, quint16 port);
    void sendMessage(const QJsonObject &message);

signals:
    void dataReceived(const QString &data);
    void connectionEstablished();
    void connectionError(const QString &error);

private slots:
    void onReadyRead();
    void onConnected();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);
};

#endif // NETWORKMANAGER_H
