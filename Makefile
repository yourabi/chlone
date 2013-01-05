all: build 

build:
	cc -Wall -Wextra -Wstrict-prototypes chlone.c -o chlone

install: build
	cp chlone /usr/local/bin

clean: 
	rm chlone



