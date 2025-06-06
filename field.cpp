#include "field.h"
#include "GameSettings.h"

#include <queue>
#include <vector>
#include <QPoint>

Field::Field(int size)
    : fieldSize(size), fieldWidth(size), fieldHeight(size),
    grid(size, std::vector<CellStatus>(size, Empty)) {}
Field::CellStatus Field::getCell(int x, int y) const {
    if (x >= 0 && x < fieldWidth && y >= 0 && y < fieldHeight) {
        return grid[x][y];
    }
    return Empty; // Выход за пределы поля возвращает пустую клетку
}

bool Field::areAllShipsSunk() const {
    for (int y = 0; y < fieldSize; ++y) {
        for (int x = 0; x < fieldSize; ++x) {
            if (grid[y][x] == Ship || grid[y][x] == RadarHit) {
                return false;
            }
        }
    }
    return true;
}

std::vector<QPoint> Field::getShipCellsAt(int x, int y) const {
    std::vector<QPoint> shipCells;
    if (getCell(x, y) != Ship && getCell(x, y) != Hit) {
        return shipCells; // Если в клетке нет корабля — вернуть пустой вектор
    }

    std::vector<std::vector<bool>> visited(fieldWidth, std::vector<bool>(fieldHeight, false));
    std::queue<QPoint> q;
    q.push(QPoint(x, y));
    visited[x][y] = true;

    // Поиск связных клеток корабля (по 4 направлениям)
    const int dx[4] = {1, -1, 0, 0};
    const int dy[4] = {0, 0, 1, -1};

    while (!q.empty()) {
        QPoint p = q.front();
        q.pop();
        shipCells.push_back(p);

        for (int dir = 0; dir < 4; ++dir) {
            int nx = p.x() + dx[dir];
            int ny = p.y() + dy[dir];
            if (nx >= 0 && nx < fieldWidth && ny >= 0 && ny < fieldHeight) {
                if (!visited[nx][ny]) {
                    Field::CellStatus status = getCell(nx, ny);
                    if (status == Ship || status == Hit || status == RadarHit) {
                        visited[nx][ny] = true;
                        q.push(QPoint(nx, ny));
                    }
                }
            }
        }
    }

    return shipCells;
}

// Проверяет, потоплен ли корабль, которому принадлежит клетка (x,y)
bool Field::isShipSunkAt(int x, int y) const {
    if (getCell(x, y) != Ship && getCell(x, y) != Hit && getCell(x,y) != RadarHit) {
        return false; // В клетке нет корабля, значит и потоплен он быть не может
    }

    auto shipCells = getShipCellsAt(x, y);

    // Проверяем, что все клетки корабля подбиты (Hit)
    for (const QPoint& cell : shipCells) {
        if (getCell(cell.x(), cell.y()) != Hit) {
            return false; // Есть неповреждённые клетки корабля
        }
    }

    return true;
}

bool Field::canPlaceShip(int x, int y, int size, bool horizontal) const {
    // Проверка выхода за пределы поля
    if (horizontal && x + size > fieldWidth) {
        return false;
    } else if (!horizontal && y + size > fieldHeight) {
        return false;
    }

    // Проверка занятых клеток
    for (int i = 0; i < size; ++i) {
        int px = horizontal ? x + i : x;
        int py = horizontal ? y : y + i;

        if (getCell(px, py) != Empty) {
            return false;  // Если хотя бы одна клетка занята
        }

        // Дополнительная проверка соседних клеток на наличие других кораблей
        if (!GameSettings::getShipsCanBeAdjacent()) {
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    int nx = px + dx;
                    int ny = py + dy;
                    if (nx >= 0 && nx < fieldWidth && ny >= 0 && ny < fieldHeight) {
                        if (getCell(nx, ny) == Ship) {
                            return false;
                        }
                    }
                }
            }
        }
    }

    return true;
}

bool Field::isInBounds(int x, int y) const {
    int size = GameSettings::getFieldSize();
    return x >= 0 && x < size && y >= 0 && y < size;
}

void Field::setCell(int x, int y, CellStatus status) {
    if (x >= 0 && x < fieldWidth && y >= 0 && y < fieldHeight) {
        grid[x][y] = status;
    }
}

void Field::placeShip(int x, int y, int size, bool horizontal) {
    for (int i = 0; i < size; ++i) {
        int px = horizontal ? x + i : x;
        int py = horizontal ? y : y + i;

        grid[px][py] = Ship;
    }
}

int Field::width() const {
    return fieldWidth;
}

int Field::height() const {
    return fieldHeight;
}

void Field::reset() {
    for (int i = 0; i < fieldWidth; ++i) {
        for (int j = 0; j < fieldHeight; ++j) {
            grid[i][j] = Empty;  // Очищаем поле
        }
    }
}
