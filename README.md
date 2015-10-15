# Virtual Realty

A revamped and reimagined version of the classic board game, Monopoly, where a single user is embedded in a virtual reality board game against computer players. The game includes not only the physical board which is traversed autonomously by robotic pieces, but also the virtual board, where the player can virtually explore the game board and surroundings. Both the physical board and the virtual game work in tandem to preserve all the rules and components of the original Monopoly board game, while also giving the user the opportunity to explore the virtual world developed on the Oculus Rift through multiple vantage points. The virtual game will include visualization for the player’s properties, money, houses/hotels, and current board position. There will also be functions requiring user interface for rolling dice, reading chance/community chest cards, buying properties, and exploring the game board through both a “bird’s eye” view and a “walk-through” view. 

## Robot Requirements

1. 3x3x3” size limit for robot’s internal circuitry, with a tolerance for extra space for wheels

2. Robots must be capable of a speed of 4 in/s (1/3 ft/s)

3. All of the Robot’s movements must be steady and continuous

4. Robot’s must be able to communicate via bluetooth with the PC terminal

5. Robot’s power supply must last 2 hours without recharge or battery replacement

6. Robots only move forward around the board, with no diagonal or reverse motion

7. Robots must be perform obstacle detection for other pieces within 1-3in. of their position

8. Robots must be able to reroute a path to its destination to avoid collisions with any obstacles it detects

9. Two robots must be able to occupy the same game space at the same time

10. Robots must have a clear identification method to discern between each player’s game piece

11. Robots must be capable of 90° turns to round board corners with a tolerance of +/-5°

12. Robots must know what game space they are currently occupying

13. Robots must use a Kinetis Freescale processor 

## Oculus Requirements

1. The Player must be able to view game play from a “bird’s eye view”

2. In “bird’s eye view” the properties owned by the player must be reflected

3. A secondary “piece view” must be available for the user to explore the board, pieces, and surroundings

4. In “piece view” the player must be on the order of 10x the height of the pieces and the houses

5. All standard game assets in the original monopoly game (money, property, houses, hotels, and chance/community chest cards) must be tracked and apparent to the player

6. All game assets, besides robots (pieces) are represented only in the Oculus

7. Money held by the user must be apparent at all times

8. Property ownership must be reflected in bird’s-eye view

9. The user must be allowed to purchase property as per the original monopoly rules
