#include <QApplication>
#include "initialscreen.h"
#include "loginscreen.h"
#include "signupscreen.h"
#include "homescreen.h"
#include "joingameroom.h"
#include "creategameroom.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QStackedWidget stackedWidget;

    // Create screens
    InitialScreen *initialScreen = new InitialScreen(&stackedWidget);
    LoginScreen *loginScreen = new LoginScreen(&stackedWidget);
    SignUpScreen *signUpScreen = new SignUpScreen(&stackedWidget);
    HomeScreen *homeScreen = new HomeScreen(&stackedWidget);
    JoinGameRoom *joinGameRoom = new JoinGameRoom(&stackedWidget);

    CreateGameRoom *createGameRoom = new CreateGameRoom(&stackedWidget);

    // Add screens to the stacked widget
    stackedWidget.addWidget(initialScreen); // Index 0
    stackedWidget.addWidget(loginScreen);   // Index 1
    stackedWidget.addWidget(signUpScreen);  // Index 2
    stackedWidget.addWidget(homeScreen);    // Index 3
    stackedWidget.addWidget(joinGameRoom);  // Index 4
    stackedWidget.addWidget(createGameRoom); // Add CreateGameRoom screen at index 5

    stackedWidget.setCurrentWidget(initialScreen);
    stackedWidget.show();

    return app.exec();
}
