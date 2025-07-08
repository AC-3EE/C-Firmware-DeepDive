# A single, powerful Makefile in the project root

# === 編譯設定 ===
CC       := gcc
# 將 CFLAGS 放在這裡統一管理
CFLAGS   := -Wall -Wextra -g -MMD -MP

# === 偽目標 ===
.PHONY: all clean rebuild run

# 預設行為：提示使用者如何使用
all:
	@echo "請指定要操作的專案, 例如: make run PROJECT=01-core-and-memory\01-memory-layout"
	@echo "可用目標: make build / run / clean / rebuild"

# 接收 PROJECT 變數, 例如: 01-core-and-memory/01-memory-layout
# SRCS 會是該 PROJECT 路徑下的所有 .c 檔
SRCS     := $(wildcard $(PROJECT)/*.c)
# OBJS 會被放到 build/PROJECT 路徑下
OBJS     := $(patsubst $(PROJECT)/%.c, build/$(PROJECT)/%.o, $(SRCS))
DEPS     := $(OBJS:.o=.d)
# TARGET 可執行檔也會被放到 build/PROJECT 路徑下
TARGET   := build/$(PROJECT)/main

# === 定義 build 目標 ===
.PHONY: build
build: $(TARGET)

# 連結規則
$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) -o $@ $^
	@echo "✔ 編譯完成: $@"

# 編譯規則
build/$(PROJECT)/%.o: $(PROJECT)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(PROJECT) -c $< -o $@

# === 其他目標 ===
run: build
	@echo "🚀 執行中: $(TARGET)"
	@$(TARGET)

clean:
	@if [ -d "build/$(PROJECT)" ]; then \
		rm -rf build/$(PROJECT); \
		echo "🧹 清除完畢: build/$(PROJECT)"; \
	fi

rebuild:
	@make clean PROJECT=$(PROJECT)
	@make build PROJECT=$(PROJECT)

# 包含依賴檔案
-include $(DEPS)