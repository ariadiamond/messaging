#!/bin/bash

CLIENTDIR=../client/
CLIENT=Client
SERVERDIR=../server/
SERVER=CryptMessageServer


# Build client and server
cd ${CLIENTDIR}
make
cd ${SERVERDIR}
make

# Start server with time to bind to the port before clients do
cd ../tests/
${SERVERDIR}${SERVER} 8080&
sleep 1

# Run clients
${CLIENTDIR}${CLIENT} aria
${CLIENTDIR}${CLIENT} aria


# Cleanup
pkill ${SERVER}
