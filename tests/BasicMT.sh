#!/bin/bash

# This is not testing race conditions, rather just if the server can deal with
# multiple clients at the same time.

CLIENT=PseudoClient
SERVERDIR=../server/
SERVER=CryptMsgServer


# Build the server
cd ${SERVERDIR}
make

# Build PseudoClient
cd ../tests/
make pseudo

# Start server
${SERVERDIR}${SERVER} -l &
sleep 1 #So the server can start before getting requests


# Clients
# Test 1, same message to multiple people from the same person
echo "Staying hydrated with tea keeps you warm and happy!" > MsgFile1
./${CLIENT} aria soft MsgFile1 &
./${CLIENT} aria rmnd MsgFile1

# Test 2, two messages from the same person to multiple people
echo "Flowers and floral teas :)" > MsgFile1
echo "If you are looking at a screen a lot, look at something 20 feet away for 20 seconds every 20 minutes." > MsgFile2
./${CLIENT} aria soft MsgFile1 &
./${CLIENT} aria rmnd MsgFile2

# Request things
./${CLIENT} soft &
./${CLIENT} rmnd

echo -e "\n"

# Cleanup
pkill ${SERVER}
rm MsgFile?