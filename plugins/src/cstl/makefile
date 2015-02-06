GPP=g++
GCC=gcc
OUTFILE="./cstl.so"

COMPILE_FLAGS=-m32 -c -O3 -w -DLINUX -fexceptions -I ./AMX_SDK/amx/

all:
	$(GCC) $(COMPILE_FLAGS) ./AMX_SDK/amx/*.c
	$(GPP) $(COMPILE_FLAGS) ./AMX_SDK/*.cpp
	$(GPP) $(COMPILE_FLAGS) ./*.cpp
	$(GPP) -m32 -O2 -fshort-wchar -shared -o $(OUTFILE) *.o -lpthread
	rm -f *.o

clean:
	rm -f *.o
	rm -f ./AMX_SDK/amx/*.o
	rm -f ./AMX_SDK/*.o 
