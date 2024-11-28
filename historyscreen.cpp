#include "historyscreen.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QDebug>

HistoryScreen::HistoryScreen(QStackedWidget *stackedWidget, const QString &token, QWidget *parent)
    : QWidget(parent), stackedWidget(stackedWidget), token(token) {

    summaryLabel = new QLabel(this);
    historyList = new QListWidget(this);
    backButton = new QPushButton("Back", this);

    layout = new QVBoxLayout(this);
    layout->addWidget(summaryLabel);
    layout->addWidget(historyList);
    layout->addWidget(backButton);

    setLayout(layout);

    connect(backButton, &QPushButton::clicked, this, &HistoryScreen::onBackButtonClicked);
}

void HistoryScreen::setToken(const QString &newToken) {
    token = newToken;
    qDebug() << "Token set in HistoryScreen:" << token;
}

void HistoryScreen::loadHistory() {
    qDebug() << "History screen activated. Token:" << token;

    if (token.isEmpty()) {
        QMessageBox::critical(this, "Error", "You must log in to view history.");
        return;
    }

    fetchHistory();
}

void HistoryScreen::fetchHistory() {
    QTcpSocket socket;
    socket.connectToHost("127.0.0.1", 8080);

    if (!socket.waitForConnected(3000)) {
        QMessageBox::critical(this, "Connection Error", "Failed to connect to the server.");
        return;
    }

    qDebug() << "Using token for view_profile request:" << token;

    QJsonObject json;
    json["type"] = "view_profile";
    json["token"] = token;

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

    qDebug() << "Server response for view_profile:" << responseData;

    QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
    QJsonObject responseObj = responseDoc.object();

    if (responseObj["status"].toString() == "success") {
        displayHistory(responseObj["data"].toObject());
    } else {
        QString errorMessage = responseObj["message"].toString();
        QMessageBox::critical(this, "Error", errorMessage);
    }

    socket.close();
}

void HistoryScreen::displayHistory(const QJsonObject &data) {
    int totalMatches = data["totalMatches"].toInt();
    int totalWins = data["totalWins"].toInt();
    double winPercentage = data["winPercentage"].toDouble();

    summaryLabel->setText(QString("Total Matches: %1 | Wins: %2 | Win Percentage: %3%")
                              .arg(totalMatches)
                              .arg(totalWins)
                              .arg(QString::number(winPercentage, 'f', 2)));

    QJsonArray matches = data["matches"].toArray();
    historyList->clear();

    for (const QJsonValue &matchValue : matches) {
        QJsonObject match = matchValue.toObject();
        QString matchText = QString("Match ID: %1 | Winner: %2 | Time: %3")
                                .arg(match["id"].toString())
                                .arg(match["winnerId"].toString())
                                .arg(match["timestamp"].toString());
        historyList->addItem(matchText);
    }
}

void HistoryScreen::onBackButtonClicked() {
    stackedWidget->setCurrentIndex(3); // Navigate back to Home Screen
}
