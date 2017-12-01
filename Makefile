
ARG = -Wall -Werror -std=c99 -pedantic
SRC = unicode.c utf8.c utf16.c utf32.c
OBJ = unicode.o utf8.o utf16.o utf32.o

all: a.out

a.out: libunicode.a unicode_example.c
	gcc unicode_example.c $(ARG) -L. -lunicode

libunicode.a: $(SRC) unicode.h unicode_internal.h
	gcc $(SRC) -c $(ARG)
	ar rcs libunicode.a $(OBJ)

clean:
	rm -rf a.out *.o *.a *.so *.h.gch 
