# GENERAL CONFIGURATIONS
TARGET 		:= CGO

SRC_DIR 	:= src
INC_DIR		:= inc
RES_DIR		:= res
BUILD_DIR 	:= obj
TARGET_DIR	:= bin


VULKAN_SDK_PATH := /home/berat/libs/VulkanSDK/1.1.106.0/x86_64

# FLAGS, LIBRARIES, INCLUDES
CC 		:= g++
CFLAGS		:= -std=c++17 -Wall
LDFLAGS		:= -L$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan
INCLUDES	:= -I$(VULKAN_SDK_PATH)/include -I/usr/local/include


#-----------------------------------------------------------------------------
# LINE OF NO EDIT
#-----------------------------------------------------------------------------
SOURCES 	:= $(shell find $(SRC_DIR) -type f -name *.cpp)
OBJECTS     	:= $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(SOURCES:.cpp=.o))

# Default Target
all: resources $(TARGET)

test: all
	LD_LIBRARY_PATH=$(VULKAN_SDK_PATH)/lib
	VK_LAYER_PATH=$(VULKAN_SDK_PATH)/etc/explicit_layer.d
	./$(TARGET_DIR)/$(TARGET)

clean:
	@rm -rf $(BUILD_DIR)

cleanall: clean
	@rm -rf $(TARGET_DIR)

rebuild: cleanall all


# Helper targets
resources: directories
#	@cp $(RES_DIR)/* $(TARGET_DIR)/

directories:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(TARGET_DIR)

# Linking
$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET_DIR)/$(TARGET) $^ $(LDFLAGS)

#Compiling
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $< $(INCLUDES)

# No file targets
.PHONY: all test clean cleanall rebuild resource

