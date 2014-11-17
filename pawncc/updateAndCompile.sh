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
	cd gamemodes
	$PAWNCC -i$INCLUDEDIR -i../include $BUILDNAME.pwn &> $BUILDNAME.log
	if diff -q isamp-core.amx $TESTSERVER/$BUILDNAME.amx
	then
		echo "El resultado es identico al anterior"
	else
		echo "El nuevo gm difiere, actualizando..."
		cp $BUILDNAME.amx ../../../testserver/gamemodes/$BUILDNAME.amx
	fi
fi
