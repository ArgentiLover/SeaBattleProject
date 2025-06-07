#include "GameSettings.h"

OpponentType GameSettings::opponent = OpponentType::Human;
Difficulty GameSettings::difficulty = Difficulty::Easy;
PlatformType GameSettings::platform = PlatformType::SinglePC;
int GameSettings::fieldSize = 10;
bool GameSettings::shipsCanBeAdjacent = false;
QList<int> shipCounts = {4, 3, 2, 1};
QString GameSettings::shootMode = "Стандартная";
int GameSettings::timeLimit = 30;
QString GameSettings::player1name = "Игрок 1";
QString GameSettings::player2name = "Игрок 2";
QList<int> GameSettings::shipCounts = {4, 3, 2, 1};
