SRC_DIR = src
OBJ_DIR = compile
INC_DIR = include
TEST_DIR = test
DOXYFILE = Doxyfile
DOCUMENT_DIR = document

# Tìm tất cả file .c trong SRC_DIR và TEST_DIR
SRCS = $(shell find $(SRC_DIR) -type f -name "*.c")
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)

# Tạo danh sách file object từ SRC_DIR
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Biên dịch
CC = gcc
CFLAGS = -Wall -I$(INC_DIR) `pkg-config --cflags gtk+-3.0`
LDFLAGS = `pkg-config --libs gtk+-3.0`
TARGET = project

# ====================================
# Mục tiêu chính: Build project chính
# ====================================
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) -lgmp $(LDFLAGS) -Wl,--export-all-symbols

# Tạo file .o trong compile/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

#=====================================
#Hỗ trợ tạo doc
#=====================================
doc:
	doxygen $(DOXYFILE)

# ====================================
# Build các chương trình test
# ====================================
# Tạo danh sách .exe cho mỗi file test
TEST_EXES = $(patsubst $(TEST_DIR)/%.c, $(TEST_DIR)/%.exe, $(TEST_SRCS))
TEST_OBJS = $(filter-out $(OBJ_DIR)/main.o, $(OBJS))

# Build tất cả file test
test: $(TEST_EXES)

# Build từng file test
$(TEST_DIR)/%.exe: $(TEST_DIR)/%.c $(TEST_OBJS)
	$(CC) $(CFLAGS) $< $(TEST_OBJS) -o $@ $(LDFLAGS)

# ====================================
# Clean
# ====================================
clean:
	rm -rf $(OBJ_DIR) $(TARGET) $(TEST_EXES) $(DOCUMENT_DIR)

doc_erase:
	rm -rf $(DOCUMENT_DIR)
