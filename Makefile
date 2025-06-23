CXX = c++
CXXFLAGS = -O3 -DNDEBUG -Wall

SRCS = main.cpp pseudosquares_prime_sieve.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = pseudosquares_prime_sieve

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)
