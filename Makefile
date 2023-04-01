all: boulderdash

clean:
	-rm -f *~ *.o

purge: clean
	-rm -f boulderdash

boulderdash: boulderdash.o tiles.o audio.o render.o
	gcc boulderdash.o tiles.o audio.o render.o -o boulderdash -Wall -lallegro -lallegro_image -lallegro_audio -lallegro_acodec -lallegro_font -lallegro_ttf -lm

boulderdash.o: boulderdash.c boulderdash.h
	gcc -c boulderdash.c -o boulderdash.o -Wall

tiles.o: tiles.c tiles.h boulderdash.h audio.h
	gcc -c tiles.c -o tiles.o -Wall

audio.o: audio.c audio.h boulderdash.h
	gcc -c audio.c -o audio.o -Wall

render.o: render.c render.h boulderdash.h tiles.h
	gcc -c render.c -o render.o -Wall