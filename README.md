# ThirdPersonRPG

Third person shooter prototype inspired by Horizon Zero Dawn prototypes and final game

Warning - Project repository is missing two folders - StarterContent, and ParagonLtBelica to reduce size
Both are free assets. Get them in the Epic marketplace, import them into an empty project, and then copy-paste into the content folder of this project for the project to work properly

## Video - 
[![Horizon Inspired Prototype Divij Sood](https://img.youtube.com/vi/3hIgYaVVGgs/0.jpg)](https://www.youtube.com/watch?v=3hIgYaVVGgs)


## Features

Character (ThirdRPGCharacter) - 

- Move and jump
- Dodge sideways to avoid fire (Left Ctrl or Left face button on gamepad)
- Zoom in to take aim (Right click or Left trigger)
- Shoot (Left click or Right trigger)
- Place trap (F key or Right face button on gamepad)
- Health
	
Enemy - 
-Modular/procedural enemy. Starts with a core block. The final "assembled" enemy varies in shape and size.
AI controller allows the enemy to patrol or seek the player. The player needs to be close enough for the AI to detect it.
Core block (EnemyPawn) spawns pieces and attaches them to its faces. If the core is destroyed, the whole robot goes down.

- Modules (EnemyModule) - Each module has its own function. They are attached to the pawn and fall off when destroyed.
	- Holder (grey) - Spawns and attaches more modules
	- Armour (black) - Protects the core
	- Weak spot (yellow) - Has low HP and may explode when destroyed, damaging modules near it
	- Weapon (red) - Machine gun - Fast, weak projectiles
	- Weapon (red) - Missiles - Big, slow projectiles that explode and can blow up cover/obstacles
	
- AI - AI controller (EnemyAIController) that lets the enemy patrol or seek the player
- Health - each piece has its own health.
	
