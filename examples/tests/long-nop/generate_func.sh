#!/bin/bash

cat $5 > $2
echo ".section .text" >> $2
echo "eapp_entry:" >> $2
echo "add:" >> $2
yes "$(cat $3)" | head -n $1 >> $2
cat $4 >> $2
