#include "gameboard.h"
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
extern QString globalUserToken;

// This is the game play board
GameBoard::GameBoard (QStackedWidget *stackedWidget, QWidget *parent)
    : QWidget(parent), stackedWidget(stackedWidget) {
    // Main layout to contain both boards and the game title
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    // Player's Board
    playerLayout = new QGridLayout();
    createBoard(playerLayout, playerBoard, "Player Board");
    QVBoxLayout *playerContainer = new QVBoxLayout();
    QLabel *playerLabel = new QLabel("Player's Board", this);
    playerLabel->setAlignment(Qt::AlignCenter);
    playerContainer->addWidget(playerLabel);
    playerContainer->addLayout(playerLayout);

    // Center layout for game title and return button
    QVBoxLayout *centerLayout = new QVBoxLayout();
    centerLayout->setSpacing(20);

    // Game title label
    gameTitle = new QLabel("Battleship Game", this);
    gameTitle->setAlignment(Qt::AlignCenter);
    gameTitle->setStyleSheet("font-size: 24px; font-weight: bold;");
    centerLayout->addWidget(gameTitle);

    // Return button
    returnButton = new QPushButton("Return", this);
    returnButton->setFixedSize(100, 40);
    returnButton->setStyleSheet("font-size: 14px;");
    centerLayout->addWidget(returnButton, 0, Qt::AlignCenter);
    connect(returnButton, &QPushButton::clicked, this, &GameBoard::onReturnClicked);

    // Opponent's Board
    opponentLayout = new QGridLayout();
    createBoard(opponentLayout, opponentBoard, "Opponent Board");
    QVBoxLayout *opponentContainer = new QVBoxLayout();
    QLabel *opponentLabel = new QLabel("Opponent's Board", this);
    opponentLabel->setAlignment(Qt::AlignCenter);
    opponentContainer->addWidget(opponentLabel);
    opponentContainer->addLayout(opponentLayout);

    // Add components to the main layout with spacing
    mainLayout->addLayout(playerContainer);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(centerLayout);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(opponentContainer);

    setLayout(mainLayout);
    setMinimumSize(800, 600);
}

void GameBoard::createBoard(QGridLayout *layout, QPushButton *board[8][8], const QString &boardName) {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            board[row][col] = new QPushButton();
            board[row][col]->setFixedSize(40, 40);
            board[row][col]->setStyleSheet("background-color: lightblue; border: 1px solid gray;");
            layout->addWidget(board[row][col], row, col);
        }
    }
}

void GameBoard::onReturnClicked() {
    QTcpSocket socket;
    socket.connectToHost("127.0.0.1", 8080);

    if (!socket.waitForConnected(3000)) {
        QMessageBox::critical(this, "Connection Error", "Failed to connect to the server.");

    }

    // Prepare the request JSON
    qDebug() << "Using token for create_room request:" << token;

    QJsonObject requestJson;
    requestJson["type"] = "leave_room";
    requestJson["token"] = token;

    QJsonDocument requestDoc(requestJson);
    QByteArray requestData = requestDoc.toJson();
    socket.write(requestData);

    // Wait for the server to acknowledge the request
    if (!socket.waitForBytesWritten(3000)) {
        QMessageBox::critical(this, "Error", "Failed to send data to the server.");
    }

    // Wait for the server's response
    if (!socket.waitForReadyRead(3000)) {
        QMessageBox::critical(this, "Error", "No response from the server.");
    }

    // Process the response
    QByteArray responseData = socket.readAll();
    qDebug() << "Server response for create_room:" << responseData;

    QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
    QJsonObject responseObj = responseDoc.object();

    qDebug() << "Server response for view_profile:" << responseData;
    stackedWidget->setCurrentIndex(3);
}

void GameBoard::setToken(const QString &newToken) {
    token = newToken;
    qDebug() << "Token set in CreateGameScreen:" << token;
}

void GameBoard::setInitialState(QList<QList<int>> initialState) {
    initialBoardState = initialState;
    qDebug() << "Initial state saved";
}

void GameBoard::displayInitialState() {
    qDebug() << "Intial Board State:";

    int row_num = 0;
    for (const QList<int> &row : initialBoardState) {
        QString line;
        int col_num = 0;
        for (int cell : row) {
            line += QString::number(cell) + " ";
            switch(cell) {
            case 1:
                playerBoard[row_num][col_num]->setStyleSheet("background-color: red;");
                break;
            case 2:
                playerBoard[row_num][col_num]->setStyleSheet("background-color: orange;");
                break;
            case 3:
                playerBoard[row_num][col_num]->setStyleSheet("background-color: yellow;");
                break;
            case 4:
                playerBoard[row_num][col_num]->setStyleSheet("background-color: green;");
                break;
            case 5:
                playerBoard[row_num][col_num]->setStyleSheet("background-color: blue;");
                break;
            }
            col_num = col_num + 1;
        }
        row_num = row_num + 1;
        qDebug() << line;
    }
}
