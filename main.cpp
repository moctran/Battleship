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
QString globalUserToken;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

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
    stackedWidget.setCurrentWidget(initialScreen);
    stackedWidget.show();

    return app.exec();
}
