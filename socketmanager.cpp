// SocketManager.cpp
#include "SocketManager.h"
#include <QDebug>
SocketManager* SocketManager::instance = nullptr;

SocketManager::SocketManager() {
    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::connected, this, &SocketManager::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &SocketManager::onDisconnected);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
            this, &SocketManager::onError);
}

SocketManager::~SocketManager() {
    delete socket;
}

SocketManager* SocketManager::getInstance() {
    if (!instance) {
        instance = new SocketManager();
    }
    return instance;
}

QTcpSocket* SocketManager::getSocket() {
    return socket;
}

void SocketManager::connectToServer(const QString& host, quint16 port) {
    if (socket->state() == QTcpSocket::UnconnectedState) {
        socket->connectToHost(host, port);
    }
}

void SocketManager::disconnectFromServer() {
    if (socket->state() != QTcpSocket::UnconnectedState) {
        socket->disconnectFromHost();
    }
}

void SocketManager::onConnected() {
    emit connectionEstablished();
}

void SocketManager::onDisconnected() {
    emit connectionLost();
}

void SocketManager::onError(QAbstractSocket::SocketError socketError) {
    emit errorOccurred(socket->errorString());
}



void SocketManager::printSocketInfo() const {
    if (!socket) {
        qDebug() << "Socket is not initialized.";
        return;
    }

    qDebug() << "Socket Information:";
    qDebug() << "  State:" << socket->state();
    qDebug() << "  Peer Address:" << (socket->peerAddress().toString().isEmpty() ? "Not connected" : socket->peerAddress().toString());
    qDebug() << "  Peer Port:" << (socket->peerPort() == 0 ? "Not connected" : QString::number(socket->peerPort()));
    qDebug() << "  Local Port:" << socket->localPort();
    qDebug() << "  Connected:" << (socket->isOpen() ? "Yes" : "No");
    // Print error information if any
    if (socket->error() != QAbstractSocket::UnknownSocketError) {
        qDebug() << "  Last Error:" << socket->errorString();
    } else {
        qDebug() << "  Last Error: None";
    }
}
