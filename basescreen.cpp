#include "basescreen.h"

baseScreen::baseScreen(QWidget *parent)
    : QWidget{parent}
{}

QByteArray baseScreen::sendRequest(const QJsonObject &requestData, int timeout) {
    QTcpSocket tempSocket;
    tempSocket.connectToHost("127.0.0.1", 8080);

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
