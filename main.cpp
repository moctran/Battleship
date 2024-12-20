#include <QApplication>
#include "initialscreen.h"
#include "loginscreen.h"
#include "signupscreen.h"
#include "homescreen.h"
#include "joingameroom.h"
#include "creategameroom.h"
#include "historyscreen.h"
#include "battleshipboard.h"
#include "gameboard.h"
#include <QString>
#include "networkmanager.h"
#include "testScreen.h"
QString globalUserToken;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create NetworkManager instance
    NetworkManager networkManager;

    // Connect to localhost:8080
    networkManager.connectToServer("127.0.0.1", 8080);

    // Send subscription message once connected
    QObject::connect(&networkManager, &NetworkManager::connectionEstablished, [&]() {
        QJsonObject message;
        message["type"] = "subcribe_notification";
        message["token"] = "6569C395-A174-4616-B54E-80830C4144F5";
        networkManager.sendMessage(message);
    });

    // Handle incoming data
    QObject::connect(&networkManager, &NetworkManager::dataReceived, [](const QString &data) {
        qDebug() << "Data received from server:" << data;

        // Parse JSON data if needed
        QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
        if (!doc.isNull() && doc.isObject()) {
            QJsonObject obj = doc.object();
            qDebug() << "Parsed JSON:" << obj;
        } else {
            qDebug() << "Received invalid JSON.";
        }
    });


    QStackedWidget stackedWidget;

    // Create screens
    InitialScreen *initialScreen = new InitialScreen(&stackedWidget);
    LoginScreen *loginScreen = new LoginScreen(&stackedWidget);
    SignUpScreen *signUpScreen = new SignUpScreen(&stackedWidget);
    HomeScreen *homeScreen = new HomeScreen(&stackedWidget);
    JoinGameRoom *joinGameRoom = new JoinGameRoom(&stackedWidget);
    HistoryScreen *historyScreen = new HistoryScreen(&stackedWidget, globalUserToken);

    CreateGameRoom *createGameRoom = new CreateGameRoom(&stackedWidget);

    BattleshipBoard *setupgameBoard = new BattleshipBoard(&stackedWidget); // Set up board
    GameBoard *gameBoard = new GameBoard(&stackedWidget);
    TestScreen *testScreen = new TestScreen(&stackedWidget);
    // Add screens to the stacked widget
    stackedWidget.addWidget(initialScreen); // Index 0
    stackedWidget.addWidget(loginScreen);   // Index 1
    stackedWidget.addWidget(signUpScreen);  // Index 2
    stackedWidget.addWidget(homeScreen);    // Index 3
    stackedWidget.addWidget(joinGameRoom);  // Index 4
    stackedWidget.addWidget(createGameRoom); // Index 5

    stackedWidget.addWidget(historyScreen); // Add History Screen at an appropriate index (6)
    stackedWidget.addWidget(setupgameBoard); // Index 7
    stackedWidget.addWidget(gameBoard); // Index 8
    stackedWidget.addWidget(testScreen); // Index 9
    stackedWidget.setCurrentWidget(initialScreen);
    stackedWidget.show();

    return app.exec();
}
