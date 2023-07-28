#!/bin/bash
echo "Setting libraries"
export LD_LIBRARY_PATH=/home/studentas/Desktop/Task\ Number\ 5/lib/:$LD_LIBRARY_PATH
./main deviceinfo.json
