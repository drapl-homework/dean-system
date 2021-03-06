TEST_SRCS := $(wildcard src/*_test.cpp)
TEST_OBJS := $(addprefix obj/, $(notdir $(TEST_SRCS:.cpp=.o)))
TEST_BINS := $(addprefix bin/, $(notdir $(basename $(TEST_SRCS))))
TEST_DEPS := $(TEST_OBJS:.o=.d)

SRCS := $(filter-out src/%_test.cpp, $(wildcard src/*.cpp))
OBJS := $(addprefix obj/, $(notdir $(SRCS:.cpp=.o)))
BINS := $(addprefix bin/, $(notdir $(basename $(SRCS))))
DEPS := $(OBJS:.o=.d)

CC := clang++


all: $(BINS)

test: $(TEST_BINS)

-include $(TEST_DEPS)
-include $(DEPS)

obj/%.d: src/%.cpp
	 $(CC) -MM $^ | sed 's/^/obj\//' > $@
	
obj/%.o: obj/%.d
	$(CC) -c $(addprefix src/, $(notdir $(patsubst %.o,%.cpp,$@))) -o $@ -g

bin/%: obj/%.o
	$(CC) -o $@ $< -g

clean:
	rm -f obj/*
	rm -f bin/*
