#!/bin/bash

TAR_SRC=./srcUpdate/
rm -fr $TAR_SRC
mkdir $TAR_SRC
cp -fa ./update/ $TAR_SRC
cp -fa ./Resource/ $TAR_SRC
cp -f ./doTar.sh $TAR_SRC
cp -fa ./Target/ $TAR_SRC

rm -fr $TAR_SRC/update/.git
tar czvf Update.tar.gz srcUpdate
#rm -fr $TAR_SRC
