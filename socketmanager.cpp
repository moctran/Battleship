#include "SocketManager.h"
#include <QDebug>

SocketManager* SocketManager::instance = nullptr;

SocketManager::SocketManager() {
    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::connected, this, &SocketManager::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &SocketManager::onDisconnected);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
            this, &SocketManager::onError);
    connect(socket, &QTcpSocket::readyRead, this, &SocketManager::onReadyRead); // Listen for incoming data
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
        // Wait for connection
        if (!socket->waitForConnected(60000)) { // 5000 ms timeout
            qDebug() << "Failed to connect to server:" << socket->errorString();
            emit errorOccurred("Failed to connect to server: " + socket->errorString());
        } else {
            qDebug() << "Successfully connected to server.";
        }
    }

}

void SocketManager::disconnectFromServer() {
    if (socket->state() != QTcpSocket::UnconnectedState) {
        socket->disconnectFromHost();
    }
}

void SocketManager::onConnected() {
    qDebug() << "Socket connected to server.";
    emit connectionEstablished();
}

void SocketManager::onDisconnected() {
    qDebug() << "Socket disconnected from server.";
    emit connectionLost();
}

void SocketManager::onError(QAbstractSocket::SocketError socketError) {
    qDebug() << "Socket error:" << socket->errorString();
    emit errorOccurred(socket->errorString());
}

void SocketManager::onReadyRead() {
    if (!socket) {
        qDebug() << "Socket is not initialized.";
        return;
    }

    QByteArray message = socket->readAll(); // Read all available data
    qDebug() << "Message received from server:" << message;

    emit messageReceived(message); // Emit the signal to notify listeners
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

