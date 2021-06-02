CXX_COMPILER:=g++
SRC_DIR := .
CXX_FLAGS_DEFAULT := -Ofast
CXX_FLAGS_DEBUG := -g -O0
CXX_FLAGS_PROFILER := -g -O0 -lprofiler -ltcmalloc
BUILD_DIR := build

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/multiples_debug: $(SRC_DIR)/multiples_roblox.cc | $(BUILD_DIR)
	$(CXX_COMPILER) $(CXX_FLAGS_DEBUG) -o $@ $<

$(BUILD_DIR)/multiples: $(SRC_DIR)/multiples_roblox.cc | $(BUILD_DIR)
	$(CXX_COMPILER) $(CXX_FLAGS_DEFAULT) -o $@ $<

$(BUILD_DIR)/multiples_profiler: $(SRC_DIR)/multiples_roblox.cc | $(BUILD_DIR)
	$(CXX_COMPILER) $(CXX_FLAGS_PROFILER) -o $@ $<	

all: \
    $(BUILD_DIR)/multiples_debug \
	$(BUILD_DIR)/multiples_profiler \
	$(BUILD_DIR)/multiples

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean

