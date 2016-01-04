# cmdo top level makefile

default: blinker


blinker:
	make -C proj/blinker


clean:
	make -C  proj/blinker clean

	
