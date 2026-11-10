# BeanstalkBlitz

A competitive multiplayer jump-racing game in C++, built on DigiPen's AlphaEngine.

Players climb a vertical course to the finish line by chaining charged jumps. Movement
runs through an explicit state machine (idle, grounded, jump charge, airborne, flying,
falling, stun, victory), the course is assembled from container-driven maps, and enemies
plus a boss stand between you and the finish.

## Networking

`Project/Source/NetworkingTest.cpp/.hpp` is the multiplayer layer, and the first socket
code I ever wrote. It contains:

- a Winsock client/server (blocking sockets, a thread per peer)
- an RPC bus that turns network command IDs into function calls
- lobby object management

It is a prototype for the authoritative-server design rather than the finished match
loop: the game still plays its race locally in this snapshot.

## Layout

```
Project/BeanstalkBlitz.sln        Visual Studio solution
Project/Source                    game code (players, states, enemies, maps, UI, audio)
Project/Extern/AlphaEngine        DigiPen's 2D engine
Project/Extern/rapidjson          JSON parsing
```

## Build

Windows only. Open `Project/BeanstalkBlitz.sln` in Visual Studio and build; headers and
libs for AlphaEngine and rapidjson are expected under `Project/Extern/`.

## Status

Coursework snapshot, not maintained. Kept as a record of the state-machine and socket
work, not as a playable release.
