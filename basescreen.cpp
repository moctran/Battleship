#include "basescreen.h"

baseScreen::baseScreen(QWidget *parent)
    : QWidget{parent}
{}

QByteArray baseScreen::sendRequest(const QJsonObject &requestData, int timeout) {
    QTcpSocket tempSocket;
    tempSocket.connectToHost("172.16.183.145", 8080);

    if (!tempSocket.waitForConnected(timeout)) {
        qDebug() << "Failed to connect to the server.";
        return QByteArray();
    }

    // Send the request data
    QJsonDocument requestDoc(requestData);
    QByteArray requestDataBytes = requestDoc.toJson(QJsonDocument::Compact);
    tempSocket.write(requestDataBytes);

    if (!tempSocket.waitForBytesWritten(timeout)) {
        qDebug() << "Failed to write data to the server.";
        return QByteArray();
    }

    // Wait for the response
    if (!tempSocket.waitForReadyRead(timeout)) {
        qDebug() << "No response from the server within timeout.";
        return QByteArray();
    }

    QByteArray responseData = tempSocket.readAll();
    tempSocket.close();
    return responseData;
}

void baseScreen::setToken(const QString &newToken) {
    token = newToken;
    qDebug() << "Token set:" << token;
}

void baseScreen::setId(const QString &newId) {
    userId = newId;
    qDebug() << "User ID set:" << userId;
}

void baseScreen::setName(const QString &newName) {
    userName = newName;
    qDebug() << "User name set:" << userName;
}
