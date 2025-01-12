// BaseGameScreen.cpp
#include "basegamescreen.h"
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QDebug>

BaseGameScreen::BaseGameScreen(QStackedWidget *stackedWidget, QWidget *parent)
    : baseScreen(parent) {}

void BaseGameScreen::leaveRoom(const QString &token, QStackedWidget *stackedWidget) {
    QTcpSocket socket;
    socket.connectToHost("127.0.0.1", 8080);

    if (!socket.waitForConnected(60000)) {
        QMessageBox::critical(this, "Connection Error", "Failed to connect to the server.");
        return;
    }

    // Prepare the request JSON
    qDebug() << "Using token for leave_room request:" << token;

    QJsonObject requestJson;
    requestJson["type"] = "leave_room";
    requestJson["token"] = token;

    QJsonDocument requestDoc(requestJson);
    QByteArray requestData = requestDoc.toJson();
    socket.write(requestData);

    // Wait for the server to acknowledge the request
    if (!socket.waitForBytesWritten(60000)) {
        QMessageBox::critical(this, "Error", "Failed to send data to the server.");
        return;
    }

    // Wait for the server's response
    if (!socket.waitForReadyRead(60000)) {
        QMessageBox::critical(this, "Error", "No response from the server.");
        return;
    }

    // Process the response
    QByteArray responseData = socket.readAll();
    qDebug() << "Server response for leave_room:" << responseData;

    QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
    QJsonObject responseObj = responseDoc.object();

    if (responseObj["status"].toString() == "success") {
        stackedWidget->setCurrentIndex(3); // Go back to the previous screen
    } else {
        QMessageBox::critical(this, "Error", responseObj["message"].toString());
    }
}

QString BaseGameScreen::getColour(int index) {
    switch(index) {
    case 1:
        return "red";
        break;
    case 2:
        return "orange";
        break;
    case 3:
        return "yellow";
        break;
    case 4:
        return "green";
        break;
    case 5:
        return "blue";
        break;
    default:
        return "lightblue";
        break;
    }
    return QString();
}

void BaseGameScreen::setFirstPlayerStatus(bool check) {
    isFirstPlayer = check;
    qDebug() << "First player?" << isFirstPlayer;
}
