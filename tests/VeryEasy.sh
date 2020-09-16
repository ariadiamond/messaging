#!/bin/bash

# Colors are essential
RED="\x1b[91m"
GREEN="\x1b[92m"
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
valgrind ${SERVERDIR}${SERVER} -r > out 2>&1 &
sleep 1

echo -e "The idea that climate change is going to eventually eradicate humanity and yet people are more inclined to try to continue to exploit the earth because it improves their lives materialistically is kind of sadening.\nI know it\'s amazing that people have better lives, but also their lives are simultaneously being destroyed by the rich." > MsgFile


# Do the things
#echo -e "\x1b[33mSend Message\x1b[0m"
${CLIENT} aria x axoo MsgFile
#echo -e "\x1b[33mCheck for message\x1b[0m"
${CLIENT} axoo a > clientFile1
#echo -e "\x1b[33mCheck again for message\x1b[0m"
${CLIENT} axoo a > clientFile2
#echo -e "\x1b[33mAria checks for message\x1b[0m"
${CLIENT} aria x > clientFile3

# Did the things
kill "$(lsof -i :8080 | grep "[0-9]" | awk '{ print $2}')"

##########
# Tests! #
##########

echo -e "\n\x1b[100;1mTests!\x1b[0m"

# Test 1 (did it go to server)
echo -n "Message transfered to the server "
grep -f MsgFile axoo > /dev/null
if [ $? -eq 0 ]; then
	echo -e "[${GREEN}Passed :)${UNSET}]"
else
	echo -e "[${RED}Failed :(${UNSET}]"
	cat axoo
fi

# Test 2 (did it get to the client)
echo -n "Message received by client       "
grep -f MsgFile clientFile1 > /dev/null
if [ $? -eq 0 ]; then
	echo -e "[${GREEN}Passed :)${UNSET}]"
else
	echo -e "[${RED}Failed :(${UNSET}]"
	cat clientFile
fi

# Test 3 (what if we get it again?)
echo -n "Message received by client again "
grep -f MsgFile clientFile2 > /dev/null
if [ $? -eq 0 ]; then
	echo -e "[${GREEN}Passed :)${UNSET}]"
else
	echo -e "[${RED}Failed :(${UNSET}]"
	cat clientFile2
fi

# Test 4 (what if aria tries to get their non-existent messages)
echo -n "A client checks with no messages "
grep "You don't have any new messages" clientFile3 > /dev/null
if [ $? -eq 0 ]; then
	echo -e "[${GREEN}Passed :)${UNSET}]"
else
	echo -e "[${RED}Failed :(${UNSET}]"
	cat clientFile3
fi

###############################
# Server Output with valgrind #
###############################
echo ""
echo -e "\n\x1b[100;1mServer Output:\x1b[0m"
cat out

# Cleanup
rm axoo clientFile? MsgFile out
