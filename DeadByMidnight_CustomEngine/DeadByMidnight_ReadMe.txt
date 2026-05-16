========== BLOATWARE ==============
By: Dead By Midnight

Team Roster:

CAI Wenjin (RTIS)
Primary Role: Programmer
Champion: Graphics

SIM Mong Chuan (RTIS)
Primary Role: Product Manager
Secondary Role: Programmer
Champion: Production, Debugging

TEO Wen Xi (RTIS)
Primary Role: Technical Lead
Champion: Engine

HEAR Li Heng (IMGD)
Primary Role: Programmer
Champion: Level Design

SIA Xin Tian (IMGD)
Primary Role: Programmer
Champion: Physics

TAN Kang Zheng (IMGD)
Primary Role: Programmer
Champion: Mechanics

LOW Jovan Zhuo Wen (UXGD)
Primary Role: Designer 
Champion: Art VFX

WONG Weng Kit, Zephan (UXGD)
Primary Role: Design Lead
Secondary Role: Art Lead
Champion: Game Music, Art UI Design



--- Game Concept Synopsis ---
You play as a hacker-virus blob, armed with the power to change sizes and conquer physics-based challenges within an AI's virtual world.

***NOTE
The game must be played on a plugged in computer. Playing on an unplugged computer may cause poor CPU performance and cause unexpected behaviour and bugs.

Alt-Tab will cause unexpected behaviour and bugs.

Ctrl + Alt + Delete will cause the game to come back in windowed mode, press Alt + Enter to reenter full screen mode

Audio is not paused in the pause menu, triggering the pause menu will cause misalignments with the audio and the visuals, this is not a bug but a technical limitation.

Pause Menu in boss level is scaled differently due to a camera zoom level difference from the main screen this is a technical limitation and not a bug or error in sizing.

In black-border cutscenes, the player will be suspended for a few seconds, this is not a bug. 

Gameplay video has some clipping issues same as previous milestones, cause is still unknown.

BGM will not play in main level if player presses '1' in the tutorial section, this is intentional and not a bug.


Laser beam SFX will cut off if player moves to the next stage while the laser beam is in the midst of firing it is not an audio bug and it is intentional.

In the Boss level, when player is killed by a laser beam or laser, the Boss will protrude further away from the left edge of the screen for a brief moment when the player spawns. This is to prevent the player from being blocked by the Boss. To prevent soft-locks, the AI will turn translucent and not kill the player on contact nor fire any lasers. The AI will revert to being opaque and hazardous to the player when the player is far enough from the left edge of the screen so that the player does not die continuously and get trapped behind the AI.


There are two exceptions to the above rules.

One, in the first segment of the Boss Stage, as the player's respawn point is far enough from the left edge of the screen, the Boss will never become translucent during that segment.

Two, in the last segment of the Boss Stage, the AI will not protrude from the left edge of the screen upon killing the player with its laser, and the distance threshold for it to revert back to being opaque and hazardous in this segment is shorter than the other segments prior to this.






Features:
- Jump Leniency player will teleport above the wall if the collision is just nice same as M2.


Controls 

Default Keyboard:
Move Left - Press Left Arrow
Move Right - Press Right Arrow
Jump - Press Up Arrow
Shrink - Space, Hold to enable, Release to revert back
Jump Boost - Press Up Arrow + Space together to get maximum jump boost
Pause Menu - Esc Key to trigger and exit
Quick Restart - Press R Key (kills player and resets its position)
Pause Menu, Win Condition and Start Menu Navigation Key - Use Up and Down arrow keys to navigate
Pause Menu, Win Condition and Start Menu Select Key - "Enter" key
Cycle between windowed and full-screen mode - Alt + Enter

Keyboard Cheats:
Skip Opening Cutscene at 10x Speed - Hold the 'O' Key OR the 'Enter' Key
Skip To Last Levels Of Main and Boss Level - Press '1' Key


Shortcuts:
FPS Counter - Press 'G' to toggle the FPS counter

Default Controller:
Movement - Directional Pad
Jump - Press 'X' button
Shrink -  Press 'R1' button

Controller Cheats:

Skip Opening Cutscene at 10x Speed - Hold the 'Triangle' Key 
Skip to last level of map - Press 'L1' button




--- Game Objects ---

//Player Character 
You play as a black-colored hacker-virus blob with 2 eyes.

//Rogue A.I Antagonist
The giant iris that floats in the background while taunting the player through dialogue. It has a Normal Mode and a Boss Mode, starting in its Normal Mode. Whenever a dialogue trigger is activated, it will play voiceover dialogue lines with audio and visible text at the bottom of the screen. Dialogue triggers can activate from the player reaching certain areas, repeated deaths that vary between the Main Level and the Boss Scene, and scripted cutscenes. 

When the player transitions from the Main Scene to the Boss Scene, it will change from its Normal Mode to its Boss Mode, where it will now stay on the left side of the screen and aim to follow the player's Y-Axis position. Every few seconds, if the player is within a certain range on the Y-Axis from the A.I, it will fire a horizontal laser across the screen that kills the player. The cooldown for this laser is higher than normal if it manages to kill the player with this attack.

//Dialogue Text
These will appear when the A.I. is speaking, over a translucent black box.

//Tiles
Translucent Platforms with White High-Tech Texture lines. When the A.I Antagonist is in its Boss Mode, these will glow red.

//Laser
Red Glowing Lines, touching them kills the player and sends them to their last checkpoint AKA start of current level

//Wind tunnels 
Purple areas, the player can be carried by them if they Shrink, will not affect normal-sized character

//Key & Door 
Key is green in colour. Must collect green keycard without dying to be able to unlock the door. Door will be partially red if it has not been unlocked yet and turn green when the key card is collected.

//Pause Menu 
Use Up and Down arrow keys to scroll through options. Press Enter to select, press Esc to toggle pause menu on and off.

//Audio Control Menu
Able to adjust master, bgm and sfx channels. Up and Down arrow keys to scroll through the menu, Left and Right arrow keys to adjust volume of selected channel.

//Key Binding Menu
Ability to rebind keys. 'O', '1', "ENTER" and 'G' keys cannot be used as they are reserved for cheats.

//Win Condition 
Use Up and Down arrow keys to scroll through options. Press Enter to select. Choosing "Restart" will take you to the main menu.

Pause Menu Options: 
"Resume Game" - Resumes game
"How To Play" - Brings up an image that teaches you how to play the game
"Quit Game" - Brings up a confirmation message, If user clicks "Yes" application will close, if user clicks "No" confirmation message will close

--- Audio ---
//Voiceover 
As part of the narrative, the voice over dialogues will trigger during cutscenes or when the player dies.

--- Scenes ---
//MAIN MENU
Players can choose "Start Game" to start the game, "Quit Game" to quit the game from this scene, "Volume" to adjust the game's master/bgm/sfx audio levels, and "Controls" to rebind the game's controls.

//OPENING DIALOGUE CUTSCENE
This will be the first scene that the player sees when they select “Start Game”. It is entirely a text-based cutscene. At first, it will represent the protagonist booting up their virtual console to access a fictional equivalent of the Dark Web to load an e-mail sent by one of their associates. After that, the text will clear and load new text from their associate's e-mail, requesting the player to enter the simulation to stop AXIOM.

//TUTORIAL
After the opening dialogue cutscene, players will be sent to this section. In this section, players will be introduced to the AXIOM the A.I, running, jumping, shrinking, jump boost and the wind tunnel.

//FIRST IN-GAME CUTSCENE
When the player completes the tutorial and crosses the first green door, they will die suddenly and a cutscene starts. This is to establish a clear differentiation between the tutorial and the actual levels as well as to flesh out narrative. During the cutscene, the player will not be able to move. In this cutscene the player will be suspended for a few seconds. A bigger and different revive animation will play here but has not been implemented this is not a bug.

After a few lines of voiceover dialogue, the player will be released and they will be able to start the actual gameplay.

//MAIN SCENE
Once the player is released they will face a series of challenges created to test the player’s ability to control the size manipulation and jumping mechanics. In these challenges the user will be tasked with maneuvering past a series of lasers, collecting the key and opening the green door to complete a level.

When locked, the green door will be partially red and completely green when a key is collected to indicate that the player is now able to pass through the door.

//SECOND IN-GAME CUTSCENE
At the end of the Main Scene, the player will enter another scripted cutscene, coming face to face with a giant green door and moving on their own to the right to trigger it as AXIOM pleads with the player not to do so. After that, the player will transition from the Main Level to the Boss Scene, where the cutscene continues as AXIOM is now enraged by the player's actions and enters its Boss Mode, giving the entire game's surroundings a reddish hue.

//BOSS SCENE
Similar to the Main Scene, the player will need to overcome a series of challenges to complete each level, but this time, in every level, AXIOM will periodically fire a horizontal laser that will kill the player if they are within its vertical detection range.

//ENDING CUTSCENE AT THE LAST SECTION OF THE BOSS SCENE
In this scripted cutscene, the player has finished all levels and is activating the fail-safe protocol, causing the game world to slowly explode. Once the screen completely turns white, the player will be taken to the Credits Scene.

//CREDITS SCENE
The game's title will appear, and shortly after, the screen will scroll down along with all of the game's credits. Images taken related to the game's development will appear on the left and right. At the very bottom, a death counter tallying all of the player's deaths from the start of the game will appear, and afterwards, the player will be sent to the Ending Dialogue Cutscene. To speed through the credits, hold the 'O' key OR the 'Enter' key.

//ENDING DIALOGUE CUTSCENE
The player will be sent here after the Credits. Similar to the Opening Dialogue Cutscene, it is entirely a text-based cutscene. The player will be congratulated for beating the game and stopping AXIOM, before being sent back to the main menu.


