CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pthread
# Additional compiler flags for static linking of the standard C++ library and GCC runtime, costs about 1.2 MB per binary.
CXXFLAGS += -static-libstdc++ -static-libgcc
BUILD_DIR = build
TARGETS = $(BUILD_DIR)/server_linux $(BUILD_DIR)/client_linux $(BUILD_DIR)/client_linux_basic

all: $(TARGETS)

$(BUILD_DIR)/server_linux: src/server_linux.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/client_linux: src/client_linux.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/client_linux_basic: src/client_linux_basic.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -rf $(BUILD_DIR)
