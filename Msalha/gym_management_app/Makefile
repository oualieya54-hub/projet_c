CC = gcc
APP_NAME = gym_app
SRC_DIR = src
BUILD_DIR = build

# Use pkg-config to get GTK+ 2.0 compiler and linker flags
CFLAGS = -Wall `pkg-config --cflags gtk+-2.0`
LIBS = `pkg-config --libs gtk+-2.0`

# Collect all .c files from the source directory and its subdirectories
SOURCES = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/common/*.c) $(wildcard $(SRC_DIR)/course/*.c) $(wildcard $(SRC_DIR)/member/*.c) $(wildcard $(SRC_DIR)/trainer/*.c) $(wildcard $(SRC_DIR)/salle/*.c)

# Generate object file names in the build directory
OBJECTS = \
    $(BUILD_DIR)/auth.o \
    $(BUILD_DIR)/main.o \
    $(BUILD_DIR)/common/support.o \
    $(BUILD_DIR)/course/course_callbacks.o \
    $(BUILD_DIR)/course/course_integration.o \
    $(BUILD_DIR)/course/course_interface.o \
    $(BUILD_DIR)/course/gcour.o \
    $(BUILD_DIR)/member/member.o \
    $(BUILD_DIR)/member/member_callbacks.o \
    $(BUILD_DIR)/member/member_integration.o \
    $(BUILD_DIR)/member/member_interface.o \
    $(BUILD_DIR)/trainer/entraineur.o \
    $(BUILD_DIR)/trainer/trainer_callbacks.o \
    $(BUILD_DIR)/trainer/trainer_integration.o \
    $(BUILD_DIR)/trainer/trainer_interface.o \
	$(BUILD_DIR)/salle/salle.o \
	$(BUILD_DIR)/salle/salle_interface.o \
    $(BUILD_DIR)/salle/salle_callbacks.o \
    $(BUILD_DIR)/salle/salle_integration.o

.PHONY: all clean

all: $(APP_NAME)

$(APP_NAME): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(APP_NAME) $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(APP_NAME)

run: all
	./$(APP_NAME)
