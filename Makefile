# To compile all programs, run:
#   make
#
# AED, ua, 2023

CC = gcc
CFLAGS += -g -Wall -Wextra
CPPFLAGS += -MMD

TARGETS = example1 example2 example3 RandomTester LoaderTester DotExporter

all: $(TARGETS)

RandomTester: RandomTester.o Graph.o SortedList.o

LoaderTester: LoaderTester.o Graph.o SortedList.o

DotExporter: DotExporter.o Graph.o SortedList.o

example1: example1.o Graph.o SortedList.o

example2: example2.o Graph.o GraphTopologicalSorting.o \
 IntegersQueue.o SortedList.o

example3: example3.o Graph.o GraphTopologicalSorting.o \
 IntegersQueue.o SortedList.o instrumentation.o


# Include dependencies (generated with gcc -MMD)
-include *.d


# Cleanup
clean:
	rm -f *.o *.d
	rm -f $(TARGETS)

