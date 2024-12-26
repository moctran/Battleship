#include "announcementpopup.h"
#include <QApplication>
#include <QScreen>
#include <QVBoxLayout>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QFontMetrics>

AnnouncementPopup::AnnouncementPopup(QWidget *parent)
    : QWidget(parent), messageLabel(new QLabel(this)) {
    // Set window properties
    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup); // Make it a frameless popup window
    setAttribute(Qt::WA_TranslucentBackground);          // Make the background transparent
    setAutoFillBackground(true);                        // Enable background fill

    // Set up the layout for the popup
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->setWordWrap(true); // Enable text wrapping
    messageLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: white; "
                                "background-color: rgba(0, 0, 0, 0.7); border-radius: 10px; padding: 10px;");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(messageLabel);
    setLayout(layout);

    // Set a default size
    setFixedSize(300, 100);
}

AnnouncementPopup::~AnnouncementPopup() {
    // Cleanup if necessary
}

void AnnouncementPopup::showPopup(const QString &messageText) {
    // Set the message text in the label
    messageLabel->setText(messageText);
    messageLabel->setWordWrap(true); // Enable word wrapping
    messageLabel->setAlignment(Qt::AlignCenter); // Center-align text

    // Apply font and styling for better visibility
    messageLabel->setStyleSheet(
        "font-size: 24px; font-weight: bold; color: white; "
        "background-color: rgba(0, 0, 0, 0.7); border-radius: 10px; padding: 20px;"
        );

    // Set the fixed size of the popup window
    setFixedSize(800, 600);

    // Manually set the QLabel's geometry to fit within the popup
    messageLabel->setGeometry(20, 20, 760, 560); // Leave padding of 20px on all sides

    // Center the popup on the primary screen
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);

    // Animate the opacity of the popup for a smooth appearance
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
    setGraphicsEffect(opacityEffect);
    QPropertyAnimation *animation = new QPropertyAnimation(opacityEffect, "opacity");
    animation->setDuration(500); // Duration for fade-in animation
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();

    // Show the popup
    show();

    // Hide the popup after 3 seconds
    QTimer::singleShot(60000, this, &QWidget::close);
}

