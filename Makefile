SRC_DIR = src
OBJ_DIR = compile
INC_DIR = include

# Tìm tất cả file .c trong SRC_DIR và các thư mục con
SRCS = $(shell find $(SRC_DIR) -type f -name "*.c")
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

CC = gcc
CFLAGS = -Wall -I$(INC_DIR) `pkg-config --cflags gtk+-3.0`
LDFLAGS = `pkg-config --libs gtk+-3.0`
TARGET = project

# Tạo file thực thi từ danh sách object files
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) -lgmp $(LDFLAGS) -Wl,--export-all-symbols

# Tạo object file cho mỗi file .c
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@) 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)
