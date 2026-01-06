#!/bin/bash

# Configuration
CONFIG_FILE="config.txt"
BACKUP_CONFIG="config.txt.bak"
CLIENT="./ub-client"

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

# Backup config
cp "$CONFIG_FILE" "$BACKUP_CONFIG"

# Cleanup trap
cleanup() {
    mv "$BACKUP_CONFIG" "$CONFIG_FILE"
}
trap cleanup EXIT

print_status() {
    local status=$1
    local protocol=$2
    local endpoint=$3

    if [ "$status" == "SUCCESS" ]; then
        printf "\033[0;36m[ RECEIVED RESPONSE ]\033[0m %-7s %s\n" "[$protocol]" "$endpoint"
    else
        printf "${RED}[     NO REPLY      ]${NC} %-7s %s\n" "[$protocol]" "$endpoint"
    fi
}

run_test() {
    local protocol=$1
    local args=$2
    local endpoint_display=$3

    # Run client and capture output
    output=$($CLIENT $args 2>&1)

    # Check for success marker
    if echo "$output" | grep -q "\-\-\- Result"; then
        print_status "SUCCESS" "$protocol" "$endpoint_display"
    else
        print_status "FAIL" "$protocol" "$endpoint_display"
        # Optional: print failure details for debugging script
        # echo "$output"
    fi
}

run_suite() {
    local protocol=$1
    local use_https=$2

    # Update config
    if [ "$use_https" == "true" ]; then
        sed -i 's/^use_https=.*/use_https=true/' "$CONFIG_FILE"
    else
        sed -i 's/^use_https=.*/use_https=false/' "$CONFIG_FILE"
    fi

    # Run Tests
    run_test "$protocol" "--cities" "/GetCities"
    run_test "$protocol" "--location Örebro" "/GetLocation?name=Örebro"
    run_test "$protocol" "--weather 59.32 18.06" "/GetWeather?lat=59.32&lon=18.06"
    run_test "$protocol" "--surprise" "/GetSurprise"
}

# Run HTTP Suite
run_suite "HTTP" "false"

# Run HTTPS Suite
run_suite "HTTPS" "true"
