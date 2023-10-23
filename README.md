# department-analysis-system
Implementation of department analysis system using TCP socket programming

USC-EE450-Project_part1-sp23

1)  OS-Version: Ubuntu 22.04

2)  In this project, we are trying out socket programming in which we have one server and two clients and the same server will service 
requests of both the clients. The client is going to send the department name to the server, which in turn will search for that department 
name in its database or data stored and the send the corresponding data as response to the client that requested it.

3)  Code Files:
    a)  servermain.cpp
        -   used to create a server that runs and is ready for connection to a client.
        -   once the connection is set up with the client, it accepts data from that client i.e., department name.
        -   searches for the received departname name in its data and then sends the corresponding data i.e., server id which contains the 
            department, to its client by sending the status of found or not found in form of a message
        

    b)  client.cpp
        -   makes the socket connection with the servermain.cpp i.e., the server
        -   accepts department name from the user and sends a request to the server
        -   it then receives the response from server accordingly if the department name is found or not
        -   printing if the codeword can be pass or not pass

4)  The project fails when the memory is not enough for the data and functions to perform.

5)  Reused Code: There a few code snippets which have been used directly from the Beej's guide to socket programming and 
have been marked with the comments in the code

The code snippets that are used from Beej's guide to socket programming are as follows: