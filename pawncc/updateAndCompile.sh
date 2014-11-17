#!/bin/bash
source compiler.cfg
FORCE="FALSE"

if test "$1" = "-f"
then
	FORCE="TRUE"
fi

cd $PROJDIR
resp=$(git pull)

if test "$resp" = "Already up-to-date." -a "$FORCE" = "FALSE"
then
	echo "* Already up-to-date" > /dev/null
else
	echo $resp >> $CCHOME/git.log
	echo "* Se detectaron cambios en el repo *"
	echo "Compilando..."
	cd $CCHOME
	./compile.sh > lastCompilation.log
fi
