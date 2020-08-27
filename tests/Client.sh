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

echo -e "aria: x\nhmmm: *\naxoo: \`\nsoft:  \nrmnd: 5\n" > .keyfile

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
echo -e "Stuffed animals as cuddle buddies >>>>\n(hehe)" > msgFile
./${CLIENT} soft soft msgFile

# Get Messages
./${CLIENT} rmnd
./${CLIENT} soft

echo "It's okay not to be perfect, although if you don't try, you will never be perfect." > msgFile
./${CLIENT} aria rmnd msgFile
echo "You.\nAre.\nImportant.\n" > msgFile # checking if escape sequences work
./${CLIENT} axoo rmnd msgFile
./${CLIENT} rmnd

echo "" #newline

# Cleanup
pkill ${SERVER}
rm msgFile
#rm soft rmnd # easy to comment out (Don't forget -r!)