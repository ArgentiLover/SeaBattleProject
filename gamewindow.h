#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "field.h"
#include "GameSettings.h"

namespace Ui {
class GameWindow;
}

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(Field* player1Field, Field* player2Field, QWidget *parent = nullptr);
    ~GameWindow();

private slots:
    void handleCellClick(int x, int y);

private:
    Ui::GameWindow *ui;

    Field* player1Field;
    Field* player2Field;
    Field* currentEnemyField;

    QTimer* turnTimer;
    int turnTimeRemaining;

    bool waitingForNextPlayer = false;
    QPushButton* switchPlayerButton = nullptr;

    QVector<QVector<QPushButton*>> player1Buttons;
    QVector<QVector<QPushButton*>> player2Buttons;

    int currentPlayer = 1;
    void setupGrids();
    void switchTurn();
    void updateUI();
    QString borderStyleForCell(int x, int y, Field* field);
};

#endif // GAMEWINDOW_H
