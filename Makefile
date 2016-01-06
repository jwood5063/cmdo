# cmdo top level makefile

default: blinker hello


blinker:
	make -C proj/blinker

hello:
	make -C proj/hello

clean:
	make -C  proj/blinker clean
	make -C  proj/hello clean

	
