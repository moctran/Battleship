#include "loginscreen.h"
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <socketmanager.h>

extern QString globalUserToken;

LoginScreen::LoginScreen(QStackedWidget *stackedWidget, QWidget *parent)
    : QWidget(parent), stackedWidget(stackedWidget) {

    usernameInput = new QLineEdit(this);
    usernameInput->setPlaceholderText("Enter username");

    passwordInput = new QLineEdit(this);
    passwordInput->setPlaceholderText("Enter password");
    passwordInput->setEchoMode(QLineEdit::Password);

    loginButton = new QPushButton("Log in", this);
    backButton = new QPushButton("Back", this);

    layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Login Screen", this));
    layout->addWidget(usernameInput);
    layout->addWidget(passwordInput);
    layout->addWidget(loginButton);
    layout->addWidget(backButton);
    setLayout(layout);

    connect(loginButton, &QPushButton::clicked, this, &LoginScreen::onLoginButtonClicked);
    connect(backButton, &QPushButton::clicked, this, &LoginScreen::onBackButtonClicked);
}

void LoginScreen::onLoginButtonClicked() {
    QString username = usernameInput->text();
    QString password = passwordInput->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Username and password cannot be empty.");
        return;
    }

    QTcpSocket socket;
    socket.connectToHost("127.0.0.1", 8080);

    if (!socket.waitForConnected(3000)) {
        QMessageBox::critical(this, "Connection Error", "Failed to connect to the server.");
        return;
    }

    QJsonObject json;
    json["type"] = "signin";
    json["username"] = username;
    json["password"] = password;

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    socket.write(data);

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

    if (responseObj["status"].toString() == "success") {
        QJsonObject dataObj = responseObj["data"].toObject();
        globalUserToken = dataObj["token"].toString();

        // Debug: Log the token
        qDebug() << "Token received from server:" << globalUserToken;

        QMessageBox::information(this, "Login Successful", "You are now logged in.");
        stackedWidget->setCurrentIndex(3); // Navigate to Home Screen
    } else {
        QString errorMessage = responseObj["message"].toString();
        QMessageBox::critical(this, "Login Failed", errorMessage);
    }
    socket.close();

    // Call the "subscribe_notification" API using SocketManager
    SocketManager* socketManager = SocketManager::getInstance();
    socketManager->connectToServer("127.0.0.1", 8080); // Ensure connection is established

    QJsonObject subscribeJson;
    subscribeJson["type"] = "subcribe_notification";
    subscribeJson["token"] = globalUserToken;

    QJsonDocument subscribeDoc(subscribeJson);
    QByteArray subscribeData = subscribeDoc.toJson();
    socketManager->getSocket()->write(subscribeData);
    socketManager->getSocket()->flush();
    socketManager->getInstance()->printSocketInfo();


}

void LoginScreen::onBackButtonClicked() {
    stackedWidget->setCurrentIndex(0); // Navigate back to the Initial Screen
}
