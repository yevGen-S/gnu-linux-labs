#!/bin/bash
while true
do
  echo "Enter text: "
  read text
  echo "You entered: $text"
  sleep 5
  echo "Enter one more text: "
  read another_text
  echo "You entered once more: $another_text"
  sleep 30
done
