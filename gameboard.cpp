#include "gameboard.h"
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
extern QString globalUserToken;

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
    returnButton = new QPushButton("Return", this);
    returnButton->setFixedSize(100, 40);
    returnButton->setStyleSheet("font-size: 14px;");
    centerLayout->addWidget(returnButton, 0, Qt::AlignCenter);
    connect(returnButton, &QPushButton::clicked, this, &GameBoard::onReturnClicked);

    // Make Move button
    makeMoveButton = new QPushButton("Make Move", this);
    makeMoveButton->setFixedSize(100, 40);
    makeMoveButton->setStyleSheet("font-size: 14px;");
    makeMoveButton->setEnabled(false); // Initially disabled
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
    setMinimumSize(800, 600);

    // Connect opponent board buttons to the click handler
    connectOpponentBoardButtons();
}

// Create a board
void GameBoard::createBoard(QGridLayout *layout, QPushButton *board[10][10], const QString &boardName) {
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            board[row][col] = new QPushButton();
            board[row][col]->setFixedSize(40, 40);
            board[row][col]->setStyleSheet("background-color: lightblue; border: 1px solid gray;");
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
    /* QTcpSocket *socket = new QTcpSocket(this); // Use dynamic socket for signal-slot connections
    socket->connectToHost("127.0.0.1", 8080);

    // Check if connection to the server is successful
    if (!socket->waitForConnected(3000)) {
        QMessageBox::critical(this, "Connection Error", "Failed to connect to the server.");
        makeMoveButton->setEnabled(true); // Re-enable the button on failure
        socket->deleteLater();
        return;
    }

    // Prepare the JSON request
    QJsonObject requestJson;
    requestJson["type"] = "make_move";
    requestJson["token"] = token;
    requestJson["x"] = row;
    requestJson["y"] = col;

    QJsonDocument requestDoc(requestJson);
    QByteArray requestData = requestDoc.toJson();

    // Send the request to the server
    socket->write(requestData);
    if (!socket->waitForBytesWritten(3000)) {
        QMessageBox::critical(this, "Error", "Failed to send data to the server.");
        makeMoveButton->setEnabled(true); // Re-enable the button on failure
        socket->deleteLater();
        return;
    }

    // Connect to the readyRead signal to process the server's response asynchronously
    connect(socket, &QTcpSocket::readyRead, this, [this, socket]() {
        QByteArray responseData = socket->readAll();
        QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
        QJsonObject responseObj = responseDoc.object();

        qDebug() << "Move response:" << responseObj;

        if (responseObj["status"] == "success") {
            qDebug() << "Move confirmed by the server.";
            makeMoveButton->setEnabled(true); // Re-enable the button for the next move
        } else {
            QMessageBox::warning(this, "Invalid Move", responseObj["message"].toString());
            makeMoveButton->setEnabled(true); // Re-enable the button on failure
        }

        // Clean up the socket
        socket->close();
    });

    // Handle socket errors
    connect(socket, &QTcpSocket::errorOccurred, this, [this, socket]() {
        QMessageBox::critical(this, "Error", "Failed to communicate with the server.");
        makeMoveButton->setEnabled(true); // Re-enable the button on failure

        // Clean up the socket
        socket->close();
    }); */
    qDebug() << "To implement later...";
}


// Handle return button click
void GameBoard::onReturnClicked() {
    QTcpSocket socket;
    socket.connectToHost("127.0.0.1", 8080);

    if (!socket.waitForConnected(3000)) {
        QMessageBox::critical(this, "Connection Error", "Failed to connect to the server.");
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
    if (!socket.waitForBytesWritten(3000)) {
        QMessageBox::critical(this, "Error", "Failed to send data to the server.");
    }

    // Wait for the server's response
    if (!socket.waitForReadyRead(3000)) {
        QMessageBox::critical(this, "Error", "No response from the server.");
    }

    // Process the response
    QByteArray responseData = socket.readAll();
    qDebug() << "Server response for leave_room:" << responseData;

    QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
    QJsonObject responseObj = responseDoc.object();
    stackedWidget->setCurrentIndex(3); // Go back to the previous screen
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
            switch (cell) {
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
            default:
                playerBoard[row_num][col_num]->setStyleSheet("background-color: lightblue;");
                break;
            }
            col_num += 1;
        }
        row_num += 1;
        qDebug() << line;
    }
}
