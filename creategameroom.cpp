#include "creategameroom.h"
#include "battleshipboard.h"
#include "socketmanager.h"
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

extern QString globalUserToken;

CreateGameRoom::CreateGameRoom(QStackedWidget *stackedWidget, QWidget *parent)
    : BaseGameScreen(stackedWidget, parent), stackedWidget(stackedWidget) {

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
    SocketManager* socketManager = SocketManager::getInstance();
    connect(socketManager, &SocketManager::messageReceived, this, &CreateGameRoom::onPlayerChanges);
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
    // QTcpSocket socket;
    // socket.connectToHost("127.0.0.1", 8080);

    // if (!socket.waitForConnected(3000)) {
    //     QMessageBox::critical(this, "Connection Error", "Failed to connect to the server.");
    // }

    // // Prepare the request JSON
    // qDebug() << "Using token for leave_room request:" << token;

    // QJsonObject requestJson;
    // requestJson["type"] = "leave_room";
    // requestJson["token"] = token;

    // QJsonDocument requestDoc(requestJson);
    // QByteArray requestData = requestDoc.toJson();
    // socket.write(requestData);

    // // Wait for the server to acknowledge the request
    // if (!socket.waitForBytesWritten(3000)) {
    //     QMessageBox::critical(this, "Error", "Failed to send data to the server.");
    // }

    // // Wait for the server's response
    // if (!socket.waitForReadyRead(3000)) {
    //     QMessageBox::critical(this, "Error", "No response from the server.");
    // }

    // // Process the response
    // QByteArray responseData = socket.readAll();
    // qDebug() << "Server response for leave_room:" << responseData;

    // QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
    // QJsonObject responseObj = responseDoc.object();
    // stackedWidget->setCurrentIndex(3); // Go back to the previous screen
    leaveRoom(token, stackedWidget);
}

void CreateGameRoom::onSendInviteClicked() {
    QMessageBox::information(this, "Send Invite", "Invite sent to the selected player!");
}

void CreateGameRoom::populateOnlinePlayers() {
    qDebug() << "populateOnlinePlayers() called";

    QTcpSocket socket;
    socket.connectToHost("127.0.0.1", 8080);

    if (!socket.waitForConnected(3000)) {
        QMessageBox::critical(this, "Connection Error", "Failed to connect to the server.");
        return;
    }

    QJsonObject requestJson;
    requestJson["type"] = "online_users";
    requestJson["token"] = token;

    QJsonDocument requestDoc(requestJson);
    QByteArray requestData = requestDoc.toJson();
    socket.write(requestData);

    if (!socket.waitForBytesWritten(3000)) {
        QMessageBox::critical(this, "Error", "Failed to send data to the server.");
        return;
    }

    if (!socket.waitForReadyRead(3000)) {
        QMessageBox::critical(this, "Error", "No response from the server.");
        return;
    }

    QByteArray responseData = socket.readAll();
    QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
    QJsonObject responseObj = responseDoc.object();

    QString status = responseObj["status"].toString();
    QString message = responseObj["message"].toString();

    if (status == "success") {
        QJsonArray dataArray = responseObj["data"].toArray();
        std::vector<Player> players;
        for (const QJsonValue &value : dataArray) {
            QJsonObject playerObj = value.toObject();
            Player player;
            player.id = playerObj["id"].toString();
            player.username = playerObj["username"].toString();
            players.push_back(player);
            qDebug() << player.id << " " << player.username;
        }
        displayOnlinePlayers(players); // Hiển thị danh sách người chơi online
    } else {
        QMessageBox::critical(this, "Error", message);
    }
}


void CreateGameRoom::displayOnlinePlayers(std::vector<Player>& players) {
    for (const Player &player : players) {
        // Create a custom widget for each player
        QWidget *itemWidget = new QWidget(onlinePlayersList);
        QHBoxLayout *itemLayout = new QHBoxLayout(itemWidget);

        // Create the player name label
        QLabel *playerLabel = new QLabel(player.username, itemWidget);

        // Set a minimum height for the label to ensure text is not cut off
        playerLabel->setMinimumHeight(30);

        // Create the "Send" button
        QPushButton *sendButton = new QPushButton("Send Request", itemWidget);

        // Adjust the button size
        sendButton->setMinimumHeight(30);
        sendButton->setFixedWidth(100);

        // Connect the button's clicked signal to the slot
        connect(sendButton, &QPushButton::clicked, this, [this, player]() {
            onSendButtonClicked(player.id);
        });

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

void CreateGameRoom::onSendButtonClicked(const QString &userId) {
    QTcpSocket socket;
    socket.connectToHost("127.0.0.1", 8080);

    if (!socket.waitForConnected(3000)) {
        QMessageBox::critical(this, "Connection Error", "Failed to connect to the server.");
    }


    QJsonObject requestJson;
    requestJson["type"] = "invite_user";
    requestJson["token"] = token;
    requestJson["user_id"] = userId;


    QJsonDocument requestDoc(requestJson);
    QByteArray requestData = requestDoc.toJson();
    socket.write(requestData);

    if (!socket.waitForBytesWritten(3000)) {
        QMessageBox::critical(this, "Error", "Failed to send data to the server.");
    }

    // Wait for the server's response
    if (!socket.waitForReadyRead(3000)) {
        QMessageBox::critical(this, "Error", "No response from the server.");
    }
    QByteArray responseData = socket.readAll();

    QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
    QJsonObject responseObj = responseDoc.object();
    QString message = responseObj["message"].toString();
    QMessageBox::information(this, "Invite User", message);
}

QString CreateGameRoom::generateRoomID() {
    QTcpSocket socket;
    socket.connectToHost("127.0.0.1", 8080);

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

void CreateGameRoom::onPlayerChanges(const QByteArray &message) {
    QJsonDocument doc = QJsonDocument::fromJson(message);

    // Check if the parsing was successful
    if (!doc.isObject()) {
        qWarning() << "Invalid JSON format!";
        return;
    }

    // Get the root JSON object
    QJsonObject jsonObject = doc.object();

    // Check if the "type" attribute exists and matches "ROOM_PLAYERS_CHANGE"
    if (jsonObject.contains("type") && jsonObject["type"].toString() == "ROOM_PLAYERS_CHANGE") {
        qDebug() << "Message type is ROOM_PLAYERS_CHANGE";

        // Check if the "data" attribute exists and is an object
        if (jsonObject.contains("data") && jsonObject["data"].isObject()) {
            QJsonObject dataObject = jsonObject["data"].toObject();

            // Extract "firstPlayerId" and "secondPlayerId"
            QString firstPlayerId = dataObject.value("firstPlayerId").toString();
            QString secondPlayerId = dataObject.value("secondPlayerId").toString();

            // Print IDs to console
            qDebug() << "First Player ID:" << firstPlayerId;
            qDebug() << "Second Player ID:" << secondPlayerId;

            // Update labels
            player1Label->setText("First Player: " + firstPlayerId);
            player2Label->setText("Second Player: " + secondPlayerId);
        } else {
            qWarning() << "'data' not found or is not an object!";
        }

        // Display the "message" part in a message box
        if (jsonObject.contains("message")) {
            QString messageText = jsonObject["message"].toString();
            QMessageBox::information(this, "Room Players Change", messageText);
        } else {
            qWarning() << "'message' not found in JSON!";
        }

    } else {
        qDebug() << "Message type is not ROOM_PLAYERS_CHANGE, ignoring.";
    }
}

void CreateGameRoom::updateLabels(const QString player1Name, const QString player2Name) {
    player1Label->setText("First Player: " + player1Name);
    player2Label->setText("Second Player: " + player2Name);

    qDebug() << "Updated Labels:";
    qDebug() << "First Player: " << player1Name;
    qDebug() << "Second Player: " << player2Name;
}

