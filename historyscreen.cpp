#include "historyscreen.h"
#include "basescreen.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QDebug>

HistoryScreen::HistoryScreen(QStackedWidget *stackedWidget, const QString &token, QWidget *parent)
    : baseScreen(parent), stackedWidget(stackedWidget), token(token) {

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
    QJsonObject requestJson;
    requestJson["type"] = "view_profile";
    requestJson["token"] = token;

    QByteArray responseData = sendRequest(requestJson, 60000);

    qDebug() << "Server response for view_profile:" << responseData;

    QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
    QJsonObject responseObj = responseDoc.object();

    if (responseObj["status"].toString() == "success") {
        displayHistory(responseObj["data"].toObject());
    } else {
        QString errorMessage = responseObj["message"].toString();
        QMessageBox::critical(this, "Error", errorMessage);
    }
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
