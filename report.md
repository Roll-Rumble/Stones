# Roll Rumble Project Report

## Introduction

Our game is called Roll Rumble and was developed in our own game engine using OpenGL for rendering, our own game and physics engine alongside a custom database implementation.

Our game was inspired by wooden maze games that we played as children. We decided to create a virtual version using the FPGA and AWS that allows you to play with your friends and race to the finish line. 
Our game features low latency servers and FPGA input which allows for smooth control of the ball on screen as this was important to the user experience. 

The overall goal of the game is to move from the start location of the maze and traverse it whilst avoiding the holes that block your path. If you do fall into a hole you will be reset to the beginning of the maze and have to restart. In this way you can race against a friend to reach the exit of the map but you must combine speed with precision as falling into a hole has devastating consequences, but slowing down too much will make you easy to overtake.

## System architecture

<p align="center">
  <img src="full_system_architecture.png" alt="Fig 1.1: Full System Architecture Diagram" style="width: 90%;"/>
  <br>
  <em>Fig 1.1: Full System Architecture Diagram</em>
</p>

The system architecture, as shown in Fig 1.1, depicts interactions between two clients, each controlling a ball in the game through a DE10-Lite board. The accelerometer on the board sends data to the Nios II processor, which then filters the data using a 49-taps hardware FIR filter on the board. This filtered data guides the motion of the ball in our fully custom game engine, running at a 20 FPS tick rate, handling physics; as well as rendering via OpenGL.

## Design decisions

We have decided to make our own FIR filter. We wrote a custom verilog file and connected it using PIO. This way all the multiplications and additions are happenning in paralell. 

Because of our custom FIR hardware filtering, the response from the FPGA is very fast. However, at some point a further increase in the sampling rate becomes unnoticeable and no longer practical. Therefore, we have decided to set our controller polling rate to 1 kHz. This results in a latency of 1 ms and it is better than for example an Xbox controller which has a polling rate of 125 Hz. 

We decided to write as much code as possible from scratch in C++ to gain the best
understanding of how networking and game development work behind the scenes and
to have low latency code.

We used CMake to compile our code on the Windows client and Linux server because of its
cross-platform capabilities.
This enabled us to reduce code duplication between client and server.

For example, the basic networking with our encoding
and decoding functions were used on both client and server.
Using CMake we put this code
in subdirectories and compiled it into static libraries that were
linked with both the server and client.
To make sure we don't compile the graphics code and other things unnecessary code on the server
we added `#ifdef CLIENT_COMPILE` into the library code to only compile this
if `CLIENT_COMPILE` was set, which is done in our `CMakeLists.txt`
file by automatically detecting the host platform on which the compilation is done.


For writing graphics from scratch we chose OpenGL over DirectX since it is cross-platform.
To open an OpenGL context we used the GLFW library because this provides platform-independent
functions for creating a context (using Win32 API on windows and Xorg on Linux).
The other prerequisite for writing OpenGL code is getting access to the functions
inside the driver of our graphics card.
To get access to the OpenGL functions in our graphics card driver we needed to have headers for the function definitions and link to the correct
functions. This was done with the GLEW library.

For the actual graphics programming we had to generate vertex buffers for every
shape we want to draw and then write two shaders in the Graphics Library Shader Language (GLSL):
A *vertex shader* that transforms the coordinates we
specified into coordinates OpenGL understands using linear algebra with OpenGL matrices.
A *fragment shader* that draws the part of the texture specified in the vertex
buffer on the screen between the given vertex coordinates.

To generate the textures, i.e. OpenGL matrices, from PNG images we used code
from the `nothings/stb` Github repository: <https://github.com/nothings/stb>.

Locking the frame rate has two main benefits: simplifing physics calculations and reducing the load on the server. Physics calculations now have a constant frame time for updating positions. Since the client updates the server with its position after rendering each frame, limiting the framerate reduces the bandwidth of the packets sent by the client, reducing the UDP packet loss ratio. The game uses a frame rate of 20 because this is enough to have the motion of the balls be visibly smooth, while having an extremely low UDP packet loss ratio.

Most online multiplayer games have a fixed rate at which they update the server-side gamestate and send updates to the client, which helps to ensure fairness between clients and prevent the server being overloaded. Roll Rumble processes client packets as soon as they are received, then immediately updates the other client. This is more efficient for the game because it reduces latency, and since there are only two clients overloading the server is not a worry.

The database was a custom nosql design that allowed a very lightweight and fast read/write. It is composed of a C++ class which manages the db and is capable of reading and writing to specific Game Ids whilst tracking the amount of games played and storing their replay data. The data itself is stored in a json file which is formatted for maximum machine readability. Using these files a replay can be retrieved very quickly and sent over TCP. We tested the database by storing a large amount of data and making sure it was retrieved without error by comparing it to the original data, all within a reasonable time. The replays are retrieved fully from a single command and requires only a Game Id, after this the database functions sort packing and sending of data with clientside functions to unpack and remake the replay in its original form. These functions were all tested extensively for errors at a component and system level throughout.

We used multithreading to make sure that a live game was not impacted by the logging of data. Each client has its own thread running on the server that deals with appropriately storing the data. This enables even more performance increases and prevents bottlenecking from logging.

Communication occurs via both UDP and TCP connections. TCP ensures secure initial connection setup, while UDP provides low-latency updates for game data.

On the server, the main thread receives UDP data from clients, updating player positions, processing game logic, and writing position data to a database. Separate threads handle database access for each client to maintain responsiveness. The main thread synchronizes data between clients for multiplayer functionality. 

## Performance metrics

We have tested our implementation of the custom hardware FIR against our heavily optimised software implementation. The table below is the average of 3000 measurements. 

| Filter Type             | Clock Cycles | Resulting delay (ms)|
|-------------------------|--------------| -----|
| Optimised Software FIR     | 136,631      | 2.7|
| Custom Hardware FIR     | 1,592        | 0.0 |

The resulting delay was calculated in mind with the 50 MHz clock associated with Nios II and kept to a precision to one decimal digit. 
This custom FIR is 86 times faster than our already optimised software implemenataion. Therefore, the delay stemming from the FPGA controller is a non-issue. 

- The average total input latency, between moving the controller, and the ball moving on the screen is 138ms.
- The client maintains a consistent 20 FPS (could likely be pushed much higher)
- The average round-trip time over UDP between the client and server is 6.6ms
- The average UDP packet loss ratio is just 0.02% for communications between the client and server

## Testing

- To test communication between the NIOS II and the client PC, 100 messages were sent from the NIOS II to the host PC and checked to ensure correctness.
- To test communication from the client to the server, 100 messages were sent from the client to the server, and checked to ensure correctness. The same was then done with the server returning the values to the client to test communication from the server to the client.

When testing the game we payed close attention to test all possible actions the
players might take.

First of all we made sure that both clients don't show the game menu until
the other client is connected to the server.

In the main menu there are two actions: Either start the game with the
"Start Game" button or look at replays from the database by clicking the
"Replays" button.
We tested the menu by first letting both clients press "Start Game" which made
each of them spawn in the map and also display the other ball.
Next we tested the database menu by letting both clients go into the "Replays"
menu. Here, we were able to let them both watch the same replay or different ones
which proves that the multithreaded database request handling on the server worked
as expected.

One edge case would be to let one player press "Start Game" while the other player
clicks on "Replays".
Since we did not implement functionality to make both clients wait until
the other one is viewing the map, this allowed the player who clicked "Start Game"
to already play while his opponent was still at the startup position.
This was expected.
However, watching replays while one player already plays worked, correctly.

For gameplay testing we tried all kinds of collisions with the walls to check whether we could get
a ball stuck.
Moreover, we tested all of the holes to see if they moved the player back to
the starting position.
Lastly, we let the players finish in different orders to check whether it would
always show "You have won!" and "You have lost!" to the correct player.

This brings us to the last part of system level testing: the end screen.
We verified that the "Restart Game" button correctly puts both players in the
map view and that the server correctly reset their positions to the start position.
By viewing the files of the database we also verified that this correctly started
recording a new game while saving the old one.
For the "Replays" menu we did the same testing as before trying to let both watch
different and the same replays at the same time.
Finally, we verified that the "Exit" button closes the game window.

### INCLUDE TESTING DIAGRAM