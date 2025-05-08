CCX = g++
#CXXFLAGS = -std=c++11 -Wall -Werror -Wdeprecated -pedantic -DMEMTRACE -g
CXXFLAGS = -std=c++11 -Werror -Wall -Wdeprecated -pedantic -DMEMTRACE -g
TARGET  := telefonkonyv

HEADERS := $(wildcard *.h) $(wildcard *.hpp)
SOURCES := $(wildcard *.cpp)
OBJECTS := $(SOURCES:%.cpp=%.o)

TEST	= bemenet.txt


$(TARGET): $(OBJECTS)
	$(CCX) $^ -g -o $@ -ldl

%.o: %.c $(HEADERS)
	$(CCX) $(CXXFLAGS) -c $<


test:	$(PROG) $(TEST)
	for i in $(TEST); do \
	  ./$(PROG) < $$i ; \
	done


clean:
	rm -rf $(OBJECTS) telefonkonyv
