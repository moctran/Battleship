#include "creategameroom.h"
#include "battleshipboard.h"
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

extern QString globalUserToken;

CreateGameRoom::CreateGameRoom(QStackedWidget *stackedWidget, QWidget *parent)
    : QWidget(parent), stackedWidget(stackedWidget) {

    // Left side (Player Info and Buttons)
    player1Label = new QLabel("My Name", this);
    player2Label = new QLabel("Opponent Name (hardcoded)", this);
    startGameButton = new QPushButton("Start Game", this);
    backButton = new QPushButton("Back", this);

    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addWidget(new QLabel("Players", this));
    leftLayout->addWidget(player1Label);
    leftLayout->addWidget(new QLabel("VS", this));
    leftLayout->addWidget(player2Label);
    leftLayout->addWidget(startGameButton);
    leftLayout->addWidget(backButton);

    // Right side (Room ID and Online Players)
    roomIDLabel = new QLabel("Room ID: ", this);

    onlinePlayersList = new QListWidget(this);

    populateOnlinePlayers();

    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->addWidget(roomIDLabel);
    rightLayout->addWidget(new QLabel("Online Players", this));
    rightLayout->addWidget(onlinePlayersList);

    // Combine left and right side
    QHBoxLayout *screenLayout = new QHBoxLayout();
    screenLayout->addLayout(leftLayout);
    screenLayout->addLayout(rightLayout);

    setLayout(screenLayout);

    // Connect buttons
    connect(startGameButton, &QPushButton::clicked, this, &CreateGameRoom::onStartGameClicked);
    connect(backButton, &QPushButton::clicked, this, &CreateGameRoom::onBackClicked);
}

// Redirect to set up ship screen
void CreateGameRoom::onStartGameClicked() {
    QMessageBox::information(this, "Game preparation", "Redirect to ship set up screen...");

    BattleshipBoard *setupBoard = dynamic_cast<BattleshipBoard *>(stackedWidget->widget(7)); // Index 5
    if (setupBoard) {
        setupBoard->setToken(globalUserToken); // Pass the token dynamically
    }

    stackedWidget->setCurrentIndex(7);
}

void CreateGameRoom::setToken(const QString &newToken) {
    token = newToken;
    qDebug() << "Token set in CreateGameScreen:" << token;
}

void CreateGameRoom::setRoomID(const QString &roomId) {
    roomID = roomId;
    qDebug() << "RoomID set in CreateGameScreen:" << roomID;
}

void CreateGameRoom::onBackClicked() {
    stackedWidget->setCurrentIndex(3); // Navigate back to Home Screen
}

void CreateGameRoom::onSendInviteClicked() {
    QMessageBox::information(this, "Send Invite", "Invite sent to the selected player!");
}

void CreateGameRoom::populateOnlinePlayers() {
    // Clear any existing items in the list
    onlinePlayersList->clear();

    // Hardcoded list of online players
    QStringList players = {"Player1", "Player2", "Player3", "Player4"};

    for (const QString &player : players) {
        // Create a custom widget for each player
        QWidget *itemWidget = new QWidget(onlinePlayersList);
        QHBoxLayout *itemLayout = new QHBoxLayout(itemWidget);

        // Create the player name label
        QLabel *playerLabel = new QLabel(player, itemWidget);

        // Set a minimum height for the label to ensure text is not cut off
        playerLabel->setMinimumHeight(30);

        // Create the "Send" button
        QPushButton *sendButton = new QPushButton("Send Request", itemWidget);

        // Adjust the button size
        sendButton->setMinimumHeight(30);
        sendButton->setFixedWidth(100);

        // Add the label and button to the layout
        itemLayout->addWidget(playerLabel);
        itemLayout->addWidget(sendButton);
        itemLayout->setContentsMargins(5, 5, 5, 5); // Add some padding
        itemWidget->setLayout(itemLayout);

        // Add the custom widget to the QListWidget
        QListWidgetItem *listItem = new QListWidgetItem(onlinePlayersList);

        // Set a fixed height for the list item to match the widget
        listItem->setSizeHint(QSize(0, 40)); // Set height to 40 pixels

        onlinePlayersList->addItem(listItem);
        onlinePlayersList->setItemWidget(listItem, itemWidget);
    }
}

QString CreateGameRoom::generateRoomID() {
    QTcpSocket socket;
    socket.connectToHost("192.168.10.103", 8080);

    if (!socket.waitForConnected(3000)) {
        QMessageBox::critical(this, "Connection Error", "Failed to connect to the server.");
        return QString();
    }

    // Prepare the request JSON
    qDebug() << "Using token for create_room request:" << token;

    QJsonObject requestJson;
    requestJson["type"] = "create_room";
    requestJson["token"] = token;

    QJsonDocument requestDoc(requestJson);
    QByteArray requestData = requestDoc.toJson();
    socket.write(requestData);

    // Wait for the server to acknowledge the request
    if (!socket.waitForBytesWritten(3000)) {
        QMessageBox::critical(this, "Error", "Failed to send data to the server.");
        return QString();
    }

    // Wait for the server's response
    if (!socket.waitForReadyRead(3000)) {
        QMessageBox::critical(this, "Error", "No response from the server.");
        return QString();
    }

    // Process the response
    QByteArray responseData = socket.readAll();
    qDebug() << "Server response for create_room:" << responseData;

    QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
    QJsonObject responseObj = responseDoc.object();

    QString status = responseObj["status"].toString();
    QString message = responseObj["message"].toString();

    if (status == "success") {
        QJsonObject dataObj = responseObj["data"].toObject();
        QString roomId = dataObj["id"].toString();

        QMessageBox::information(this, "Room Created", "Room created successfully. Room ID: " + roomId);
        return roomId;
    } else {
        QMessageBox::critical(this, "Error", message);
        return QString() = "failed";
    }
}

void CreateGameRoom::displayRoomID() {
    roomIDLabel->setText("Room ID: " + roomID);  // This updates the label with the room ID
}
