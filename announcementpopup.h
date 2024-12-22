#ifndef ANNOUNCEMENTPOPUP_H
#define ANNOUNCEMENTPOPUP_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

class AnnouncementPopup : public QWidget {
    Q_OBJECT

public:
    explicit AnnouncementPopup(QWidget *parent = nullptr);
    ~AnnouncementPopup() override;

    // Method to show the popup with a custom message
    void showPopup(const QString &messageText);

private:
    QLabel *messageLabel;  // Label to display the message
};

#endif // ANNOUNCEMENTPOPUP_H
