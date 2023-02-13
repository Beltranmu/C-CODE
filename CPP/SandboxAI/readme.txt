Members of the group: Ricardo Beltrán Muriel & Javier Benito Abolafio

General Information

  This simple AI project contains:

  - Random Movement
  - Pattern Movement
  - Pacman/Target Movement

  Random Movement: The enemy chooses randomly one direction (NSEW) and moves towards it.
                   This process is repeated endlessly.

  Pattern Movement: The user will define a pattern for every single enemy by using ImGui.

  Pacman/Target Movement: The enemy will try to reach a defined position (which will be editable
                          during runtime) by calculating the Euclidean Distance from three of their
                          different cells: North, East and West. The enemy won't ever be able to
                          move backwards.
  
  ATENTION: The enemy may seem to be overlapping a wall. That's because the enemy's skin is 4 times
            bigger than the hitbox.


How to edit enemies' behavior:

1. Right click on the desired enemy
2. Left click over the ImGui panel to choose the movement: Random, Pattern, Pacman
3. If Pattern:
   3.1. Select Movement (Wait, Forward, Turn 180º, Turn Left 90º, Turn Right 90º)
   3.2. Define the number of movements
   3.3. Add Pattern
   3.4. Click on End Addition Pattern Mode when it is done.
   
   If Pacman:
   3.5. Use the sliders to move the target cell up-down left-right. It is displayed with a green square.
   Warning: The enemy won't move if stuck against the wall and the target cell is at their back.


Map Control:

  There is an ImGui panel where the user can control four elements:
  
  - Input Framerate
  - AI Framerate
  - World Framerate
  - Draw Framerate

  Note: -1 value means unlimited framerate, 0 is unavailable.