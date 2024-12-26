#include "joingameroom.h"
#include "qjsondocument.h"
#include "creategameroom.h"
#include <QMessageBox>

extern QString globalUserToken;

JoinGameRoom::JoinGameRoom(QStackedWidget *stackedWidget, QWidget *parent)
    : baseScreen(parent), stackedWidget(stackedWidget) {

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

    QByteArray responseData = sendRequest(requestJson, 60000);
    qDebug() << "Server response for leave_room:" << responseData;

    QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
    QJsonObject responseObj = responseDoc.object();

    // Check the status field
    if (responseObj["status"].toString() == "error") {
        QMessageBox::critical(this, "Error", "Failed to join room");
    } else {
        QJsonObject dataObj = responseObj["data"].toObject(); // Extract the nested "data" object

        // Access fields within "data"
        QString firstPlayerId = dataObj.value("firstPlayerId").toString();
        QString secondPlayerId = dataObj.value("secondPlayerId").toString();
        QString roomId = dataObj.value("id").toString();

        qDebug() << "First Player ID:" << firstPlayerId;
        qDebug() << "Second Player ID:" << secondPlayerId;
        qDebug() << "Room ID:" << roomId;

        // Redirect using extracted values
        redirect(firstPlayerId, secondPlayerId, roomId);
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
