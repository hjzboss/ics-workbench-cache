NAME := $(shell basename $(PWD))
BZ2 := microbench-test.log.bz2
build: $(NAME)-64

run:
	./cachesim-64 ${BZ2}
	./statistic.py

include ../Makefile
