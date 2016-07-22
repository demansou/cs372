#!/bin/bash
echo 'Compiling chatserve.py...'
echo 'Which port to use for server?'
read
echo 'Starting chatserve...'
python chatserve.py $REPLY &
