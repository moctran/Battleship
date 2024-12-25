#include "gameresultscreen.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

GameResultScreen::GameResultScreen(QStackedWidget *stackedWidget, QWidget *parent)
    : QWidget(parent), stackedWidget(stackedWidget) {
    // Create layout
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Add Winner label
    winnerLabel = new QLabel("Winner: TBD", this);
    winnerLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(winnerLabel);

    // Add Loser label
    loserLabel = new QLabel("Loser: TBD", this);
    loserLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(loserLabel);

    // Add button
    QPushButton *homeButton = new QPushButton("Go to Home Screen", this);
    layout->addWidget(homeButton);

    // Connect button signal to a slot
    connect(homeButton, &QPushButton::clicked, this, &GameResultScreen::navigateToHome);

    // Set layout
    setLayout(layout);
}

void GameResultScreen::navigateToHome() {
    if (stackedWidget) {
        stackedWidget->setCurrentIndex(3); // Assuming the home screen is at index 3
    }
}

void GameResultScreen::setResults(const QString &winner, const QString &loser) {
    winnerLabel->setText("Winner: " + winner);
    loserLabel->setText("Loser: " + loser);
}
