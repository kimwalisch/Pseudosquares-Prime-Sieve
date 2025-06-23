CXX = c++
CXXFLAGS = -O3 -DNDEBUG -Wall -Wextra -pedantic

SRCS = main.cpp pseudosquares_prime_sieve.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = pseudosquares_prime_sieve

TEST_SRC = test.cpp
TEST_OBJ = $(TEST_SRC:.cpp=.o)
TEST_BIN = test
TEST_OBJS = $(TEST_OBJ) pseudosquares_prime_sieve.o

all: $(TARGET) $(TEST_BIN)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

$(TEST_BIN): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean check

check: $(TEST_BIN)
	./$(TEST_BIN)

clean:
	rm -f $(OBJS) $(TEST_OBJ) $(TEST_BIN) $(TARGET)
