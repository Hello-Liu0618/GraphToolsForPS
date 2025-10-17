CXX = g++
CXXFLAGS = -std=c++17 -I./include -I./include/graph -I./include/graph/algorithm -Wall -Wextra
LDFLAGS = 

# 目录定义
SRC_DIR = src
APP_DIR = app
TEST_DIR = test
BIN_DIR = bin
OBJ_DIR = obj

# 创建目录列表
DIRS = $(BIN_DIR) $(OBJ_DIR)

# 源文件与目标文件
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
APP_SRCS = $(wildcard $(APP_DIR)/*.cpp)
TEST_SRCS = $(wildcard $(TEST_DIR)/*.cpp)

# 生成目标文件路径
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
APP_OBJS = $(patsubst $(APP_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(APP_SRCS))
TEST_OBJS = $(patsubst $(TEST_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(TEST_SRCS))

# 可执行文件目标
APP_TARGET = $(BIN_DIR)/main.exe
TEST_TARGET = $(BIN_DIR)/test.exe

# 默认目标
all: $(DIRS) $(APP_TARGET) $(TEST_TARGET)

# 创建目录
$(DIRS):
	mkdir -p $@

# 编译核心源文件为目标文件
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 编译应用程序源文件为目标文件
$(OBJ_DIR)/%.o: $(APP_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 编译测试程序源文件为目标文件
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 链接生成应用程序
$(APP_TARGET): $(OBJS) $(APP_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# 链接生成测试程序
$(TEST_TARGET): $(OBJS) $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# 清理目标：删除所有生成文件（包括src目录下的临时文件）
clean:
	# 清理bin目录下的可执行文件
	rm -rf $(BIN_DIR)/*.exe
	# 清理obj目录下的目标文件
	rm -rf $(OBJ_DIR)/*.o
	# 清理src目录下可能生成的临时文件（如.o、.d等，保留源代码）
	rm -rf $(SRC_DIR)/*.o $(SRC_DIR)/*.d $(SRC_DIR)/*.gch
	# 清理app和test目录下的临时文件
	rm -rf $(APP_DIR)/*.o $(APP_DIR)/*.d
	rm -rf $(TEST_DIR)/*.o $(TEST_DIR)/*.d
	# 删除空目录（忽略非空错误）
	rmdir $(BIN_DIR) $(OBJ_DIR) 2>/dev/null || true

# 伪目标
.PHONY: all clean $(DIRS)