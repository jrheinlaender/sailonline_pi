#! /bin/bash

# This script refactors this plugin into a new plugin with a name of your choice.
# To rename the plugin to "catsarebest", run `bash make-new-plugin.sh catsarebest`.

newname="$1"
Newname="$(echo "${newname^}")"
NEWNAME="$(echo "${newname^^}")"

grep -rl sailonline . | grep -v .git | while read name; do
  sed -e "s+sailonline+$newname+g" -i "$name";
done

grep -rl Sailonline . | grep -v .git | while read name; do  
  sed -e "s+Sailonline+$Newname+g" -i "$name";
done 

grep -rl SAILONLINE . | grep -v .git | while read name; do  
  sed -e "s+SAILONLINE+$NEWNAME+g" -i "$name";
done 

find . -name "*sailonline*" | grep -v .git | while read name; do
  mv "$name" "$(echo "$name" | sed -e "s+sailonline+$newname+g")"
done

find . -name "*Sailonline*" | grep -v .git | while read name; do
  mv "$name" "$(echo "$name" | sed -e "s+Sailonline+$Newname+g")"
done
