# Messaging

*This project is partially a result of worrying about not being good enough for grad school applications.*

Did somone say Ceaser cypher?!?! Let's get this old but ~~gold~~ security mechanism implemented! 

--------------------------------------------------------------------------------
## Building

The server can be built by going into the server directory and running make

```bash
cd server/
make
```

The client can be done the same way (but into the client directory)

```bash
cd client/
make config
make
```

--------------------------------------------------------------------------------
## Running

To start the server:

```bash
./server/CryptMsgServer [-p port] [-l] [-r]
```
|Argument|Meaning|
|:------:|-------|
|-p port |`port` represents what port you want to star the server on. The default is 8080|
|-l      |enables logging in file `error.log`|
|-r      |disables auto removing messages after they are requested. This is a debugging option and requesting messages will return messages already received|

To run the client:
```bash
./client/Client
```

--------------------------------------------------------------------------------
## Vulnerabilities

This uses a Ceaser cypher to encrypt messages, which is bad. If you want to brute force it, the algorithm is fast to run, and there's a very small amount of inputs to try (128 for ASCII, however many options are unavailable). This is because adding or subtracting characters with too big numbers would put the character out of ASCII range (ie. if you have an 'a' (97), you know that it cannot spin more than positive 20 or negative 97 (but less because special characters).

There's also frequency analysis, which is a little less certain with shorter messages, but since the spin amount is consistent, passing a few messages will get a large enough reportiore quickly.
