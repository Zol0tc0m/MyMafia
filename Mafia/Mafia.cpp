#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <Windows.h>

using namespace std;

class Player {
public:
    string name;
    bool alive;
    string roleName;
};

class Role {
public:
    string name;
};

class Game {
public:
    vector<Player> players;
    Role mafiaRole;
    Role citizenRole;
    Role detectiveRole;
    Role doctorRole;
};

bool addPlayer(Game& game, string name) {
    Player player;
    player.name = name;
    player.alive = true;
    game.players.push_back(player);
    return true;
}

void chooseRole(Game& game, Player& player) {
    int choice;
    do {
        cout << "Выберите роль для " << player.name << ": " << endl;
        cout << "1. Мафия" << endl;
        cout << "2. Гражданин" << endl;
        if (game.detectiveRole.name != "") {
            cout << "3. Детектив" << endl;
        }
        if (game.doctorRole.name != "") {
            cout << "4. Доктор" << endl;
        }
        cin >> choice;
    } while (choice < 1 || choice > 4);

    switch (choice) {
    case 1:
        player.roleName = game.mafiaRole.name;
        break;
    case 2:
        player.roleName = game.citizenRole.name;
        break;
    case 3:
        player.roleName = game.detectiveRole.name;
        break;
    case 4:
        player.roleName = game.doctorRole.name;
        break;
    }
}

void distributeRoles(Game& game) {
    int mafiaCount = 0;
    int citizenCount = 0;
    int detectiveCount = 0;
    int doctorCount = 0;

    for (size_t i = 0; i < game.players.size(); i++) {
        if (game.players[i].roleName == game.mafiaRole.name) {
            mafiaCount++;
        }
        else if (game.players[i].roleName == game.citizenRole.name) {
            citizenCount++;
        }
        else if (game.players[i].roleName == game.detectiveRole.name) {
            detectiveCount++;
        }
        else if (game.players[i].roleName == game.doctorRole.name) {
            doctorCount++;
        }
    }

    for (size_t i = 0; i < game.players.size(); i++) {
        if (game.players[i].roleName != "") continue;

        if (game.players.size() <= 9 && mafiaCount < 2) {
            game.players[i].roleName = game.mafiaRole.name;
            mafiaCount++;
        }
        else if (game.players.size() >= 10 && mafiaCount < 4) {
            game.players[i].roleName = game.mafiaRole.name;
            mafiaCount++;
        }
        else if (detectiveCount == 0 && game.detectiveRole.name != "") {
            game.players[i].roleName = game.detectiveRole.name;
            detectiveCount++;
        }
        else if (doctorCount == 0 && game.doctorRole.name != "") {
            game.players[i].roleName = game.doctorRole.name;
            doctorCount++;
        }
        else {
            game.players[i].roleName = game.citizenRole.name;
        }
    }
}


void printRoles(Game& game) {
    cout << "Роли игроков:" << endl;
    for (size_t i = 0; i < game.players.size(); i++) {
        Player player = game.players[i];
        cout << player.name << ": " << player.roleName << endl;
    }
}

void gameLogic(Game& game) {
    bool mafiaWins = false;
    bool citizenWins = false;

    while (!mafiaWins && !citizenWins) {

        cout << "Город засыпает. Просыпается мафия. " << endl;
        int victimIndex;
        do {
            cout << "Выберите жертву: ";
            cin >> victimIndex;
            victimIndex--;
        } while (victimIndex < 0 || victimIndex >= game.players.size() || !game.players[victimIndex].alive);
        game.players[victimIndex].alive = false;

        bool hasDetective = false;
        for (size_t i = 0; i < game.players.size(); i++) {
            if (game.players[i].roleName == game.detectiveRole.name && game.players[i].alive) {
                hasDetective = true;
                break;
            }
        }
        if (hasDetective) {
            cout << "Мафия засыпает. Просыпается детектив." << endl;
            int suspectIndex;
            do {
                cout << "Выберите подозреваемого: ";
                cin >> suspectIndex;
                suspectIndex--;
            } while (suspectIndex < 0 || suspectIndex >= game.players.size() || !game.players[suspectIndex].alive);
            if (game.players[suspectIndex].roleName == game.mafiaRole.name) {
                cout << "Детектив угадал!" << endl;
                game.players[suspectIndex].alive = false;
            }
            else {
                cout << "Детектив ошибся." << endl;
                game.players[suspectIndex].alive = false;
            }
        }

        bool hasDoctor = false;
        for (size_t i = 0; i < game.players.size(); i++) {
            if (game.players[i].roleName == game.doctorRole.name && game.players[i].alive) {
                hasDoctor = true;
                break;
            }
        }
        if (hasDoctor) {
            cout << "Детектив засыпает. Просыпается доктор." << endl;
            int patientIndex;
            do {
                cout << "Выберите пациента: ";
                cin >> patientIndex;
                patientIndex--;
            } while (patientIndex < 0 || patientIndex >= game.players.size());

            if (game.players[patientIndex].alive) {
                cout << "Данный человек не нуждается в лечении." << endl;
            }
            else {
                game.players[patientIndex].alive = true;
                cout << "Доктор вылечил игрока." << endl;
            }
        }

        for (size_t i = 0; i < game.players.size(); i++) {
            if (game.players[i].roleName == game.citizenRole.name && game.players[i].alive) {
                cout << "Город просыпается. Граждане подозревают мафию в игроке: ";
                int suspectIndex;
                do {
                    cin >> suspectIndex;
                    suspectIndex--;
                } while (suspectIndex < 0 || suspectIndex >= game.players.size() || !game.players[suspectIndex].alive);
                if (game.players[suspectIndex].roleName == game.mafiaRole.name) {
                    cout << "Граждане угадали, что " << game.players[suspectIndex].name << " - мафия." << endl;
                    game.players[suspectIndex].alive = false;
                }
                else {
                    cout << "Граждане не угадали, что " << game.players[suspectIndex].name << " - мафия." << endl;
                    game.players[suspectIndex].alive = false;
                }
                break;
            }
        }

        int mafiaCount = 0;
        int citizenCount = 0;
        for (size_t i = 0; i < game.players.size(); i++) {
            if (game.players[i].roleName == game.mafiaRole.name && game.players[i].alive) {
                mafiaCount++;
            }
            if (game.players[i].roleName == game.citizenRole.name && game.players[i].alive) {
                citizenCount++;
            }
        }
        if (mafiaCount >= citizenCount) {
            mafiaWins = true;
        }
        if (mafiaCount == 0) {
            citizenWins = true;
        }

        cout << "Текущие результаты:" << endl;
        for (size_t i = 0; i < game.players.size(); i++) {
            Player player = game.players[i];
            cout << player.name << ": ";
            if (player.alive) {
                cout << "Жив" << endl;
            }
            else {
                cout << "Мертв" << endl;
            }
        }

        cout << "Проверка результатов игры..." << endl;
        Sleep(1000);
    }

    if (mafiaWins) {
        cout << "Мафия победила!" << endl;
    }
    else if (citizenWins) {
        cout << "Граждане победили!" << endl;
    }
}

void printResults(Game& game) {
    cout << "Результаты игры:" << endl;
    for (size_t i = 0; i < game.players.size(); i++) {
        Player player = game.players[i];
        cout << player.name << ": ";
        if (player.alive) {
            cout << "Жив" << endl;
        }
        else {
            cout << "Мертв" << endl;
        }
    }
}

int main() {
    SetConsoleCP(1251);
    setlocale(LC_ALL, "Russian");
    Game game;

    game.mafiaRole.name = "Мафия";
    game.citizenRole.name = "Гражданин";
    game.detectiveRole.name = "Детектив";
    game.doctorRole.name = "Доктор";

    string name;
    do {
        cout << "Введите имя игрока (или пустую строку для завершения). Игру невозможно начать, если колличество игроков меньше 6: ";
        getline(cin, name);
        if (name != "") {
            addPlayer(game, name);
        }
    } while (name != "" || game.players.size() < 6);

    chooseRole(game, game.players[0]);

    distributeRoles(game);

    printRoles(game);

    gameLogic(game);

    printResults(game);

    return 0;
}
