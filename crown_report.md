## Design decisions

### Implementing everything from scratch in C++

We decided to write as much code as possible from scratch in C++ to gain the best
understanding of how networking and game development work behind the scenes and
to have low latency code.

### CMake for reusing code across client and server

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

### Graphics in OpenGL

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

## Testing

### System level testing

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
