#!/bin/bash

# Colors are essential
RED="\x1b[31m"
GREEN="\x1b[32m"
BLUE="\x1b[34m"
UNSET="\x1b[0m"

CLIENT=./PseudoClient
SERVERDIR=../server/
SERVER=CryptMsgServer


#############
# Prep work #
#############

# Build server
cd ${SERVERDIR}
make > /dev/null

# Build psuedoclient
cd ../tests/
make pseudo > /dev/null


# Create messages to send
echo -e "I have a feeling that the people who say \"all lives matter\" are the same people opposed to affirmative action.\nBecause de jure segregation does not exist*, there are no official boundaries that keep minorities from prosperity.\nWhat is not mentioned is the legacy of socially codified inequality that means that minorities have access to less capital and attitudes towards them are still (sometimes veiled) reincarnations of the inequality of the past.\n\n* There aren't any laws explicitly mentioning limiting Black people (to my knowledge), but there are laws that overwhelmingly affect a certain population. These laws help maintain social hierarchies and prevent Black people equal access." > Msg1
echo -e "Despite gender's remarkably arbitrariness, there are very strict expectations about how it works and how to act to follow the rules set by it.\nWhile there are traits typically associated with either men or women, which they are associated with is completely arbitrary.\nAnd no one is completely perfect at only embodying thier gender's traits.\nIf trying to use sex to distinguish, depsite popular belief, it is also very arbitrary.\nThere are intersex people for one, who's existence invalidates the idea of binary sexes.\nBut more than that, many people who are assigned to one of the codified sex categories often do ont have every single feature associated with the sex category." > Msg2
echo -e "One of the largest issues I see with computer science is how it, like all other sciences, is seen as objective and inherently apolitical.\nI'm hoping people are starting to see how it isn't especially as computers dominate so many parts of our lives.\n" > Msg3
echo -e "With the fires in california, there have been mentions of climate change and returns to Indigenous land practices.\nWhile important and would have averted the current crisis, I'm skeptical if it would be enough now.\nLooking at the lack of action and hostility to action, I doubt people are willing to give up their modern conveniences (even maybe me) in order to protect the majority of the world from extinction.\nI don't have much hope for humanity, but I don't think people are going to agree to go back to such practices and much more radical action is necessary." > Msg4


#################
# Do the things #
#################

# Start the server
echo -e "\n\x1b[100mServer Output:\x1b[0m"
${SERVERDIR}${SERVER} -p 8080 -r &

sleep 1

# Run the clients concurrently
${CLIENT} aria x hmmm Msg1 &
${CLIENT} rmnd Q hmmm Msg2 &
${CLIENT} axoo a hmmm Msg3 &
${CLIENT} aria x hmmm Msg4

sleep 1

${CLIENT} hmmm r > outC


##########
# Tests! #
##########

echo -e "\n\x1b[100mTests:\x1b[0m"

# Do the clients write over each other?
FAILED=false
echo "Messages recieved by server:"
for I in Msg?; do
	grep -f $I hmmm > /dev/null
	if [ $? -eq 0 ]; then
		grep -f $I outC > /dev/null
		if [ $? -eq 0 ]; then
			echo -e "$I: [${GREEN}passed${UNSET}]"
		else
			echo -e "$I was put in as one message (likely not next to it's header): [${BLUE}partial credit ${UNSET}]"
			FAILED=true
		fi
	else
		echo -e "$I got split up: [${RED}failed${UNSET}]"
		FAILED=true
	fi
done

if [ $FAILED == "true" ]; then
	cat outC
fi


###########
# Cleanup #
###########

echo ""
pkill ${SERVER}
rm out? Msg? hmmm