#!/bin/bash

# Run from root directory
test -e ../lib && cd ..

# Exit immediately if a command exits with a non-zero status
set -e

cd lib
rm -rf master.tar.gz*
rm -rf modular_arithmetic*
wget https://github.com/hurchalla/modular_arithmetic/archive/master.tar.gz
tar xvf master.tar.gz
rm master.tar.gz*
mv modular_arithmetic-master modular_arithmetic

git status
echo ""

read -p "Do you want to commit and push the changes? [y/n] " -n 1 -r
echo    # (optional) move to a new line
if [[ $REPLY =~ ^[Yy]$ ]]
then
    git add .
    git commit -m "Update to latest libmodular_arithmetic"
    git push
fi
