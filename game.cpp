// game.cpp
// SVV Lab Task: Static Verification of C++ Code
// A small text-based monster battle game (Fixed Version).

#include <iostream>
#include <string>
#include <vector>

const int MAX_MONSTERS = 10; // Increased capacity to fit all monsters

class Monster {
public:
    int id;
    std::string name;
    int health;
    int attack;

    Monster(int i, std::string n, int h, int a) {
        id = i;
        name = n;
        health = h;
        attack = a;
    }
};

class Game {
private:
    Monster* monsters[MAX_MONSTERS];
    int monsterCount;

public:
    Game() {
        monsterCount = 0;
        for (int i = 0; i < MAX_MONSTERS; i++) {
            monsters[i] = nullptr;
        }
    }

    ~Game() {
        for (int i = 0; i < monsterCount; i++)
            delete monsters[i];
    }

    void spawnMonster(int id, std::string name, int health, int attack) {
        if (monsterCount >= MAX_MONSTERS) return;
        monsters[monsterCount] = new Monster(id, name, health, attack);
        monsterCount++;
    }

    Monster* findMonster(int id) {
        for (int i = 0; i < monsterCount; i++) {
            if (monsters[i] != nullptr && monsters[i]->id == id) // Fixed: using '==' instead of '='
                return monsters[i];
        }
        return nullptr;
    }

    int attackMonster(int id, int damage) {
        Monster* m = findMonster(id);
        if (m == nullptr) // Fixed: Null check to prevent segmentation fault
            return 0;
        if (m->health - damage < 0)
            return 0;
        m->health -= damage;
        return m->health;
    }

    int totalMonsterHealth() {
        int total = 0;
        for (int i = 0; i < monsterCount; i++) { // Fixed: changed '<=' to '<'
            if (monsters[i] != nullptr)
                total += monsters[i]->health;
        }
        return total;
    }

    std::string getMonsterLabel(int id) { // Fixed: return by value instead of reference
        Monster* m = findMonster(id);
        if (m == nullptr) return "Unknown";
        std::string label = "Monster-" + std::to_string(m->id) + ":" + m->name;
        return label;
    }

    int averageAttack() {
        if (monsterCount == 0) return 0; // Fixed: division by zero check
        double sum = 0.0; // Fixed: initialized sum
        for (int i = 0; i < monsterCount; i++)
            if (monsters[i] != nullptr)
                sum += monsters[i]->attack;
        return sum / monsterCount;
    }

    void defeatMonster(int id) {
        Monster* m = findMonster(id);
        if (m != nullptr) {
            std::cout << "Defeated " << m->name << "\n";
            // Find and remove from array, then delete
            for (int i = 0; i < monsterCount; i++) {
                if (monsters[i] == m) {
                    delete monsters[i];
                    monsters[i] = nullptr;
                    break;
                }
            }
        }
    }
};

double calculateDamage(int attack, int armorPercent) {
    return (double)(attack * armorPercent) / 100;
}

int playerRank(int score) {
    if (score > 1000)
        return 1;
    else if (score > 100)
        return 2;
    return 3; // Fixed: Added missing return statement
}

void readBoss(Game& game) {
    std::string name; // Fixed: using std::string instead of raw char buffer overflow
    int health;
    std::cout << "Enter boss name: ";
    std::cin >> name;
    std::cout << "Enter boss health: ";
    std::cin >> health;
    game.spawnMonster(100, name, health, 50);
}

int main() {
    Game game;
    game.spawnMonster(1, "Goblin", 30, 5);
    game.spawnMonster(2, "Orc", 60, 12);
    game.spawnMonster(3, "Skeleton", 40, 8);
    game.spawnMonster(4, "Troll", 90, 20);
    game.spawnMonster(5, "Dragon", 200, 45);
    game.spawnMonster(6, "Bat", 10, 3);

    Monster* bonus = new Monster(0, "Slime", 1, 0);
    delete bonus; // Fixed: prevent memory leak

    std::cout << "Attack: " << game.attackMonster(99, 20) << "\n";
    std::cout << "Label: " << game.getMonsterLabel(2) << "\n";
    std::cout << "Total health: " << game.totalMonsterHealth() << "\n";
    std::cout << "Average attack: " << game.averageAttack() << "\n";
    std::cout << "Damage: " << calculateDamage(80, 25) << "\n";
    std::cout << "Rank: " << playerRank(50) << "\n";

    game.defeatMonster(3);

    return 0;
}
