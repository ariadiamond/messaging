#!/bin/bash

CLIENT=PseudoClient
SERVERDIR=../server/
SERVER=CryptMsgServer


#############
# Prep work #
#############

# Build server
cd ${SERVERDIR}
make > /dev/null

# Build Client
cd ../tests/
make pseudo > /dev/null

echo ""


#################
# Do the things #
#################

# Start server with time to set up before accepting clients
${SERVERDIR}${SERVER} -p 8080 &
sleep 1


# Send messages
echo "Make sure you are hydrated" > msgFile
./${CLIENT} aria x rmnd msgFile
echo "Blankets are super good!" > msgFile
./${CLIENT} hmmm r soft msgFile
echo "It's okay to be emotionally vulnerable" > msgFile
./${CLIENT} hmmm r rmnd msgFile
echo "You can be hydrated and happy with Hot Chocolate :)" > msgFile
./${CLIENT} rmnd Q soft msgFile
echo -e "Stuffed animals as cuddle buddies >>>>\n(hehe)" > msgFile
./${CLIENT} soft R soft msgFile

# Get Messages
./${CLIENT} rmnd Q
./${CLIENT} soft R

echo "It's okay not to be perfect, although if you don't try, you will never be perfect." > msgFile
./${CLIENT} aria x rmnd msgFile
echo -e "You.\nAre.\nImportant.\n" > msgFile
./${CLIENT} axoo a rmnd msgFile
./${CLIENT} rmnd Q

echo ""


###########
# Cleanup #
###########

pkill ${SERVER}
rm msgFile
#rm soft rmnd # easy to comment out (Don't forget -r!)