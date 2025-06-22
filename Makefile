CXX = c++
CXXFLAGS = -O3 -DNDEBUG -Wall

all: check

compile_test:
	$(CXX) $(CXXFLAGS) pseudosquares_prime_sieve.cpp -o pseudosquares_prime_sieve

.PHONY: clean
clean:
	rm -f ./pseudosquares_prime_sieve
