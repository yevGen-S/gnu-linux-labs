#!/bin/bash

FIRST="FIRST STRING"
SECOND="SECOND STRING"

echo $FIRST
echo $SECOND

CONCAT="$FIRST and $SECOND"

SUBST=${CONCAT:5:3}

REPST=${CONCAT/$FIRST/$SECOND}

echo $CONCAT
echo $SUBST
echo $REPST
