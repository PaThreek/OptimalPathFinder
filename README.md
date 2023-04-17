# OptimalPathFinder
Output of this application is a generated image of a path from point A to point B.  

The Server for the Client [OptimalPathFinderClient](https://github.com/PaThreek/OptimalPathFinderClient) application. Finds the optimal path from point A to point B based on the request from the Client.

Developed, compiled and tested under Ubuntu 22.04 LTS. Compiler gcc 11.3.0.

Ideas to improve:
* the socket connection could be improved with more complex error handling, timeouts, reconnections, etc.
* the data transmission over the socket connection could be improved with extended error handling
* reading and writing of the data received/sent from/to the Client is currently hard-coded to one purpose, that is the path request. There can be defined a more generic way how to deal with the data received/sent from the Client
* implementation of unit tests
