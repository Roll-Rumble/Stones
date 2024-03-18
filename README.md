# Roll Rumble Project Report

## Introduction
- highlight purpose of system (describe video game) (Orlan)

(up to here no more than 1/3 of first page)

## System architecture
- board + client + server + client + board
- needs to include at least one diagram of entire system's architecture

(brief overview of how everything connects)

(1 pages with diagram) (Noam)


## Design decisions
- Use hardware filtering on DE-10 lite (Petr)
- 1kHz polling rate for controller input (client multithreading) (Petr)
- Write everything full custom in C++ (optimise for low latency) (Constantin)
- Writing own game engine including OpenGL with 2D shaders and textures (Constantin)
- Lock client frame rate to 20FPS (Ryan)
- Server sends updates to clients as soon as it has them (Ryan)
- Custom database (server multithreading) (Orlan)
- Custom TCP/UDP with timeouts (+ client ID and separate UDP ports on server) (Noam)

(1.5 pages)

## Performance metrics
- processing for FIR filter vs software (Petr)
- input latency (move controller -> ball move) (Ryan other metrics testing)
- average frame rate for client
- client-server latency (ball moves on screen to ball move registered by server)
- client-server-client latency (client can register win to server register win)
- client-client latency (ball moves on one screen to ball moves on the other)
- UDP packet loss ratio

(0.5 pages)

## Testing
- same way that real games are tested - 1hr+ playtime checking for bugs
- testing having 2 separate clients get replay from server

Testing for:
- whole game (Constantin) (basically everything user can see/sense here)
- board-client communication    (TBD who writes these testing bits)
- client-server communication
- server-client communication
- database

(1 pages)
