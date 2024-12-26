#include "historyscreen.h"
#include "basescreen.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QDebug>
#include <QHeaderView>

HistoryScreen::HistoryScreen(QStackedWidget *stackedWidget, const QString &token, QWidget *parent)
    : baseScreen(parent), stackedWidget(stackedWidget), token(token) {

    historyTable = new QTableWidget(this);
    backButton = new QPushButton("Back", this);

    layout = new QVBoxLayout(this);
    layout->addWidget(historyTable);
    layout->addWidget(backButton);

    setLayout(layout);

    // Configure the table
    historyTable->setColumnCount(3);
    historyTable->setHorizontalHeaderLabels({"Match ID", "Winner", "Time"});
    historyTable->horizontalHeader()->setStretchLastSection(true);
    historyTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    historyTable->setSelectionMode(QAbstractItemView::NoSelection);
    historyTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

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
    historyTable->clearContents();
    historyTable->setRowCount(0);

    QJsonArray matches = data["matches"].toArray();

    for (const QJsonValue &matchValue : matches) {
        QJsonObject match = matchValue.toObject();

        int row = historyTable->rowCount();
        historyTable->insertRow(row);

        QTableWidgetItem *matchIdItem = new QTableWidgetItem(match["id"].toString());
        QTableWidgetItem *winnerItem = new QTableWidgetItem(match["winnerId"].toString());
        QTableWidgetItem *timeItem = new QTableWidgetItem(match["timestamp"].toString());

        historyTable->setItem(row, 0, matchIdItem);
        historyTable->setItem(row, 1, winnerItem);
        historyTable->setItem(row, 2, timeItem);
    }
}

void HistoryScreen::onBackButtonClicked() {
    stackedWidget->setCurrentIndex(3); // Navigate back to Home Screen
}
