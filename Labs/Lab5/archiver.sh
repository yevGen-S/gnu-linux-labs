#!/bin/bash
FOLDER_PATH=$1
ARCHIVE_NAME=$2
PERIOD=$3
while true
do
  TIME=$(date +%d_%m_%H_%M_%S)
  tar -cvf "$ARCHIVE_NAME_$TIME.tar" $FOLDER_PATH
  echo $FOLDER_PATH folder archived to $ARCHIVE_NAME.tar
  sleep $PERIOD
done
