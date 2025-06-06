#include "shipplacementmenu.h"
#include "ui_shipplacementmenu.h"
#include "GameSettings.h"
#include "field.h"
#include "gamewindow.h"
#include "mainwindow.h"

#include <QWidget>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMap>
#include <QMessageBox>


ShipPlacementMenu::ShipPlacementMenu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ShipPlacementMenu)
{

    ui->setupUi(this);
    fieldPlayer1 = new Field(GameSettings::getFieldSize());
    fieldPlayer2 = new Field(GameSettings::getFieldSize());

    currentPlayer = 1;
    field = fieldPlayer1;
    createFieldGrid();
    initShipSelection();

    connect(ui->btnSize1, &QPushButton::clicked, this, [=] { selectShip(1); });
    connect(ui->btnSize2, &QPushButton::clicked, this, [=] { selectShip(2); });
    connect(ui->btnSize3, &QPushButton::clicked, this, [=] { selectShip(3); });
    connect(ui->btnSize4, &QPushButton::clicked, this, [=] { selectShip(4); });
}

ShipPlacementMenu::~ShipPlacementMenu()
{
    delete ui;
    delete field;
}

void ShipPlacementMenu::initShipSelection() {
    remainingShips[1] = GameSettings::getShip1Count();
    remainingShips[2] = GameSettings::getShip2Count();
    remainingShips[3] = GameSettings::getShip3Count();
    remainingShips[4] = GameSettings::getShip4Count();
    updateShipCountUI();
}

void ShipPlacementMenu::updateShipCountUI() {
    ui->labelSize1->setText(QString::number(remainingShips[1]));
    ui->labelSize2->setText(QString::number(remainingShips[2]));
    ui->labelSize3->setText(QString::number(remainingShips[3]));
    ui->labelSize4->setText(QString::number(remainingShips[4]));
}

void ShipPlacementMenu::selectShip(int size) {
    if (size < 1 || size > 4) return;

    if (remainingShips.contains(size) && remainingShips[size] > 0) {
        currentShipSize = size;
    } else {
        QMessageBox::information(this, "Нет кораблей", "Все корабли этого размера уже размещены.");
    }
}

bool ShipPlacementMenu::canPlaceShip(int x, int y, int size, bool horizontal) const {
    // Проверка на выход за пределы поля
    if (horizontal && x + size > field->width()) {
        return false;  // Выход за пределы поля по горизонтали
    } else if (!horizontal && y + size > field->height()) {
        return false;  // Выход за пределы поля по вертикали
    }

    // Проверка, чтобы клетки, на которых будет корабль, были пустыми
    for (int i = 0; i < size; ++i) {
        int px = horizontal ? x + i : x;
        int py = horizontal ? y : y + i;

        if (field->getCell(px, py) != Field::Empty) {
            return false;  // Клетка занята
        }

        // Дополнительная проверка соседних клеток на запрещенные попадания
        if (!GameSettings::getShipsCanBeAdjacent()) {
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    int nx = px + dx;
                    int ny = py + dy;
                    if (nx >= 0 && nx < field->width() && ny >= 0 && ny < field->height()) {
                        if (field->getCell(nx, ny) == Field::Ship) {
                            return false;  // Есть соседний корабль
                        }
                    }
                }
            }
        }
    }

    return true;
}

void ShipPlacementMenu::placeShipAt(int x, int y) {
    bool horizontal = shipHorizontal;
    if (currentShipSize == 0) {
        QMessageBox::information(this, "Ошибка", "Выберите размер корабля.");
        return;
    }

    if (remainingShips[currentShipSize] <= 0) {
        QMessageBox::information(this, "Ошибка", "Корабли этого размера закончились.");
        return;
    }

    if (field->canPlaceShip(x, y, currentShipSize, horizontal)) {
        // Если корабль можно разместить, то размещаем его
        field->placeShip(x, y, currentShipSize, horizontal);
        placedShips.append({x, y, currentShipSize, horizontal});
        // Обновляем количество оставшихся кораблей
        if (!remainingShips.contains(currentShipSize)) return;
        remainingShips[currentShipSize]--;
        updateShipCountUI();

        // Проверяем, все ли корабли размещены
        if (allShipsPlaced()) {
            if (currentPlayer == 1) {
                currentPlayer = 2;
                ui->label_player->setText("Игрок 2");
                field = fieldPlayer2;
                resetShipPlacementForNewPlayer();
                QMessageBox::information(this, "Игрок 2", "Теперь ход размещения для Игрока 2.");
            } else {
                QMessageBox::information(this, "Готово", "Оба игрока разместили корабли. Игра начинается!");
                GameWindow* gameWin = new GameWindow(fieldPlayer1, fieldPlayer2);
                gameWin->show();
                this->close();
            }
        }
        clearShipPreview(0, 0);
        updateFieldDisplay();
    } else {
        QMessageBox::warning(this, "Ошибка размещения", "Невозможно разместить корабль здесь.");
        clearShipPreview(0, 0);
    }
}

void ShipPlacementMenu::createFieldGrid() {
    int size = GameSettings::getFieldSize();
    fieldCells.resize(size);

    ui->fieldGrid->setSpacing(0);
    ui->fieldGrid->setContentsMargins(0, 0, 0, 0);

    for (int row = 0; row < size; ++row) {
        fieldCells[row].resize(size);
        for (int col = 0; col < size; ++col) {
            Cell *cell = new Cell(row, col, this);

            fieldCells[row][col] = cell;

            cell->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

            connect(cell, &QPushButton::clicked, this, [=]() {
                placeShipAt(col, row);  // при клике вызывается размещение
            });
            connect(cell, &Cell::rightClicked, this, [=](int, int) {
                shipHorizontal = !shipHorizontal;  // переключаем ориентацию
                clearShipPreview(0, 0);
                showShipPreview(col, row);
            });
            connect(cell, &Cell::hovered, this, [=](int, int) {
                showShipPreview(col, row);  // обязательно с правильными аргументами
            });
            connect(cell, &Cell::hoverLeft, this, &ShipPlacementMenu::clearShipPreview);

            ui->fieldGrid->addWidget(cell, row, col);

        }
    }

    for (int i = 0; i < size; ++i) {
        ui->fieldGrid->setColumnStretch(i, 1);
        ui->fieldGrid->setRowStretch(i, 1);
    }
}


//отрисовывает не так, как планировалось, но мне понравилось
QString ShipPlacementMenu::borderStyleForCell(int x, int y) {
    bool leftShip   = (x > 0 && field->getCell(x - 1, y) == Field::Ship);
    bool rightShip  = (x < field->width() - 1 && field->getCell(x + 1, y) == Field::Ship);
    bool topShip    = (y > 0 && field->getCell(x, y - 1) == Field::Ship);
    bool bottomShip = (y < field->height() - 1 && field->getCell(x, y + 1) == Field::Ship);

    QString borderTop    = topShip    ? "border-top: none;"    : "border-top: 2px solid black;";
    QString borderBottom = bottomShip ? "border-bottom: none;" : "border-bottom: 2px solid black;";
    QString borderLeft   = leftShip   ? "border-left: none;"   : "border-left: 2px solid black;";
    QString borderRight  = rightShip  ? "border-right: none;"  : "border-right: 2px solid black;";

    return borderTop + borderBottom + borderLeft + borderRight;
}

void ShipPlacementMenu::updateFieldDisplay() {
    int size = GameSettings::getFieldSize();

    // Сброс всех стилей
    for (int row = 0; row < size; ++row) {
        for (int col = 0; col < size; ++col) {
            fieldCells[row][col]->setStyleSheet("background-color: lightblue; border: 1px solid gray;");
        }
    }

    // Отрисовка границ кораблей
    for (const PlacedShip& ship : placedShips) {
        for (int i = 0; i < ship.size; ++i) {
            int x = ship.horizontal ? ship.x + i : ship.x;
            int y = ship.horizontal ? ship.y : ship.y + i;

            QString borderTop    = (i == 0)                        ? "border-top: 2px solid black;"    : "border-top: none;";
            QString borderBottom = (i == ship.size - 1)           ? "border-bottom: 2px solid black;" : "border-bottom: none;";
            QString borderLeft   = ship.horizontal && i == 0      ? "border-left: 2px solid black;"   : "border-left: none;";
            QString borderRight  = ship.horizontal && i == ship.size - 1
                                      ? "border-right: 2px solid black;" : "border-right: none;";

            if (!ship.horizontal) {
                std::swap(borderTop, borderLeft);
                std::swap(borderBottom, borderRight);
            }

            fieldCells[y][x]->setStyleSheet(
                QString("background-color: darkgray; %1%2%3%4")
                    .arg(borderTop)
                    .arg(borderBottom)
                    .arg(borderLeft)
                    .arg(borderRight)
                );
        }
    }
}

bool ShipPlacementMenu::allShipsPlaced() const {
    for (int i = 1; i <= 4; ++i) {
        if (remainingShips[i] > 0)
            return false;
    }
    return true;
}
void ShipPlacementMenu::showShipPreview(int x, int y) {
    if (currentShipSize == 0) return;
    clearShipPreview(0, 0);
    bool horizontal = shipHorizontal;
    previewCells.clear();

    for (int i = 0; i < currentShipSize; ++i) {
        int px = horizontal ? x + i : x;
        int py = horizontal ? y : y + i;


        if (field->getCell(px, py) == Field::Ship) continue;
        if (px >= field->width() || py >= field->height()) break;

        if (py >= 0 && py < field->height() && px >= 0 && px < field->width()) {
            previewCells.append(fieldCells[py][px]);
            fieldCells[py][px]->setStyleSheet("background-color: rgba(0, 200, 0, 0.4); border: 1px solid gray;");
        }
    }
}

void ShipPlacementMenu::clearShipPreview(int, int) {
    for (auto cell : previewCells) {
        int row = cell->row();
        int col = cell->col();
        if (field->getCell(col, row) == Field::Ship) {
            QString borders = borderStyleForCell(col, row);
            cell->setStyleSheet(QString("background-color: darkgray; %1").arg(borders));
        }
        else cell->setStyleSheet("background-color: lightblue; border: 1px solid gray;");
    }
    previewCells.clear();
}
void ShipPlacementMenu::resetShipPlacementForNewPlayer() {
    placedShips.clear();
    remainingShips.clear();
    remainingShips[1] = GameSettings::getShip1Count();
    remainingShips[2] = GameSettings::getShip2Count();
    remainingShips[3] = GameSettings::getShip3Count();
    remainingShips[4] = GameSettings::getShip4Count();
    updateShipCountUI();
    updateFieldDisplay();
}

void ShipPlacementMenu::on_pushButton_clicked()
{
    field->reset();
    placedShips.clear();
    currentShipSize = 0;
    previewCells.clear();
    clearShipPreview(0, 0);
    remainingShips.clear();
    remainingShips[1] = GameSettings::getShip1Count();
    remainingShips[2] = GameSettings::getShip2Count();
    remainingShips[3] = GameSettings::getShip3Count();
    remainingShips[4] = GameSettings::getShip4Count();
    updateShipCountUI();
    updateFieldDisplay();
    updateFieldDisplay();
    updateShipCountUI();
}


void ShipPlacementMenu::on_pushButton_return_clicked()
{
    MainWindow *mainWin = new MainWindow();
    mainWin->show();

    this->close();
    return;
}

