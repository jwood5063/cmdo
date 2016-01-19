# cmdo top level makefile

default: blinker hello arduino


blinker:
	make -C proj/blinker

hello:
	make -C proj/hello

arduino:
	make -C proj/arduino

clean:
	make -C  proj/blinker clean
	make -C  proj/hello clean
	make -C  proj/arduino clean

	
