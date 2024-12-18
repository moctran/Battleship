// SocketManager.h
#ifndef SOCKETMANAGER_H
#define SOCKETMANAGER_H

#include <QTcpSocket>
#include <QObject>
#include <QString>

class SocketManager : public QObject {
    Q_OBJECT

public:
    static SocketManager* getInstance();
    QTcpSocket* getSocket();

    void connectToServer(const QString& host, quint16 port);
    void disconnectFromServer();
    void printSocketInfo() const;

signals:
    void connectionEstablished();
    void connectionLost();
    void errorOccurred(const QString& error);

private:
    SocketManager();
    ~SocketManager();
    static SocketManager* instance;
    QTcpSocket* socket;

private slots:
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError socketError);
};

#endif // SOCKETMANAGER_H
