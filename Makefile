.PHONY: all
all:
	mkdir -p bin/
	gcc src/* -I include/ -o bin/dump
