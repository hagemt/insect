BUILD_TAG = insect
BUILD_TYPE = DEBUG

SAY = $(shell which echo) -e "[$(BUILD_TAG)]"
RM = $(shell which rm) -fv
PC = $(shell which pkg-config) --cflags --libs

CFLAGS_INCLUDE := -I include $(shell $(PC) libcalg-1.0)
CFLAGS_WARNING := -Wall -Wextra -Wno-long-long
CFLAGS_DEBUG   := $(CFLAGS_WARNING) -O0 -ggdb -pedantic
CFLAGS_RELEASE := $(CFLAGS_WARNING) -O3 -DNDEBUG -std=c99
CFLAGS := $(CFLAGS_INCLUDE) $(CFLAGS_$(BUILD_TYPE)) $(CFLAGS)

LIB_SOURCES := $(wildcard core/*.c posix/*.c)
LIB_OBJECTS := $(LIB_SOURCES:.c=.o)

# For concurrent version, uncomment/replace:
#CFLAGS := $(CFLAGS) -DUSE_THREADS -lpthread
#TAG_SOURCES := test/posix/main_pthread.c
TAG_SOURCES := test/main_simple.c
TAG_OBJECTS := $(TAG_SOURCES:.c=.o)

all: $(BUILD_TAG)

check: test

clean:
	@$(SAY) "removing object files"
	@$(RM) $(LIB_OBJECTS) $(TAG_OBJECTS)
	@$(SAY) "removing executables"
	@$(RM) "$(BUILD_TAG)"

install:
	@$(SAY) "just copy '$(BUILD_TAG)' into your \$$PATH"

test: all
	@$(SAY) "TEST" "'./$(BUILD_TAG)"
	@-./$(BUILD_TAG)
	@$(SAY) "TEST" "'./$(BUILD_TAG) .'"
	@./$(BUILD_TAG) .
	@$(SAY) "TEST" "'./$(BUILD_TAG) ..'"
	@./$(BUILD_TAG) ..

.PHONY: all clean check test install

$(BUILD_TAG): $(LIB_OBJECTS) $(TAG_OBJECTS)
	@$(SAY) "LINK" "($^)" "-->" "$@"
	@$(CC) $(CFLAGS) $^ -o "$@"

%.o: %.c
	@$(SAY) "CC" "$<" "-->" "$@"
	@$(CC) $(CFLAGS) -c "$<" -o "$@"
