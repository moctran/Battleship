#include <QApplication>
#include <QString>
#include "initialscreen.h"
#include "loginscreen.h"
#include "signupscreen.h"
#include "homescreen.h"
#include "joingameroom.h"
#include "creategameroom.h"
#include "historyscreen.h"
#include "battleshipboard.h"
#include "gameboard.h"
#include "gameresultscreen.h"
#include "leaderboardscreen.h"
QString globalUserToken;
QString globalUserId;
QString globalUserName;

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
    GameResultScreen *gameResult = new GameResultScreen(&stackedWidget);
    LeaderboardScreen *leaderboardScreen = new LeaderboardScreen(&stackedWidget);
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
    stackedWidget.addWidget(gameResult); // Index 9
    stackedWidget.addWidget(leaderboardScreen);
    stackedWidget.setCurrentWidget(initialScreen);
    stackedWidget.show();
    stackedWidget.setFixedSize(1200, 600); // Fixed size for all screens
    return app.exec();
}
