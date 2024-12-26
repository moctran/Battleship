#include "leaderboardscreen.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QDebug>
#include <QHeaderView>

LeaderboardScreen::LeaderboardScreen(QStackedWidget *stackedWidget, QWidget *parent)
    : baseScreen(parent), stackedWidget(stackedWidget) {
    leaderboardTable = new QTableWidget(this);
    backButton = new QPushButton("Back", this);

    // Set up the layout
    layout = new QVBoxLayout(this);
    layout->addWidget(leaderboardTable);
    layout->addWidget(backButton);
    setLayout(layout);

    // Set up the table
    leaderboardTable->setColumnCount(4);
    leaderboardTable->setHorizontalHeaderLabels({"Username", "ELO", "Matches", "Win Rate"});
    leaderboardTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    leaderboardTable->setSelectionMode(QAbstractItemView::NoSelection);
    leaderboardTable->horizontalHeader()->setStretchLastSection(true);
    leaderboardTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Connect the back button
    connect(backButton, &QPushButton::clicked, this, &LeaderboardScreen::onBackButtonClicked);
}

void LeaderboardScreen::setToken(const QString &newToken) {
    token = newToken;
    qDebug() << "Token set in LeaderboardScreen:" << token;
}

void LeaderboardScreen::loadLeaderboard() {
    qDebug() << "Leaderboard screen activated. Token:" << token;

    if (token.isEmpty()) {
        QMessageBox::critical(this, "Error", "You must log in to view the leaderboard.");
        return;
    }

    fetchLeaderboard();
}

void LeaderboardScreen::fetchLeaderboard() {
    QJsonObject requestJson;
    requestJson["type"] = "leaderboard";
    requestJson["token"] = token;

    QByteArray responseData = sendRequest(requestJson, 60000);

    qDebug() << "Server response for leaderboard:" << responseData;

    QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
    QJsonObject responseObj = responseDoc.object();

    if (responseObj["status"].toString() == "success") {
        displayLeaderboard(responseObj["data"].toArray());
    } else {
        QString errorMessage = responseObj["message"].toString();
        QMessageBox::critical(this, "Error", errorMessage);
    }
}

void LeaderboardScreen::displayLeaderboard(const QJsonArray &data) {
    leaderboardTable->setRowCount(data.size());
    int row = 0;

    for (const QJsonValue &entryValue : data) {
        QJsonObject entry = entryValue.toObject();

        // Add data to the table
        leaderboardTable->setItem(row, 0, new QTableWidgetItem(entry["username"].toString()));
        leaderboardTable->setItem(row, 1, new QTableWidgetItem(QString::number(entry["elo"].toInt())));
        leaderboardTable->setItem(row, 2, new QTableWidgetItem(QString::number(entry["matchCount"].toInt())));
        leaderboardTable->setItem(row, 3, new QTableWidgetItem(QString::number(entry["winRate"].toDouble(), 'f', 2) + "%"));

        row++;
    }
}

void LeaderboardScreen::onBackButtonClicked() {
    stackedWidget->setCurrentIndex(3); // Navigate back to Home Screen
}
