# Makefile

CXX = g++
CXXFLAGS = -O0 -o piggybank bank-account.cc

TARGETS = piggybank

all: $(TARGETS)

# clean up
clean:
	rm -f $(TARGETS) *.o *~

spartan: clean
	rm -fr *~
