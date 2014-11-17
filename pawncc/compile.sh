#!/bin/bash
source compiler.cfg

cd $PROJDIR/gamemodes
$PAWNCC -i$INCLUDEDIR -i../include $BUILDNAME.pwn #&> $BUILDNAME.log

if diff -q isamp-core.amx $TESTSERVER/$BUILDNAME.amx
then
	echo "El resultado es identico al anterior"
else
	echo "El nuevo gm difiere, actualizando..."
	cp $BUILDNAME.amx ../../../testserver/gamemodes/$BUILDNAME.amx
fi
