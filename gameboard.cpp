#include "gameboard.h"
#include "gameresultscreen.h"
#include "socketmanager.h"
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
extern QString globalUserToken;
extern QString globalUserId;
extern QString globalUserName;

// Constructor
GameBoard::GameBoard(QStackedWidget *stackedWidget, QWidget *parent)
    : BaseGameScreen(stackedWidget, parent), stackedWidget(stackedWidget) {
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

    // Center layout for game title and buttons
    QVBoxLayout *centerLayout = new QVBoxLayout();
    centerLayout->setSpacing(20);

    // Game title label
    gameTitle = new QLabel("Battleship Game", this);
    gameTitle->setAlignment(Qt::AlignCenter);
    gameTitle->setStyleSheet("font-size: 24px; font-weight: bold;");
    centerLayout->addWidget(gameTitle);

    // Return button
    resignButton = new QPushButton("Resign", this);
    resignButton->setFixedSize(100, 40);
    resignButton->setStyleSheet("font-size: 14px;");
    centerLayout->addWidget(resignButton, 0, Qt::AlignCenter);
    connect(resignButton, &QPushButton::clicked, this, &GameBoard::onResignClicked);

    // Make Move button
    makeMoveButton = new QPushButton("Make Move", this);
    makeMoveButton->setFixedSize(100, 40);
    makeMoveButton->setStyleSheet("font-size: 14px;");
    makeMoveButton->setEnabled(false); // Initially disabled
    makeMoveButton->setVisible(false);
    centerLayout->addWidget(makeMoveButton, 0, Qt::AlignCenter);
    connect(makeMoveButton, &QPushButton::clicked, this, &GameBoard::onMakeMoveClicked);

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
    setMinimumSize(1000, 600);

    // Connect opponent board buttons to the click handler
    connectOpponentBoardButtons();
    SocketManager* socketManager = SocketManager::getInstance();
    connect(socketManager, &SocketManager::messageReceived, this, &GameBoard::onResignGameHanlde);
    connect(socketManager, &SocketManager::messageReceived, this, &GameBoard::onMoveReceived);
}

// Create a board
void GameBoard::createBoard(QGridLayout *layout, QPushButton *board[10][10], const QString &boardName) {
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            board[row][col] = new QPushButton();
            board[row][col]->setFixedSize(40, 40);
            board[row][col]->setStyleSheet("background-color: lightblue");
            layout->addWidget(board[row][col], row, col);
            board[row][col]->setProperty("row", row);
            board[row][col]->setProperty("col", col);
        }
    }
}

// Connect opponent board buttons to their slot
void GameBoard::connectOpponentBoardButtons() {
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            connect(opponentBoard[row][col], &QPushButton::clicked, this, &GameBoard::onOpponentBlockClicked);
        }
    }
}

// Handle opponent block clicks
void GameBoard::onOpponentBlockClicked() {
    QPushButton *clickedBlock = qobject_cast<QPushButton *>(sender());
    if (!clickedBlock)
        return;

    if (selectedBlock == clickedBlock)
        return; // Ignore if the same block is clicked again

    // Revert the color of the previously selected block
    if (selectedBlock) {
        selectedBlock->setStyleSheet("background-color: lightblue; border: 1px solid gray;");
    }

    // Highlight the clicked block
    selectedBlock = clickedBlock;
    selectedBlock->setStyleSheet("background-color: darkblue; border: 2px solid black;");

    // Enable the Make Move button
    makeMoveButton->setEnabled(true);
}

void GameBoard::onMakeMoveClicked() {
    if (!selectedBlock) {
        qDebug() << "No block selected.";
        return;
    }

    // Get the row and column of the selected block
    int row = selectedBlock->property("row").toInt();
    int col = selectedBlock->property("col").toInt();

    // Print the chosen block coordinates
    qDebug() << "Chosen block:" << "Row" << row << ", Col" << col;

    // Disable the selected block visually
    selectedBlock->setEnabled(false);
    selectedBlock->setStyleSheet("background-color: gray; border: 1px solid black;");

    // Send the move request to the server
    sendMoveRequest(row, col);

    // Reset the selected block pointer
    selectedBlock = nullptr;
}

void GameBoard::sendMoveRequest(int row, int col) {
    QJsonObject requestJson;
    requestJson["type"] = "make_move";
    requestJson["token"] = token;
    requestJson["x"] = row;
    requestJson["y"] = col;

    QByteArray responseData = sendRequest(requestJson, 3000);
    QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
    QJsonObject responseObj = responseDoc.object();

    QString status = responseObj["status"].toString();
    QString message = responseObj["message"].toString();

    if (status == "success") {
        QMessageBox::information(this, "Make a move", message);
        makeMoveButton->setVisible(false);

        QJsonObject dataObject = responseObj["data"].toObject();
        QString winnerID = dataObject["winnerId"].toString();
        if (!winnerID.isEmpty()) {
            QString firstPlayerId = dataObject["firstPlayerId"].toString();
            QString secondPlayerId = dataObject["secondPlayerId"].toString();
            QString loserId = (winnerID == firstPlayerId) ? secondPlayerId : firstPlayerId;

            qDebug() << winnerID << loserId;
            GameResultScreen *resultScreen = dynamic_cast<GameResultScreen *>(stackedWidget->widget(10));
            if (resultScreen) {
                resultScreen->setResults(winnerID, loserId);
            }
            resetBoards();
            stackedWidget->setCurrentIndex(10);
        }
    } else {
        QMessageBox::critical(this, "Error", message);
    }
    qDebug() << responseData;
}

// Handle return button click
void GameBoard::onResignClicked() {
    QJsonObject requestJson;
    requestJson["type"] = "resign_game";
    requestJson["token"] = token;

    QByteArray responseData = sendRequest(requestJson, 3000);
    QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
    QJsonObject responseObj = responseDoc.object();

    // Debug the response
    if (responseObj.contains("status") && responseObj["status"].toString() == "success") {
        qDebug() << "Resign Successful:" << responseObj["message"].toString();
        // Navigate to index 3 in the stacked widget
        stackedWidget->setCurrentIndex(3);
    } else {
        QString errorMessage = responseObj.contains("message") ? responseObj["message"].toString() : "Unknown error";
        qDebug() << "Resign Failed:" << errorMessage;
        // Optionally show an error message to the user
        QMessageBox::critical(this, "Resign Failed", errorMessage);
    }
}

void GameBoard::onResignGameHanlde(const QByteArray &message) {
    QJsonDocument doc = QJsonDocument::fromJson(message);
    qDebug() << "Resign Game message: " << message;
    if (!doc.isObject()) {
        qWarning() << "Invalid JSON format!";
        return;
    }
    // Get the root JSON object
    QJsonObject jsonObject = doc.object();
    if (jsonObject.contains("type") && jsonObject["type"].toString() == "GAME_RESIGN") {
        if (jsonObject.contains("message")) {
            QString messageText = jsonObject["message"].toString();
            QMessageBox::information(this, "Room Players Change", messageText);
            resetBoards();
            stackedWidget->setCurrentIndex(3);
        } else {
            qWarning() << "'message' not found in JSON!";
        }
    } else {
        qDebug() << "Message type is not GAME_RESIGN, ignoring.";
    }
}

// Set the token
void GameBoard::setToken(const QString &newToken) {
    token = newToken;
    qDebug() << "Token set in GameBoard:" << token;
}

// Set the initial board state
void GameBoard::setInitialState(QList<QList<int>> initialState) {
    initialBoardState = initialState;
    qDebug() << "Initial state saved";
}

// Display the initial board state
void GameBoard::displayInitialState() {
    qDebug() << "Initial Board State:";

    int row_num = 0;
    for (const QList<int> &row : initialBoardState) {
        QString line;
        int col_num = 0;
        for (int cell : row) {
            line += QString::number(cell) + " ";
            QString colour = getColour(cell);  // Get the color from the method
            playerBoard[row_num][col_num]->setStyleSheet("background-color: " + colour + ";");
            opponentBoard[row_num][col_num]->setStyleSheet("background-colour: lightblue");
            col_num += 1;
        }
        row_num += 1;
        qDebug() << line;
    }
}

void GameBoard::firstMoveCheck(bool isFirstPlayer, bool isFirstPlayerTurn) {
    if(isFirstPlayer == isFirstPlayerTurn) {
        makeMoveButton->setVisible(true);
    }
}

void GameBoard::onMoveReceived(const QByteArray &message) {
    QJsonDocument doc = QJsonDocument::fromJson(message);
    qDebug() << "Game move message: " << message;
    if (!doc.isObject()) {
        qWarning() << "Invalid JSON format!";
        return;
    }

    QJsonObject jsonObject = doc.object();
    if (jsonObject.contains("type") && jsonObject["type"].toString() == "GAME_MOVE") {
        qDebug() << "Processing GAME_MOVE message.";

        QJsonObject dataObject = jsonObject["data"].toObject();
        if (dataObject.contains("lastMove")) {
            QJsonObject lastMoveObject = dataObject["lastMove"].toObject();
            QString winnerID = dataObject["winnerId"].toString();
            // Extract "status", "x", and "y"
            int status = lastMoveObject.value("status").toInt();
            int x = lastMoveObject.value("x").toInt();
            int y = lastMoveObject.value("y").toInt();

            qDebug() << "Extracted last move details:";
            qDebug() << "Status:" << status;
            qDebug() << "X:" << x;
            qDebug() << "Y:" << y;

            switch (status) {
            case 1:
                QMessageBox::information(this, "Move Result", "Ship Missed!");
                playerBoard[x][y]->setStyleSheet("background-color: grey;"); //
                break;
            case 2:
                QMessageBox::information(this, "Move Result", "Ship Hit!");
                playerBoard[x][y]->setStyleSheet("background-color: #004C4C;"); // Dark Teal
                if (!winnerID.isEmpty()) {
                    QString firstPlayerId = dataObject["firstPlayerId"].toString();
                    QString secondPlayerId = dataObject["secondPlayerId"].toString();
                    QString loserId = (winnerID == firstPlayerId) ? secondPlayerId : firstPlayerId;

                    GameResultScreen *resultScreen = dynamic_cast<GameResultScreen *>(stackedWidget->widget(10));
                    if (resultScreen) {
                       resultScreen->setResults(winnerID, loserId);
                    }
                    resetBoards();
                    stackedWidget->setCurrentIndex(10);
                }
                break;
            case 3:
                QMessageBox::information(this, "Move Result", "Ship Destroyed!");
                playerBoard[x][y]->setStyleSheet("background-color: #004C4C;"); // Existing Blue
                break;
            default:
                QMessageBox::warning(this, "Invalid Status", "Invalid move status!");
                break;
            }
            makeMoveButton->setVisible(true);
        } else {
            qWarning() << "No 'lastMove' field found in data!";
        }
    } else {
        qDebug() << "Message type is not GAME_MOVE, ignoring.";
    }
}

void GameBoard::resetBoards() {
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            // Reset player board
            playerBoard[row][col]->setStyleSheet("background-color: lightblue;");

            // Reset opponent board
            opponentBoard[row][col]->setStyleSheet("background-color: lightblue;");
            opponentBoard[row][col]->setEnabled(true);
        }
    }
}
