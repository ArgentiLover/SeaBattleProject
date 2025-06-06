#ifndef SHIPPLACEMENTMENU_H
#define SHIPPLACEMENTMENU_H

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QVector>
#include <QRadioButton>
#include <QGroupBox>
#include <QMap>
#include "GameSettings.h"
#include "cell.h"
#include "field.h"


namespace Ui {
class ShipPlacementMenu;
}

class ShipPlacementMenu : public QMainWindow
{
    Q_OBJECT

public:
    explicit ShipPlacementMenu(QWidget *parent = nullptr);
    ~ShipPlacementMenu();

private slots:
    void selectShip(int size);
    void placeShipAt(int x, int y);
    void showShipPreview(int x, int y);
    void clearShipPreview(int x, int y);
    void on_pushButton_clicked();

    void on_pushButton_return_clicked();

private:
    Ui::ShipPlacementMenu *ui;
    Field* field;
    bool shipHorizontal = true;
    QVector<QVector<Cell*>> fieldCells;
    QList<Cell*> previewCells;
    void createFieldGrid();
    Field* fieldPlayer1;
    Field* fieldPlayer2;
    int currentPlayer = 1;

    int currentShipSize = 0;
    QMap<int, int> remainingShips; // ключ — размер, значение — сколько осталось
    struct PlacedShip {
        int x, y;
        int size;
        bool horizontal;
    };

    QList<PlacedShip> placedShips;

    void initShipSelection();
    void updateShipCountUI();
    bool allShipsPlaced() const;
    bool canPlaceShip(int x, int y, int size, bool horizontal) const;
    void updateFieldDisplay();
    QString borderStyleForCell(int x, int y);
    void resetShipPlacementForNewPlayer();
};

#endif // SHIPPLACEMENTMENU_H
