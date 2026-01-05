#!/bin/bash

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

STATUS=$1
MESSAGE=$2

if [ "$STATUS" == "OK" ]; then
    printf "${GREEN}[ OK ]${NC} %s\n" "$MESSAGE"
elif [ "$STATUS" == "FAIL" ]; then
    printf "${RED}[ FAIL ]${NC} %s\n" "$MESSAGE"
else
    printf "%s\n" "$MESSAGE"
fi
