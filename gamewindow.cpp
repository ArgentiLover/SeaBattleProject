#include "gamewindow.h"
#include "ui_gamewindow.h"

#include "GameSettings.h"
#include "mainwindow.h"

#include <QGridLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QTimer>
#include <QTime>

GameWindow::GameWindow(Field* p1, Field* p2, QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::GameWindow),
    player1Field(p1),
    player2Field(p2),
    currentPlayer(1)
{
    ui->setupUi(this);

    //таймер
    turnTimer = new QTimer(this);
    turnTimeRemaining = GameSettings::getTimeLimit();

    connect(turnTimer, &QTimer::timeout, this, [this]() {
        turnTimeRemaining--;
        ui->labelTimer->setText(QString("Время: %1 с").arg(turnTimeRemaining));

        if (turnTimeRemaining <= 0) {
            turnTimer->stop();
            QMessageBox::information(this, "Время вышло",
                                     QString("Игрок %1 пропустил ход.").arg(currentPlayer));
            switchTurn();
            updateUI();
        }
    });

    setupGrids();
    updateUI();

    switchPlayerButton = new QPushButton(this);
    switchPlayerButton->setText("Ход игрока 1");

    int w = 250;
    int h = 50;
    switchPlayerButton->setGeometry((width() - w) / 2, (height() - h) / 2, w, h);
    QFont font = switchPlayerButton->font();
    font.setPointSize(18);
    font.setBold(true);
    switchPlayerButton->setFont(font);
    switchPlayerButton->setStyleSheet(
        "QPushButton {"
        " background-color: #0078d7;"
        " color: white;"
        " border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        " background-color: #005a9e;"
        "}"
        );
    switchPlayerButton->setVisible(false);

    connect(switchPlayerButton, &QPushButton::clicked, this, [this]() {
        waitingForNextPlayer = false;
        switchPlayerButton->setVisible(false);
        updateUI();
    });
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::setupGrids()
{
    int size = GameSettings::getFieldSize();

    player1Buttons.resize(size);
    player2Buttons.resize(size);

    ui->player1Grid->setSpacing(0);
    ui->player1Grid->setContentsMargins(0, 0, 0, 0);
    ui->player2Grid->setSpacing(0);
    ui->player2Grid->setContentsMargins(0, 0, 0, 0);

    for (int row = 0; row < size; ++row) {
        player1Buttons[row].resize(size);
        player2Buttons[row].resize(size);

        for (int col = 0; col < size; ++col) {
            QPushButton *btn1 = new QPushButton(this);
            QPushButton *btn2 = new QPushButton(this);

            btn1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            btn2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

            btn1->setStyleSheet("background-color: white; border: 1px solid gray;");
            btn2->setStyleSheet("background-color: lightblue; border: 1px solid gray;");

            // Для игрока 2 клики по левому полю (btn1)
            connect(btn1, &QPushButton::clicked, this, [this, col, row]() {
                if (currentPlayer == 2) {
                    handleCellClick(col, row);
                }
            });

            // Для игрока 1 клики по правому полю (btn2)
            connect(btn2, &QPushButton::clicked, this, [this, col, row]() {
                if (currentPlayer == 1) {
                    handleCellClick(col, row);
                }
            });

            player1Buttons[row][col] = btn1;
            player2Buttons[row][col] = btn2;

            ui->player1Grid->addWidget(btn1, row, col);
            ui->player2Grid->addWidget(btn2, row, col);
        }
    }

    for (int i = 0; i < size; ++i) {
        ui->player1Grid->setColumnStretch(i, 1);
        ui->player1Grid->setRowStretch(i, 1);
        ui->player2Grid->setColumnStretch(i, 1);
        ui->player2Grid->setRowStretch(i, 1);
    }
}

void GameWindow::handleCellClick(int x, int y)
{
    Field* enemyField = (currentPlayer == 1) ? player2Field : player1Field;
    auto& enemyButtons = (currentPlayer == 1) ? player2Buttons : player1Buttons;

    bool hit = false;

    Field::CellStatus cellStatus = enemyField->getCell(x, y);
    if (cellStatus == Field::Ship || cellStatus == Field::RadarHit) {
        enemyField->setCell(x, y, Field::Hit);
        hit = true;

        if (enemyField->isShipSunkAt(x, y)) {
            auto shipCells = enemyField->getShipCellsAt(x, y);
            for (const QPoint& cell : shipCells) {
                enemyField->setCell(cell.x(), cell.y(), Field::Sunk);
            }
        }
    } else if (cellStatus == Field::Empty) {
        enemyField->setCell(x, y, Field::Miss);
    }

    // Режим "Радар" — подсветка соседних клеток с кораблями, если не попал
    if (GameSettings::getShootingRule() == "Радар" && !hit) {
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                int nx = x + dx;
                int ny = y + dy;
                if (enemyField->isInBounds(nx, ny)) {
                    Field::CellStatus neighborStatus = enemyField->getCell(nx, ny);
                    if (neighborStatus == Field::Ship) {   // <-- тут
                        enemyField->setCell(nx, ny, Field::RadarHit);
                    }
                }
            }
        }
    }
    //победа
    if (enemyField->areAllShipsSunk()) {
        turnTimer->stop();
        updateUI();
        QMessageBox::information(this, "Победа", QString("Игрок %1 победил!").arg(currentPlayer));

        MainWindow *mainWin = new MainWindow();
        mainWin->show();

        this->close();
        return;
    }

    // Обработка смены хода
    if (GameSettings::getShootingRule() == "Стандартная") {
        if (!hit) switchTurn();
    } else {
        switchTurn();  // Одиночная и Радар — всегда смена хода
    }


    updateUI();
}




void GameWindow::switchTurn()
{
    turnTimer->stop();
    waitingForNextPlayer = true;
    currentPlayer = 3 - currentPlayer;

    switchPlayerButton->setText(QString("Ход игрока %1").arg(currentPlayer));
    switchPlayerButton->setVisible(true);

    updateUI();
}

void GameWindow::updateUI()
{
    ui->labelStatus->setText(waitingForNextPlayer
                                 ? QString("Передайте управление игроку %1").arg(currentPlayer)
                                 : QString("Ход игрока %1").arg(currentPlayer));

    auto& leftButtons = player1Buttons;
    auto& rightButtons = player2Buttons;

    int size = GameSettings::getFieldSize();

    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            QPushButton* btnLeft = player1Buttons[y][x];
            QPushButton* btnRight = player2Buttons[y][x];

            if (waitingForNextPlayer) {
                turnTimer->stop();
                ui->labelTimer->setText("");
                // Серым и заблокированным делаем все кнопки
                btnLeft->setEnabled(false);
                btnRight->setEnabled(false);
                btnLeft->setStyleSheet("background-color: gray; border: 1px solid gray;");
                btnRight->setStyleSheet("background-color: gray; border: 1px solid gray;");
            } else {
                Field::CellStatus stateLeft = player1Field->getCell(x, y);
                QString styleLeft;
                if (currentPlayer == 1) {
                    switch (stateLeft) {
                    case Field::Empty: styleLeft = "background-color: white;"; break;
                    case Field::Ship:  styleLeft = "background-color: navy;"; break;
                    case Field::RadarHit: styleLeft = "background-color: navy;"; break;
                    case Field::Hit:   styleLeft = "background-color: red;"; break;
                    case Field::Miss:  styleLeft = "background-color: gray;"; break;
                    case Field::Sunk:  styleLeft = "background-color: black;"; break;
                    }
                    btnLeft->setEnabled(false);
                } else {
                    switch (stateLeft) {
                    case Field::Hit:  styleLeft = "background-color: red;"; break;
                    case Field::Miss: styleLeft = "background-color: gray;"; break;
                    case Field::Sunk: styleLeft = "background-color: black;"; break;
                    case Field::RadarHit:  styleLeft = "background-color: yellow;"; break;
                    default:          styleLeft = "background-color: lightblue;"; break;
                    }
                    btnLeft->setEnabled(stateLeft != Field::Hit && stateLeft != Field::Miss && currentPlayer == 2);
                }
                styleLeft += "border: 1px solid gray;";
                btnLeft->setStyleSheet(styleLeft);

                // Правое поле аналогично
                Field::CellStatus stateRight = player2Field->getCell(x, y);
                QString styleRight;
                if (currentPlayer == 2) {
                    switch (stateRight) {
                    case Field::Empty: styleRight = "background-color: white;"; break;
                    case Field::Ship:  styleRight = "background-color: navy;"; break;
                    case Field::RadarHit: styleRight = "background-color: navy;"; break;
                    case Field::Hit:   styleRight = "background-color: red;"; break;
                    case Field::Miss:  styleRight = "background-color: gray;"; break;
                    case Field::Sunk:  styleRight = "background-color: black;"; break;
                    }
                    btnRight->setEnabled(false);
                } else {
                    switch (stateRight) {
                    case Field::Hit:  styleRight = "background-color: red;"; break;
                    case Field::Miss: styleRight = "background-color: gray;"; break;
                    case Field::Sunk: styleRight = "background-color: black;"; break;
                    case Field::RadarHit:  styleRight = "background-color: yellow;"; break;
                    default:          styleRight = "background-color: lightblue;"; break;
                    }
                    btnRight->setEnabled(stateRight != Field::Hit && stateRight != Field::Miss && currentPlayer == 1);
                }
                styleRight += "border: 1px solid gray;";
                btnRight->setStyleSheet(styleRight);
            }
        }
    }

    turnTimeRemaining = GameSettings::getTimeLimit();
    ui->labelTimer->setText(QString("Время: %1 с").arg(turnTimeRemaining));
    turnTimer->start(1000);
}

