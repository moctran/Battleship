#include "testScreen.h"
#include <QMessageBox>

TestScreen::TestScreen(QStackedWidget *stackedWidget, QWidget *parent)
    : QWidget(parent), stackedWidget(stackedWidget) {
    // Create layout
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Add label
    label = new QLabel("Welcome to the Test Screen!", this);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    // Add button
    QPushButton *homeButton = new QPushButton("Go to Home Screen", this);
    layout->addWidget(homeButton);

    // Connect button signal to a slot
    connect(homeButton, &QPushButton::clicked, this, &TestScreen::navigateToHome);

    // Set layout
    setLayout(layout);
}

void TestScreen::navigateToHome() {
    if (stackedWidget) {
        stackedWidget->setCurrentIndex(3); // Assuming the home screen is at index 0
    }
}
