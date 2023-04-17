# OptimalPathFinder
Output of this application is a data structure representing the coordinates of a path from point A to point B.  

This application is the Server for the Client [OptimalPathFinderClient](https://github.com/PaThreek/OptimalPathFinderClient) application. Finds the optimal path from point A to point B based on the request from the Client.

The application tries to create the socket connection and then waits for the Client [OptimalPathFinderClient](https://github.com/PaThreek/OptimalPathFinderClient) application to connect. After the connection is estabilished, the application is waiting for the coordinates from the Client to be received via the socket connection.

After the coordinates are received and valid, the application will perfrom breadth-first algorithm on the file "map.npy" to find the optimal path between the coordinates given by the Client. The path or error is returned to the Client via the socket connection.

Developed, compiled and tested under Ubuntu 22.04 LTS. Compiler gcc 11.3.0.

Ideas to improve:
* the socket connection could be improved with more complex error handling, timeouts, reconnections, etc.
* the data transmission over the socket connection could be improved with extended error handling
* reading and writing of the data received/sent from/to the Client is currently hard-coded to one purpose, that is the path request. There can be defined a more generic way how to deal with the data received/sent from the Client
* implementation of unit tests

Libraries used:

* loading the file "map.npy": [libnpy](https://github.com/llohse/libnpy)
