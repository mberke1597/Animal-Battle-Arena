Animal Battle Simulation Project
This project simulates battles between teams of animals in different arena environments. It demonstrates object-oriented programming principles in C++, including inheritance, polymorphism, and interfaces.
How to Run the Project
To compile and run this C++ project, you will need a C++ compiler (like g++).
1. Save the code:
Save the provided C++ code into a file named animal_battle.cpp (or any other .cpp extension).
2. Compile the code:
Open your terminal or command prompt and navigate to the directory where you saved animal_battle.cpp. Then, compile the code using a C++ compiler. For g++, the command would be:
g++ animal_battle.cpp -o animal_battle -std=c++11

   * g++: The command to invoke the GNU C++ compiler.
   * animal_battle.cpp: Your source code file.
   * -o animal_battle: Specifies the output executable file name as animal_battle.
   * -std=c++11: Ensures the code is compiled using the C++11 standard or later, which is necessary for features like std::move.
   3. Run the executable:
After successful compilation, you can run the executable:
./animal_battle

The program will then execute the battle simulation, printing the various stages of the battle, animal statuses, and final outcomes to the console.
Design Choices
The project's design is based on several object-oriented principles to create a flexible and extensible simulation.
1. Object-Oriented Principles
      * Encapsulation: Each class (e.g., Animal, Team, BattleArena, Trainer) encapsulates its own data (attributes) and behavior (methods), exposing only what is necessary through public interfaces. This promotes modularity and reduces dependencies.
      * Inheritance: The Animal class serves as an abstract base class, defining common properties (name, health, attackPower) and behaviors (performAttack, display, takeDamage, isAlive). Specific animal types (e.g., Lion, Elephant, Eagle, Shark) inherit from Animal, providing their unique implementations for virtual functions.
      * Polymorphism: Through virtual functions and pointers/references to the base Animal class, different animal types can be treated uniformly. For example, Team can store Animal* pointers and call performAttack or display on them without knowing their specific derived type at compile time. This allows for dynamic behavior based on the actual object type during runtime.
2. Interfaces (Abstract Base Classes)
      * Separation of Concerns: Interfaces (Flyable, Swimmable, Runnable) are used to define specific capabilities that animals might possess, independent of their core "animal" nature. This allows for clear contracts: any class implementing Flyable must provide a fly() method.
      * Multiple Inheritance for Capabilities: C++ allows multiple inheritance. By inheriting from Animal and one or more capability interfaces, animals can gain specific movement types (e.g., Eagle is Animal, Flyable, and Runnable). This is a powerful way to model diverse behaviors without creating a deep, rigid inheritance hierarchy.
      * Arena Rules: The applyArenaRules method in the Team class leverages these interfaces using dynamic_cast. This allows the arena to check if an animal possesses a certain capability (e.g., Swimmable for a "Sea" arena) and eliminate those that don't, making the arena logic flexible and extensible for new animal types or arena rules.
3. Stat Management
      * health and attackPower (Current Battle Stats): These represent the animal's current stats during a specific battle. They change as damage is taken or temporary buffs are applied.
      * currentBaseHealth and currentBaseAttackPower (Persistent Base Stats): These store the animal's baseline stats, which include initial values plus any permanent buffs applied by a Trainer. This is crucial for resetStats(), ensuring animals return to their buffed state after each battle, rather than their original, unbuffed state.
      * resetStats() Method: This method in the Animal class allows animals to fully recover their health and attack power to their currentBase values between battles, ensuring each battle starts with fresh, persistent stats.
4. Team and BattleArena Logic
      * Team Class: Manages a collection of Animal* pointers. It handles adding animals, displaying team status, calculating total health, and orchestrating attacks on an enemy team. The hasLivingAnimals() and getFirstAliveAnimal() methods are essential for managing battle flow.
      * BattleArena Class: Orchestrates the entire battle process.
      * It takes two Team* objects.
      * It resets animal stats at the beginning of each battle.
      * It randomly selects an arena type (Sea, Air, Ground).
      * It applies arena-specific rules, eliminating animals unsuitable for the current environment.
      * It manages battle rounds, with teams attacking each other until one team has no living animals.
      * It records battle winners in a history, allowing for tracking overall performance.
      * Randomness: srand(time(0)) is used once at the start of main to seed the random number generator, ensuring different arena types are chosen for each battle.
5. Memory Management (Pointers)
      * The current implementation uses raw pointers (Animal*). While this works for the simple example, in a larger application,
This design provides a solid foundation for a turn-based battle simulation, allowing for easy expansion with new animal types, abilities, and arena rules.