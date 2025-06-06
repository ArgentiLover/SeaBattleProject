#ifndef GAMESETTINGS_H
#define GAMESETTINGS_H

#include <QString>
#include <QList>

enum class OpponentType {
    Human,
    Robot
};

enum class Difficulty {
    Easy,
    Medium,
    Hard
};

enum class PlatformType {
    SinglePC,
    LocalMultiplayer
};

class GameSettings {
private:
    static int fieldSize;       // Размер поля
    static bool shipsCanBeAdjacent; //размещение рядом
    static QString shootMode;    // Режим стрельбы
    static int timeLimit;       // Время на ход
    static QList<int> shipCounts; //корабли
public:
    static OpponentType opponent;
    static Difficulty difficulty;
    static PlatformType platform;


    static void setFieldSize(int size) { fieldSize = size; }
    static void setShipsCanBeAdjacent(bool canBeAdjacent) { shipsCanBeAdjacent = canBeAdjacent; }
    static void setShipCounts(int count1, int count2, int count3, int count4) {
        shipCounts[0] = count1; shipCounts[1] = count2; shipCounts[2] = count3; shipCounts[3] = count4;
    }
    static void setShootingRule(const QString& rule) { shootMode = rule; }
    static void setTimeLimit(int time) { timeLimit = time; }

    // Методы для получения значений
    static int getFieldSize() { return fieldSize; }
    static bool getShipsCanBeAdjacent() { return shipsCanBeAdjacent; }
    static int getShip1Count() { return shipCounts[0]; }
    static int getShip2Count() { return shipCounts[1]; }
    static int getShip3Count() { return shipCounts[2]; }
    static int getShip4Count() { return shipCounts[3]; }
    static QString getShootingRule() { return shootMode; }
    static int getTimeLimit() { return timeLimit; }

};

extern OpponentType currentOpponent;

#endif // GAMESETTINGS_H
