#!/bin/sh

function gitrename() {
  cd $1
  echo "cd $1"
  for file in *; do
    if [ -f $file ]; then
      newfile=`echo $file | tr "KX" "Kx"`
      if [ "$file" != "$newfile" ]; then
        echo "git mv $file $newfile"
        mv $file $newfile
      fi
    elif [ -d $file ] && [ $file != "." ] && [ $file != ".." ]; then
      # echo "gitrename $file"
      gitrename $file
    fi
  done
  cd ..
}

gitrename "$1"
