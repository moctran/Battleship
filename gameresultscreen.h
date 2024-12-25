#ifndef GAMERESULTSCREEN_H
#define GAMERESULTSCREEN_H

#include <QWidget>
#include <QStackedWidget>
#include <QLabel>

class GameResultScreen : public QWidget {
    Q_OBJECT

public:
    explicit GameResultScreen(QStackedWidget *stackedWidget, QWidget *parent = nullptr);

    // Method to set the winner and loser
    void setResults(const QString &winner, const QString &loser);

private slots:
    // Slot to navigate back to the home screen
    void navigateToHome();

private:
    QStackedWidget *stackedWidget; // Reference to the stacked widget
    QLabel *winnerLabel;           // Label to display the winner
    QLabel *loserLabel;            // Label to display the loser
};

#endif // GAMERESULTSCREEN_H
