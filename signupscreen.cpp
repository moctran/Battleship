#include "signupscreen.h"
#include <QMessageBox>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>

SignUpScreen::SignUpScreen(QStackedWidget *stackedWidget, QWidget *parent)
    : QWidget(parent), stackedWidget(stackedWidget) {

    usernameInput = new QLineEdit(this);
    usernameInput->setPlaceholderText("Enter username");

    passwordInput = new QLineEdit(this);
    passwordInput->setPlaceholderText("Enter password");
    passwordInput->setEchoMode(QLineEdit::Password);

    signUpButton = new QPushButton("Sign up", this);
    backButton = new QPushButton("Back", this);

    layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Sign Up Screen", this));
    layout->addWidget(usernameInput);
    layout->addWidget(passwordInput);
    layout->addWidget(signUpButton);
    layout->addWidget(backButton);
    setLayout(layout);

    connect(signUpButton, &QPushButton::clicked, this, &SignUpScreen::onSignUpButtonClicked);
    connect(backButton, &QPushButton::clicked, this, &SignUpScreen::onBackButtonClicked);
}

void SignUpScreen::onSignUpButtonClicked() {
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


    // Prepare the sign-up request
    QJsonObject json;
    json["type"] = "signup";
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
        QMessageBox::information(this, "Sign Up Successful", "Your account has been created.");

        // Clear inputs
        usernameInput->clear();
        passwordInput->clear();

        // Navigate back to the Login Screen
        stackedWidget->setCurrentIndex(1); // Assuming Login Screen index is 1
    } else {
        QString errorMessage = responseObj["message"].toString();
        QMessageBox::critical(this, "Sign Up Failed", errorMessage);
    }

    // Optionally, you can choose to close the socket here if no longer needed
    // socket->close();
}

void SignUpScreen::onBackButtonClicked() {
    stackedWidget->setCurrentIndex(0); // Navigate back to the Initial Screen
}
