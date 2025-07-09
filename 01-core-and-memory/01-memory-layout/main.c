// 01-core-and-memory/01-memory-layout/main.c

#include <stdio.h>
#include <stdlib.h>

// --- .Data Section ---
// 已初始化的全域變數
int g_initialized_var = 10;
// NEW: 已初始化的靜態全域變數，同樣儲存在 .data 區段
static int s_initialized_var = 11; 

// --- .BSS Section ---
// 未初始化的全域變數
int g_uninitialized_var;
// NEW: 未初始化的靜態全域變數，同樣儲存在 .bss 區段
static int s_uninitialized_var;

// --- .Text Section ---
// 函式本身是一段程式碼
void print_memory_layout() {
    printf("這是一個在 .text 區段的函式。\n");
}


int main() {
    // --- .Stack Section ---
    // 區域變數
    int l_local_var = 20;

    // --- .Heap Section ---
    // 動態配置的記憶體
    int *heap_var_ptr = (int*) malloc(sizeof(int));
    if (heap_var_ptr == NULL) {
        perror("malloc failed");
        return -1;
    }
    *heap_var_ptr = 30;

    // --- .rodata Section (Constant strings) ---
    // 字串常數
    const char *string_literal = "Hello, Memory Layout!";


    printf("==================== Memory Addresses ====================\n");
    // .Text: 函式指標
    printf("Address of a function (Text)         : %p\n", &print_memory_layout);
    // .rodata: 唯讀的字串常數
    printf("Address of a string literal (rodata) : %p\n", (void*)string_literal);
    printf("----------------------------------------------------------\n");
    // .Data: 已初始化的全域/靜態變數
    printf("Address of initialized global (Data) : %p\n", &g_initialized_var);
    printf("Address of initialized static (Data) : %p\n", &s_initialized_var); // NEW
    // .BSS: 未初始化的全域/靜態變數
    printf("Address of uninitialized global (BSS): %p\n", &g_uninitialized_var);
    printf("Address of uninitialized static (BSS): %p\n", &s_uninitialized_var); // NEW
    printf("----------------------------------------------------------\n");
    // .Heap: 動態配置的記憶體空間
    printf("Address of heap variable (Heap)      : %p\n", (void*)heap_var_ptr);
    printf("----------------------------------------------------------\n");
    // .Stack: 函式內宣告的區域變數
    printf("Address of local variable (Stack)    : %p\n", &l_local_var);
    printf("==========================================================\n");

    // 釋放動態配置的記憶體
    free(heap_var_ptr);

    return 0;
}