#include "announcementpopup.h"
#include <QVBoxLayout>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

AnnouncementPopup::AnnouncementPopup(QWidget *parent)
    : QWidget(parent), messageLabel(new QLabel(this)) {
    // Set window properties
    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup); // Make it a frameless popup window
    setAttribute(Qt::WA_TranslucentBackground);          // Make the background transparent
    setAutoFillBackground(true);                        // Enable background fill

    // Set up the layout for the popup
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: white; "
                                "background-color: rgba(0, 0, 0, 0.7); border-radius: 10px; padding: 10px;");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(messageLabel);
    setLayout(layout);

    // Set the fixed size of the popup
    setFixedSize(300, 100);
}

AnnouncementPopup::~AnnouncementPopup() {
    // Cleanup if necessary (e.g., remove connections, delete dynamically allocated objects)
}

void AnnouncementPopup::showPopup(const QString &messageText) {
    // Set the message in the label
    messageLabel->setText(messageText);

    // Animate the opacity of the popup for a smooth appearance
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
    messageLabel->setGraphicsEffect(opacityEffect);
    QPropertyAnimation *animation = new QPropertyAnimation(opacityEffect, "opacity");
    animation->setDuration(500); // Duration for fade-in animation
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();

    // Show the popup
    show();

    // Hide the popup after 3 seconds
    QTimer::singleShot(3000, this, &QWidget::close);
}
