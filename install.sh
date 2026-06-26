#!/bin/bash

if [ "$#" -ne 1 ]
then
    echo "Usage: $0 <path to root of kdb-x installation>"
    echo "(by default kdb-x is installed to ~/.kx)"
    exit 1
fi

QBASE_MOD="$1/mod"

# check destination directory exists
if [ ! -w "$QBASE_MOD" ]; then
    echo "ERROR: Installation destination '$QBASE_MOD' does not exist"
    exit 1
fi

# check source directory exists
if [ ! -d mod ]; then
    echo "ERROR: Directory 'mod' does not exist. Please run the install script located inside the release package."
    exit 1
fi

# install module
echo "Copying module to $QBASE_MOD ..."
cp -r mod/* $QBASE_MOD
if [ $? -ne 0 ]; then
    echo "ERROR: Copy failed"
    exit 1
fi

echo "Installation complete"
exit 0
