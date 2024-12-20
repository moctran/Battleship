#include "joingameroom.h"
#include "qjsondocument.h"
#include "creategameroom.h"
#include <QMessageBox>

extern QString globalUserToken;

JoinGameRoom::JoinGameRoom(QStackedWidget *stackedWidget, QWidget *parent)
    : QWidget(parent), stackedWidget(stackedWidget) {

    roomNumberInput = new QLineEdit(this);
    roomNumberInput->setPlaceholderText("Enter Room Number");

    joinRoomButton = new QPushButton("Join Room", this);
    backButton = new QPushButton("Back", this);

    layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Join Game Room", this));
    layout->addWidget(roomNumberInput);
    layout->addWidget(joinRoomButton);
    layout->addWidget(backButton);
    setLayout(layout);

    connect(joinRoomButton, &QPushButton::clicked, this, &JoinGameRoom::onJoinRoomClicked);
    connect(backButton, &QPushButton::clicked, this, &JoinGameRoom::onBackClicked);
}

void JoinGameRoom::onJoinRoomClicked() {
    QString roomNumber = roomNumberInput->text();
    if (roomNumber.isEmpty()) {
        QMessageBox::warning(this, "Join Game Room", "Please enter a room number.");
        return;
    }

    // Create the JSON message
    QJsonObject requestJson;
    requestJson["type"] = "join_room";
    requestJson["token"] = token;
    requestJson["room_id"] = roomNumber;

    QJsonDocument requestDoc(requestJson);
    QByteArray requestData = requestDoc.toJson();

    // Set up the socket connection
    QTcpSocket socket;
    socket.connectToHost("127.0.0.1", 8080);

    if (!socket.waitForConnected(3000)) {
        QMessageBox::critical(this, "Connection Error", "Failed to connect to the server.");
        return;
    }

    // Send the request
    socket.write(requestData);
    if (!socket.waitForBytesWritten(3000)) {
        QMessageBox::critical(this, "Error", "Failed to send data to the server.");
        return;
    }

    // Wait for the server's response
    if (!socket.waitForReadyRead(3000)) {
        QMessageBox::critical(this, "Error", "No response from the server.");
        return;
    }

    QByteArray responseData = socket.readAll();
    qDebug() << "Server response for leave_room:" << responseData;

    QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
    QJsonObject responseObj = responseDoc.object();
    qDebug() << responseObj["status"].toBool();
    if(responseObj["status"].toString() == "error") {
        QMessageBox::critical(this, "Error", "Failed to join room");
    } else {
        qDebug() << responseObj["firstPlayerId"].toString();
        qDebug() << responseObj["secondPlayerId"].toString();
        redirect(responseObj.value("firstPlayerId").toString(),
                 responseObj.value("secondPlayerId").toString(),
                 responseObj.value("id").toString());

    }
    // Clear input field after submission
    roomNumberInput->clear();
}

void JoinGameRoom::redirect(QString firstPlayerId, QString secondPlayerId, QString roomID) {
    CreateGameRoom *createGameRoom = dynamic_cast<CreateGameRoom *>(stackedWidget->widget(5)); // Index 5
    if (createGameRoom) {
        createGameRoom->setToken(globalUserToken); // Pass the token dynamically
        createGameRoom->setRoomID(roomID);
        createGameRoom->displayRoomID();
        createGameRoom->updateLabels(firstPlayerId, secondPlayerId);
        // Navigate to Create Game Room Screen only if RoomID generation succeeds
        stackedWidget->setCurrentIndex(5);
    }
}

void JoinGameRoom::onBackClicked() {
    stackedWidget->setCurrentIndex(3); // Navigate back to Home Screen
}

void JoinGameRoom::setToken(const QString &newToken) {
    token = newToken;
    qDebug() << "Token set in JoinGameScreen:" << token;
}
