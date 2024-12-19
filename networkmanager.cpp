#include "networkmanager.h"
#include <QDebug>
#include <QJsonDocument>

NetworkManager::NetworkManager(QObject *parent) : QObject(parent) {
    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::readyRead, this, &NetworkManager::onReadyRead);
    connect(socket, &QTcpSocket::connected, this, &NetworkManager::onConnected);
    connect(socket, &QTcpSocket::errorOccurred, this, &NetworkManager::onErrorOccurred);
}

void NetworkManager::connectToServer(const QString &host, quint16 port) {
    socket->connectToHost(host, port);
}

void NetworkManager::sendMessage(const QJsonObject &message) {
    if (socket->state() == QAbstractSocket::ConnectedState) {
        QJsonDocument doc(message);
        QByteArray data = doc.toJson();
        socket->write(data);
        socket->flush();
    } else {
        qDebug() << "Not connected to server.";
    }
}

void NetworkManager::onReadyRead() {
    QByteArray data = socket->readAll();
    emit dataReceived(QString::fromUtf8(data));
}

void NetworkManager::onConnected() {
    qDebug() << "Connected to server.";
    emit connectionEstablished();
}

void NetworkManager::onErrorOccurred(QAbstractSocket::SocketError socketError) {
    qDebug() << "Connection error:" << socketError;
    emit connectionError(socket->errorString());
}
