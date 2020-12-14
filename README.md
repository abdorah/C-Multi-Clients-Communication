# C-Multi-Clients-Communication

*Example of communication between multiple clients provided by one server in C.*

This project represents a simple example of how to use the `select` method in C and in linux to performe a communication between multiple clients instances provided by one server.
***
### How this code works

First thing first, I used the port `8084` you can change this port and use any free port on your local machine
in the `Server.c`:

``` c
srvAddr.sin_port = htons(8084);
```

and in the `Client.c`:

``` c
adrsrv.sin_port = htons(8084);
```

also you can change the *ip address* of the server:

``` c
inet_aton("127.0.0.1", &(adrsrv.sin_addr));
```

The server assert that the username provided is unique, and then register it. It can register up to 20 client. This is something provided in this implementation you can change the number of users to add. You can change this in the `Server.c`:

``` c
char *pseudonames[20];
```

To use this code on your linux machine you need just to run the `Server.c` first and then run as much as you need of instaces of the `Client.c`. Then each client can decide his or her name and communicate with the others.

You can find in the **Assets** directorie some pictures that will show you an example of execution of the code.
