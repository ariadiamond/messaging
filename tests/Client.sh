#!/bin/bash

CLIENT=PseudoClient
SERVERDIR=../server/
SERVER=CryptMsgServer


# Build server
cd ${SERVERDIR}
make

# Build Client
cd ../tests/
make pseudo

echo "" #newline

# Start server with time to set up before accepting clients
${SERVERDIR}${SERVER} -p 8080 -l &
sleep 1


# Send messages
echo "Make sure you are hydrated" > msgFile
./${CLIENT} aria rmnd msgFile
echo "Blankets are super good!" > msgFile
./${CLIENT} hmmm soft msgFile
echo "It's okay to be emotionally vulnerable" > msgFile
./${CLIENT} hmmm rmnd msgFile
echo "You can be hydrated and happy with Hot Chocolate :)" > msgFile
./${CLIENT} rmnd soft msgFile
echo "Stuffed animals as cuddle buddies >>>>
hehe" > msgFile
./${CLIENT} soft soft msgFile

# Get Messages
./${CLIENT} rmnd
./${CLIENT} soft

echo "It's okay not to be perfect, although if you don't try, you will never find out how you do." > msgFile
./${CLIENT} aria rmnd msgFile #It doesn't always work FIXME: Client error
./${CLIENT} rmnd

echo "" #newline

# Cleanup
pkill ${SERVER}
rm msgFile
#rm soft rmnd # easy to comment out (Don't forget -r!)