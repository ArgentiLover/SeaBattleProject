#ifndef FIELD_H
#define FIELD_H

#include <vector>
#include <QPoint>

class Field {
public:
    enum CellStatus { Empty, Ship, Sunk, Hit, Miss, RadarHit };

    Field(int size);

    CellStatus getCell(int x, int y) const;
    bool canPlaceShip(int x, int y, int size, bool horizontal) const;
    void placeShip(int x, int y, int size, bool horizontal);
    int size() const;
    int width() const;
    int height() const;
    void reset();
    void setCell(int x, int y, CellStatus status);
    std::vector<QPoint> getShipCellsAt(int x, int y) const;
    bool isShipSunkAt(int x, int y) const;
    bool isInBounds(int x, int y) const;
    bool areAllShipsSunk() const;

private:
    std::vector<std::vector<CellStatus>> grid;  // Сетка поля
    int fieldWidth;
    int fieldHeight;
    int fieldSize;
};

#endif // FIELD_H
