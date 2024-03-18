### Design justification: locking client frame rate

Locking the frame rate has two main benefits: simplifing physics calculations and reducing the load on the server. Physics calculations now have a constant frame time for updating positions. Since the client updates the server with its position after rendering each frame, limiting the framerate reduces the bandwidth of the packets sent by the client, reducing the UDP packet loss ratio. The game uses a frame rate of 20 because this is enough to have the motion of the balls be visibly smooth, while having an extremely low UDP packet loss ratio.


### Design justification: server sending updates to clients as soon as it has them

Most online multiplayer games have a fixed rate at which they update the server-side gamestate and send updates to the client, which helps to ensure fairness between clients and prevent the server being overloaded. Roll Rumble processes client packets as soon as they are received, then immediately updates the other client. This is more efficient for the game because it reduces latency, and since there are only two clients overloading the server is not a worry.


### Performance metrics

- The average total input latency, between moving the controller, and the ball moving on the screen is 138ms.
- The client maintains a consistent 20 FPS (could likely be pushed much higher)
- The average round-trip time over UDP between the client and server is 6.6ms
- The average UDP packet loss ratio is just 0.02% for communications between the client and server


### Testing

- To test communication between the NIOS II and the client PC, 100 messages were sent from the NIOS II to the host PC and checked to ensure correctness.
- To test communication from the client to the server, 100 messages were sent from the client to the server, and checked to ensure correctness. The same was then done with the server returning the values to the client to test communication from the server to the client.

### INCLUDE TESTING DIAGRAM
