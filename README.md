# Messaging

*This project is partially a result of worrying about not being good enough for grad school applications.*

This is a messaging client and server pair and (will) pass encrypted messages. Please don't actually use this for sensitive things (use something like Signal).

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
## TODO

1. encryption (lol)
2. Registering names so some message integrity