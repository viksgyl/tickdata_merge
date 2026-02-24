BIN_NAME := TickMerger
CXX?=g++
SRC_EXT = cc
SRC_PATH = .
INC_PATH = ../include
LIBS = -lpthread -lboost_program_options -lstdc++fs
CXX_FLAGS = -std=c++17 -Wall -Wextra -g
INCLUDES = -I $(INC_PATH)
BUILD_PATH = ../obj
BIN_PATH = ../bin

SOURCES = $(shell find $(SRC_PATH) -name '*.$(SRC_EXT)' -printf '%T@\t%p\n' \
						| sort -k 1nr | cut -f2-)
rwildcard = $(foreach d, $(wildcard $1*), $(call rwildcard,$d/,$2) \
						$(filter $(subst *,%,$2), $d))
ifeq ($(SOURCES),)
	SOURCES := $(call rwildcard, $(SRC_PATH), *.$(SRC_EXT))
endif

OBJECTS = $(SOURCES:$(SRC_PATH)/%.$(SRC_EXT)=$(BUILD_PATH)/%.o)
DEPS = $(OBJECTS:.o=.d)

.PHONY: release
release: dirs
	@$(MAKE) all --no-print-directory

# Create the directories used in the build
.PHONY: dirs
dirs:
	@echo "Creating directories"
	@mkdir -p $(dir $(OBJECTS))
	@mkdir -p $(BIN_PATH)

.PHONY: clean
clean:
	@echo "Deleting $(BIN_NAME) symlink"
	@$(RM) ../$(BIN_NAME)
	@echo "Deleting directories"
	@$(RM) -r $(BIN_PATH) $(BUILD_PATH) ../output/*

# Main rule, checks the executable and symlinks to the output
all: $(BIN_PATH)/$(BIN_NAME)
	@echo "Making symlink: $(BIN_NAME) -> $<"
	@$(RM) $(BIN_NAME)
	@ln -sf bin/$(BIN_NAME) ../$(BIN_NAME)

$(BIN_PATH)/$(BIN_NAME): $(OBJECTS)
	@echo "Linking: $@"
	$(CMD_PREFIX)$(CXX) $(OBJECTS) $(LDFLAGS) -o $@ $(LIBS)

-include $(DEPS)

$(BUILD_PATH)/%.o: $(SRC_PATH)/%.$(SRC_EXT)
	@echo "Compiling: $< -> $@"
	$(CMD_PREFIX)$(CXX) $(CXX_FLAGS) $(INCLUDES) -MP -MMD -c $< -o $@
