## Introduction
Our game is called Roll Rumble and was developed in our own game engine using OpenGL for rendering, our own game and physics engine alongside a custom database implementation.

Our game was inspired by wooden maze games that we played as children. We decided to create a virtual version using the FPGA and AWS that allows you to play with your friends and race to the finish line. 
Our game features low latency servers and FPGA input which allows for smooth control of the ball on screen as this was important to the user experience. 

The overall goal of the game is to move from the start location of the maze and traverse it whilst avoiding the holes that block your path. If you do fall into a hole you will be reset to the beginning of the maze and have to restart. In this way you can race against a friend to reach the exit of the map but you must combine speed with precision as falling into a hole has devastating consequences, but slowing down too much will make you easy to overtake.

## Physics 
Our implementation of physics is quite simple but worked perfectly for our needs. 
More complicated physics such as adding bounce and friction only made the game more stuttery and a poorer user experience.
Physics encompasses two major elements; hitbox checking and collisions. Hitbox checking is done by using our grid system where we have a clever method for checking tiles.
We split the tile into four quadrants and calculate which one the ball centre must be inside from the position. This then means we must only check three walls for collisions (the three tiles sharing a border with the quadrant the ball is in.) 
As our frame rate is high enough it is not possible for the ball to ever enter a wall entirely in a single frame, because of this we must only check the nearest corner of the tile diagonal to the ball. 
Assuming one of these tiles is a wall we then check the distance from the ball centre to the nearest corner or edge. 
If the distance is ever smaller than the radius of the ball we move the ball to be touching the wall instead of outside.
At this point we have resolved the collision and can move to the next frame.

## Database
The database was a custom nosql design that allowed a very lightweight and fast read/write. It is composed of a C++ class which manages the db and is capable of reading and writing to specific Game Ids whilst tracking the amount of games played and storing their replay data. The data itself is stored in a json file which is formatted for maximum machine readability. Using these files a replay can be retrieved very quickly and sent over TCP. We tested the database by storing a large amount of data and making sure it was retrieved without error by comparing it to the original data, all within a reasonable time. The replays are retrieved fully from a single command and requires only a Game Id, after this the database functions sort packing and sending of data with clientside functions to unpack and remake the replay in its original form. These functions were all tested extensively for errors at a component and system level throughout.
### Multithreading
We used multithreading to make sure that a live game was not impacted by the logging of data. Each client has its own thread running on the server that deals with appropriately storing the data. This enables even more performance increases and prevents bottlenecking from logging.