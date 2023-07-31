#!/bin/bash
echo "Setting libraries"
SCRIPT_DIR="$(dirname "$(readlink -f "$0")")"
export LD_LIBRARY_PATH="$SCRIPT_DIR/lib":$LD_LIBRARY_PATH
./daemon -p agcfuzi9mnras1gs -d 262e7e1c9162ca6b27kvcl -s AkCmJ4AXgdgHdi8Y
