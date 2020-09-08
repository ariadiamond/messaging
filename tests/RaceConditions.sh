#!/bin/bash

# Colors are essential
RED="\x1b[31m"
GREEN="\x1b[32m"
UNSET="\x1b[0m"

CLIENT=./PseudoClient
SERVERDIR=../server/
SERVER=CryptMsgServer

# Build server
cd ${SERVERDIR}
make > /dev/null

# Build psuedoclient
cd ../tests/
make pseudo > /dev/null

# Start the server
${SERVERDIR}${SERVER} -p 8080 -r &

# Set up listeners to see if seed is the same for multiple
socat -v TCP-LISTEN:8081 TCP:localhost:8080 2> out1 &
socat -v TCP-LISTEN:8082 TCP:localhost:8080 2> out2 &
socat -v TCP-LISTEN:8083 TCP:localhost:8080 2> out3 &
socat -v TCP-LISTEN:8084 TCP:localhost:8080 2> out4 &

sleep 1

# Create messages to send
echo "111" > Msg1
echo "222" > Msg2
echo "333" > Msg3
echo "444" > Msg4

# Run the clients concurrently
${CLIENT} aria x hmmm Msg1 8081 &
${CLIENT} rmnd Q hmmm Msg2 8082 &
${CLIENT} axoo a hmmm Msg3 8083 &
${CLIENT} aria x hmmm Msg4 8084

sleep 1

${CLIENT} hmmm r

# Do the clients write over each other?
echo "Messages recieved by server:"
for I in Msg?; do
	grep -f $I hmmm > /dev/null
	if [ $? -eq 0 ]; then
		echo -e "$I: [${GREEN}passed${UNSET}]"
	else
		echo -e "$I: [${RED}failed${UNSET}]"
	fi
done

# Do clients get the same starting seed?
for I in out?; do
	sed 2q $I >> outA
done

echo "Are the seeds repeated?"
grep -v "^<" outA | sort -n | awk '{ print $1 }'


# Cleanup - socat automatically terminates with server
pkill ${SERVER}
rm out? Msg? hmmm