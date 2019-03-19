# ThirdPersonRPG

Third person shooter/RPG prototype inspired by Fortnite and Horizon Zero Dawn

Warning - Project repository is missing two folders - StarterContent, and ParagonLtBelica to reduce size
Both are free assets. Get them in the Epic marketplace, import them into an empty project, and then copy-paste into the content folder of this project for the project to work properly

## Video - 
[![Fortnite and Horizon Inspired Prototype Divij Sood](https://img.youtube.com/vi/hwuLpfL548E/0.jpg)](https://www.youtube.com/watch?v=hwuLpfL548E)


## Features

Character (ThirdRPGCharacter) - 

- Move and jump
- Dodge sideways to avoid fire (Left Ctrl or Left face button on gamepad)
- Zoom in to take aim (Right click or Left trigger)
- Shoot (Left click or Right trigger)
- Place trap (F key or Right face button on gamepad)
- Health
- Build System - Can build wall, floor, or ramp. Build previews/meshes snap and align to a grid. Built walls can be edited, creating a hole in one of 4 corners based on where the player is pointing.
	
Enemy - 
-Modular/procedural enemy. Starts with a core block. The final "assembled" enemy varies in shape and size.
AI controller allows the enemy to patrol or seek the player. The player needs to be close enough for the AI to detect it.
Core block (EnemyPawn) spawns pieces and attaches them to its faces. If the core is destroyed, the whole robot goes down.

- Modules (EnemyModule) - Each module has its own function. They are attached to the pawn and fall off when destroyed.
	- Holder (grey) - Spawns and attaches more modules
	- Armour (black) - Protects the core
	- Weak spot (yellow) - Has low HP and may explode when destroyed, damaging modules near it
	- Weapon (red) - Machine gun - Fast, weak projectiles. Turns slowly to aim towards the player like a turret.
	- Weapon (red) - Missiles - Big, slow projectiles that explode and can blow up cover/obstacles
	
- AI - AI controller (EnemyAIController) that lets the enemy patrol or seek the player
- Health - each piece has its own health.

## Game feel (In-progress)
- Shooting - 
	- Enemy modules flash red when hit	
	- Particle effects on firing and on impact of bullets
- Explosions cause camera shakes based on size of explosion and distance from character
- Modules become physical and drop/fly away when destroyed
- Weak spots are present to make dismantling the enemies faster and more satisfying
	
## Planned improvements
- Grid and noise based procedural generation (Partially implemented) 
	- This will allow for more design and constraints to be applied to the procedural generation
	- Generation can aim to generate humanoid or animalistic enemies of various sizes
	- Different sub-parts with various functions can be generated to make the game more strategic
- Art and animation 
	- Better firing animations for better game feel
