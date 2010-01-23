all:
	gcc -o bmtimer bmtimer.c -ldockapp

install:
	cp bmtimer /usr/bin/bmtimer

clean:
	rm bmtimer

uninstall: 
	rm /usr/bin/bmtimer
