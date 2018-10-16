#!/bin/bash

echo ".section .text" > $2
echo "eapp_entry:" >> $2
echo "add:" >> $2
yes "$(cat nop.s)" | head -n $1 >> $2
cat func_base.s >> $2
