#!/bin/bash

compton &
feh --bg-scale ~/.config/wall.jpg

avg_load() {
    #AVG_LOAD=`uptime | awk -F'load averages:' '{ print $2 }'`
    AVG_LOAD=`uptime | awk -F'[a-z]:' '{ print $2 }'`
    echo "L:$AVG_LOAD"
}

dte() {
    echo $(date)
}

battery() {

    CAPACITY=`acpiconf -i 0 | grep 'Remaining capacity:' | cut -f2`
    TIME=`acpiconf -i 0 | grep 'Remaining time:' | cut -f3`
    STATE=`acpiconf -i 0 | grep 'State:' | cut -f4`

    if [ $STATE == "discharging" ]; then
        STATE="-"
    elif [ $STATE == "charging" ]; then
        STATE="+"
    fi

    if [ $STATE == "-" ]; then
        echo "B: $CAPACITY [$STATE] ($TIME)"
    elif [ $STATE == "+" ]; then
        echo "B: $CAPACITY [$STATE]" 
    fi
}

brand() {
    BRAND=$'\uf3a4'
    echo $BRAND
}

while true; do
    xsetroot -name  "$(avg_load) | $(battery) | $(dte) | $(brand)"
    sleep 1
done &
