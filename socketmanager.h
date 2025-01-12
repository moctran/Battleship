#ifndef SOCKETMANAGER_H
#define SOCKETMANAGER_H

#include <QTcpSocket>
#include <QObject>
#include <QString>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

class SocketManager : public QObject {
    Q_OBJECT

public:
    static SocketManager* getInstance();
    QTcpSocket* getSocket();

    void connectToServer(const QString& host, quint16 port);
    void disconnectFromServer();
    void printSocketInfo() const;
    void closeConnection();

signals:
    void connectionEstablished();
    void connectionLost();
    void errorOccurred(const QString& error);
    void messageReceived(const QByteArray& message); // Signal for received messages

private:
    SocketManager();
    ~SocketManager();
    static SocketManager* instance;
    QTcpSocket* socket;

private slots:
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError socketError);
    void onReadyRead(); // Slot to handle incoming messages
};

#endif // SOCKETMANAGER_H
