#!/bin/bash

CLIENT=PseudoClient
SERVERDIR=../server/
SERVER=CryptMsgServer


# Build server
cd ${SERVERDIR}
make

# Start server with time to bind to the port before clients do
cd ../tests/
${SERVERDIR}${SERVER} 8080&
sleep 1

# Build Client
make pseudo

# Send messages
echo "Make sure you are hydrated" > msgFile
./${CLIENT} aria rmnd msgFile
echo "Blankets are super good!" > msgFile
./${CLIENT} hmmm soft msgFile
echo "It's okay to be emotionally vulnerable" > msgFile
./${CLIENT} hmmm rmnd msgFile
echo "You can be hydrated and happy with Hot Chocolate :)" > msgFile
./${CLIENT} rmnd soft msgFile
echo "Stuffed animals as cuddle buddies >>>>\nhehe" > msgFile
./${CLIENT} soft soft msgFile

# Get Messages
./${CLIENT} rmnd
./${CLIENT} soft


# Cleanup
pkill ${SERVER}
rm msgFile soft rmnd