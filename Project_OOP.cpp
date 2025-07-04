#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime> 


class Animal;

// Interfaces
class Flyable {
public:
    virtual ~Flyable() = default; 
    virtual void fly() = 0;
};

class Swimmable {
public:
    virtual ~Swimmable() = default;
    virtual void swim() = 0;
};

class Runnable {
public:
    virtual ~Runnable() = default;
    virtual void run() = 0;
};

// Animal Base Class
class Animal {
protected:
    std::string name;
    double health;                  // Current health for the battle
    double attackPower;             // Current attack power for the battle

    double currentBaseHealth;       // Health to reset to (initial + permanent buffs)
    double currentBaseAttackPower;  // Attack power to reset to (initial + permanent buffs)

public:
    Animal(std::string n, double h, double a) :
        name(std::move(n)),
        health(h),
        attackPower(a),
        currentBaseHealth(h),
        currentBaseAttackPower(a) {
    }

    virtual ~Animal() = default; // Destructor for base class with virtual functions

    virtual void performAttack(Animal* target) = 0;
    virtual void display() const = 0;

    virtual bool isAlive() const { 
        return health > 0;
    }

    double getHealth() const {
        return health;
    }
    double getAttack() const {
        return attackPower;
    }

    // Sets current health for this battle 
    void setCurrentHealth(double h_val) {
        health = h_val;
        if (health < 0) health = 0;
    }

    // Sets current attack power for this battle
    void setCurrentAttackPower(double ap_val) {
        attackPower = ap_val;
    }

    // For trainer buffs, modifying the baseline for resets
    void buffBaseHealth(double amount) {
        currentBaseHealth += amount;
        health = currentBaseHealth; // Update current health to new base immediately
        std::cout << name << " base health buffed by " << amount << ". New base HP: " << currentBaseHealth << std::endl;
    }

    void buffBaseAttackPower(double amount) {
        currentBaseAttackPower += amount;
        attackPower = currentBaseAttackPower; // Update current AP to new base immediately
        std::cout << name << " base attack buffed by " << amount << ". New base AP: " << currentBaseAttackPower << std::endl;
    }

    void takeDamage(double dmg) {
        health -= dmg;
        if (health < 0) health = 0;
        std::cout << name << " takes " << dmg << " damage. Current health: " << health << std::endl;
    }

    std::string getName() const {
        return name;
    }

    void resetStats() {
        health = currentBaseHealth;
        attackPower = currentBaseAttackPower;
         std::cout << name << " has been reset. HP: " << health << ", AP: " << attackPower << std::endl;
    }
};

// Concrete Animal Classes
class Lion : public Animal, public Runnable {
public:
    Lion(std::string n, double h, double a) : Animal(std::move(n), h, a) {}
    void performAttack(Animal* target) override {
        std::cout << getName() << " bites " << target->getName() << "!" << std::endl;
        target->takeDamage(getAttack());
    }
    void run() override {
        std::cout << getName() << " is running!" << std::endl;
    }
    void display() const override {
        std::cout << getName() << " (Lion) - Health: " << getHealth() << ", Attack: " << getAttack() << (isAlive() ? "" : " (Defeated)") << std::endl;
    }
};

class Elephant : public Animal, public Runnable {
public:
    Elephant(std::string n, double h, double a) : Animal(std::move(n), h, a) {}
    void performAttack(Animal* target) override {
        std::cout << getName() << " attacks " << target->getName() << " with ivory!" << std::endl;
        target->takeDamage(getAttack());
    }
    void run() override {
        std::cout << getName() << " is running!" << std::endl;
    }
    void display() const override {
        std::cout << getName() << " (Elephant) - Health: " << getHealth() << ", Attack: " << getAttack() << (isAlive() ? "" : " (Defeated)") << std::endl;
    }
};

class Eagle : public Animal, public Flyable, public Runnable {
public:
    Eagle(std::string n, double h, double a) : Animal(std::move(n), h, a) {}
    void performAttack(Animal* target) override {
        std::cout << getName() << " attacks " << target->getName() << " from the air!" << std::endl;
        target->takeDamage(getAttack());
    }
    void fly() override {
        std::cout << getName() << " is flying!" << std::endl;
    }
    void run() override {
        std::cout << getName() << " is running (on land)!" << std::endl;
    }
    void display() const  override {
        std::cout << getName() << " (Eagle) - Health: " << getHealth() << ", Attack: " << getAttack() << (isAlive() ? "" : " (Defeated)") << std::endl;
    }
};

class Shark : public Animal, public Swimmable {
public:
    Shark(std::string n, double h, double a) : Animal(std::move(n), h, a) {}
    void performAttack(Animal* target) override {
        std::cout << getName() << " bites " << target->getName() << " in the sea!" << std::endl;
        target->takeDamage(getAttack());
    }
    void swim() override {
        std::cout << getName() << " is swimming!" << std::endl;
    }
    void display() const override {
        std::cout << getName() << " (Shark) - Health: " << getHealth() << ", Attack: " << getAttack() << (isAlive() ? "" : " (Defeated)") << std::endl;
    }
};

// Trainer Class
class Trainer {
public:
    void applyPersistentHealthBuff(Animal* a, double buffAmount) {
        if (a) a->buffBaseHealth(buffAmount);
    }
    void applyPersistentAttackBuff(Animal* a, double buffAmount) {
        if (a) a->buffBaseAttackPower(buffAmount);
    }
};

// Team Class
class Team {
private:
    std::vector<Animal*> animals;
    std::string teamName;
public:
    Team() = default; // Default constructor

    void addAnimalToTeam(Animal* animal) { 
        if (animal) animals.push_back(animal);
    }

    void displayTeamStatus() const { 
        std::cout << "--- Team " << teamName << " Status ---" << std::endl;
        if (animals.empty()) {
            std::cout << "No animals in this team." << std::endl;
            return;
        }
        for (const Animal* a : animals) { 
            if (a) a->display();
        }
        std::cout << "Total Team Health: " << calculateTotalHealth() << std::endl;
        std::cout << "-------------------------" << std::endl;
    }

    std::string getTeamName() const {
        return teamName;
    }
    void setTeamName(const std::string& name) {
        teamName = name;
    }

    double calculateTotalHealth() const { 
        double totalTeamHealth = 0;
        for (const Animal* a : animals) {
            if (a && a->isAlive()) { 
                totalTeamHealth += a->getHealth();
            }
        }
        return totalTeamHealth;
    }

    bool hasLivingAnimals() const {
        for (const Animal* a : animals) {
            if (a && a->isAlive()) return true;
        }
        return false;
    }

    // Team attacks the enemyTeam
    void attackEnemyTeam(Team* enemyTeam) { 
        if (!enemyTeam) return;
        std::cout << "\nTeam " << teamName << " is attacking Team " << enemyTeam->getTeamName() << ":" << std::endl;
        for (Animal* attacker : animals) {
            if (attacker && attacker->isAlive()) { 
                Animal* target = enemyTeam->getFirstAliveAnimal();
                if (target) { 
                    attacker->performAttack(target);
                    if (!target->isAlive()) {
                        std::cout << target->getName() << " from Team " << enemyTeam->getTeamName() << " has been defeated!" << std::endl;
                    }
                }
                else {
                    // 
                    break;
                }
            }
       }
  }

    Animal* getFirstAliveAnimal() { 
        for (Animal* a : animals)  {
            if (a && a->isAlive()) return a;
        }
        return nullptr;
    }

    void applyArenaRules(const std::string& arenaType) {
        std::cout << "\nApplying " << arenaType << " Arena rules for Team " << teamName << ":" << std::endl;
        for (Animal* a : animals) {
            if (!a || !a->isAlive()) continue; 

            bool suitableForArena = false;
            if (arenaType == "Sea") {
                if (dynamic_cast<Swimmable*>(a)) suitableForArena = true;
            }
            else if (arenaType == "Air") {
                if (dynamic_cast<Flyable*>(a)) suitableForArena = true;
            }
            else if (arenaType == "Ground") {
                
                if (dynamic_cast<Runnable*>(a) || dynamic_cast<Flyable*>(a)) suitableForArena = true;
            }

            if (!suitableForArena) {
                a->setCurrentHealth(0); 
                std::cout << a->getName() << " is not suited for " << arenaType << " Arena and is eliminated for this battle." << std::endl;
            }
            else {
                std::cout << a->getName() << " is suited for " << arenaType << " Arena." << std::endl;
            }
        }
    }

    void resetAnimalsInTeam() { 
        std::cout << "Resetting animals for Team " << teamName << ":" << std::endl;
        for (Animal* a : animals) {
            if (a) a->resetStats();
        }
    }
};

// BattleArena Class
class BattleArena {
private:
    Team* teamA;
    Team* teamB;
    std::vector<std::string> battleWinners; 
    std::string currentArenaType;

public:
    BattleArena(Team* tA, Team* tB) : teamA(tA), teamB(tB) {
        
    }

    std::string getRandomArena() {
        int choice = rand() % 3;
        if (choice == 0) return "Sea";
        if (choice == 1) return "Air";
        return "Ground"; 
    }

    void startBattle() {
        if (!teamA || !teamB) {
            std::cout << "Error: Teams not properly initialized for battle." << std::endl;
            return;
        }

        std::cout << "\n=========================================" << std::endl;
        std::cout << "      PREPARING FOR A NEW BATTLE!      " << std::endl;
        std::cout << "=========================================" << std::endl;

        // Reset animals in both teams to their base stats
        teamA->resetAnimalsInTeam();
        teamB->resetAnimalsInTeam();

        std::cout << "\nInitial Status before Arena Rules:" << std::endl;
        teamA->displayTeamStatus();
        teamB->displayTeamStatus();

        currentArenaType = getRandomArena();
        std::cout << "\n======= Battle starting in " << currentArenaType << " Arena! =======" << std::endl;

        
        teamA->applyArenaRules(currentArenaType);
        teamB->applyArenaRules(currentArenaType);

        std::cout << "\nStatus after Arena Rules:" << std::endl;
        teamA->displayTeamStatus();
        teamB->displayTeamStatus();

        
        bool teamALives = teamA->hasLivingAnimals();
        bool teamBLives = teamB->hasLivingAnimals();

        if (!teamALives && !teamBLives) {
            std::cout << "\nNeither team has suitable animals for " << currentArenaType << " Arena! It's a DRAW!" << std::endl;
            battleWinners.push_back("Draw (Arena Elimination)");
            return;
        }
        if (!teamALives) {
            std::cout << "\nTeam " << teamA->getTeamName() << " has no suitable animals! Team " << teamB->getTeamName() << " WINS by default!" << std::endl;
            battleWinners.push_back(teamB->getTeamName() + " (Won by Arena Elimination)");
            return;
        }
        if (!teamBLives) {
            std::cout << "\nTeam " << teamB->getTeamName() << " has no suitable animals! Team " << teamA->getTeamName() << " WINS by default!" << std::endl;
            battleWinners.push_back(teamA->getTeamName() + " (Won by Arena Elimination)");
            return;
        }

       
        int round = 1;
        while (teamA->hasLivingAnimals() && teamB->hasLivingAnimals()) {
            std::cout << "\n--- Round " << round << " ---" << std::endl;

            // Team A attacks Team B
            if (teamA->hasLivingAnimals()) { 
                teamA->attackEnemyTeam(teamB);
                teamB->displayTeamStatus(); 
            }

            if (!teamB->hasLivingAnimals()) break; 

            // Team B attacks Team A
            if (teamB->hasLivingAnimals()) {
                teamB->attackEnemyTeam(teamA);
                teamA->displayTeamStatus(); 
            }

            if (!teamA->hasLivingAnimals()) break; 

            round++;
            if (round > 20) { 
                std::cout << "\nBattle exceeds 20 rounds, declaring a draw due to stalemate!" << std::endl;
                battleWinners.push_back("Draw (Stalemate)");
                return;
            }
        }

        // Determine winner
        std::cout << "\n======= BATTLE ENDED =======" << std::endl;
        if (teamA->hasLivingAnimals() && !teamB->hasLivingAnimals()) {
            std::cout << "Team " << teamA->getTeamName() << " WINS!" << std::endl;
            battleWinners.push_back(teamA->getTeamName());
        }
        else if (teamB->hasLivingAnimals() && !teamA->hasLivingAnimals()) {
            std::cout << "Team " << teamB->getTeamName() << " WINS!" << std::endl;
            battleWinners.push_back(teamB->getTeamName());
        }
        else if (!teamA->hasLivingAnimals() && !teamB->hasLivingAnimals()) {
            std::cout << "Both teams were eliminated! It's a DRAW!" << std::endl;
            battleWinners.push_back("Draw (Mutual Elimination)");
        }
        else { 
            std::cout << "It's a DRAW (likely stalemate)!" << std::endl;
            battleWinners.push_back("Draw (Stalemate)");
        }
        teamA->displayTeamStatus();
        teamB->displayTeamStatus();
    }

    void showBattleHistory() const { 
        std::cout << "\n--- Overall Battle History ---" << std::endl;
        if (battleWinners.empty()) {
            std::cout << "No battles have been recorded yet." << std::endl;
            return;
        }
        for (size_t i = 0; i < battleWinners.size(); ++i) {
            std::cout << "Battle " << i + 1 << " Winner: " << battleWinners[i] << std::endl;
        }
        std::cout << "----------------------------" << std::endl;
    }
};


int main() {
   
    srand(static_cast<unsigned>(time(0)));

    // Create Animals
    Shark shark1("Finny", 50, 30);
    Lion lion1("Leo", 100, 45);
    Elephant elephant1("Dumbo", 200, 10);
    Eagle eagle1("Sky", 75, 35);

    Shark shark2("Jaws", 60, 35);
    Lion lion2("Simba", 110, 50);

    // Create Teams
    Team teamAlpha, teamBravo;
    teamAlpha.setTeamName("Alpha");
    teamBravo.setTeamName("Bravo");

    // Add animals to teams
    teamAlpha.addAnimalToTeam(&shark1);
    teamAlpha.addAnimalToTeam(&lion1);

    teamBravo.addAnimalToTeam(&eagle1);
    teamBravo.addAnimalToTeam(&elephant1);
    // teamBravo.addAnimalToTeam(&shark2); 
    // teamAlpha.addAnimalToTeam(&lion2);


    // Apply Trainer Buffs 
    Trainer trainer;
    std::cout << "\n--- Applying Trainer Buffs ---" << std::endl;
    trainer.applyPersistentAttackBuff(&shark1, 25);  // Finny AP: 30 -> 55
    trainer.applyPersistentAttackBuff(&lion1, 10);   // Leo AP: 45 -> 55
    trainer.applyPersistentHealthBuff(&elephant1, 50); // Dumbo HP: 200 -> 250
    std::cout << "----------------------------" << std::endl;


    // Create Battle Arena
    BattleArena globalArena(&teamAlpha, &teamBravo);

    // Start multiple battles
    globalArena.startBattle();
    globalArena.startBattle();
    globalArena.startBattle();

    // Show overall winners
    globalArena.showBattleHistory();

    return 0;
}

