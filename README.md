# Messaging

*This project is partially a result of worrying about not being good enough for grad school applications.*

This passes messages encrypted with a byte long key made info stream cipher by using a PRNG to randomize the key. I would still recommend using something like Signal over this as there are glaring flaws [(such as how the seed is passed)](#Vulnerabilities).

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
make
```

--------------------------------------------------------------------------------
## Running

To start the server:

```bash
./server/CryptMsgServer [-p port] [-l] [-r] [-s seed]
```
|Argument|Meaning|
|:------:|-------|
|-p port |`port` represents what port you want to star the server on. The default is 8080|
|-l      |enables logging in file `error.log`|
|-r      |disables auto removing messages after they are requested. This is a debugging option and requesting messages will return messages already received|
|-s seed |`seed` represents the seed to start at for the PRNG that is used to encrypt messages|

To run the client:
```bash
./client/Client name key
```
where name is you and key is your key


--------------------------------------------------------------------------------
## Vulnerabilities

* With a man in the middle attack, an attacker could find the key of a user just with name verification. Since seeds and names are passed in the clear, xoring the seed and encrypted string ("Hello friendo!") against the cipher text reveals the key

--------------------------------------------------------------------------------
## TODO

1. Key passing
2. encrypting server files