// 01-core-and-memory/01-memory-layout/main.c

#include <stdio.h>
#include <stdlib.h>

// --- .Data Section ---
// g_initialized_var 是一個已初始化的全域變數。
// 它會被儲存在 .data 區段。
int g_initialized_var = 10;

// --- .BSS Section ---
// g_uninitialized_var 是一個未初始化的全域變數。
// C語言標準保證它會被初始化為0，並儲存在 .bss 區段，以節省執行檔大小。
int g_uninitialized_var;

// --- .Text Section ---
// main 函式本身是一段程式碼，它的機器指令會被儲存在 .text 區段。
void print_memory_layout() {
    printf("這是一個在 .text 區段的函式。\n");
}


int main() {
    // --- .Stack Section ---
    // l_local_var 是 main 函式內的區域變數。
    // 它會被儲存在 stack (堆疊) 中。
    int l_local_var = 20;

    // --- .Heap Section ---
    // 使用 malloc 動態配置的記憶體。
    // 這塊記憶體的位址來自 heap (堆積)。
    int *heap_var_ptr = (int*) malloc(sizeof(int));
    if (heap_var_ptr == NULL) {
        perror("malloc failed");
        return -1;
    }
    *heap_var_ptr = 30;


    // --- .Text Section (Constant strings) ---
    // "Hello, Memory Layout!" 這樣的字串常數，通常也被儲存在 .text 區段，
    // 或者是另一個唯讀資料區段 (rodata)。
    const char *string_literal = "Hello, Memory Layout!";


    printf("==================== Memory Addresses ====================\n");
    printf("Address of a function (Text)         : %p\n", &print_memory_layout);
    printf("Address of a string literal (rodata) : %p\n", (void*)string_literal);
    printf("----------------------------------------------------------\n");
    printf("Address of initialized global (Data) : %p\n", &g_initialized_var);
    printf("Address of uninitialized global (BSS): %p\n", &g_uninitialized_var);
    printf("----------------------------------------------------------\n");
    printf("Address of heap variable (Heap)      : %p\n", (void*)heap_var_ptr);
    printf("----------------------------------------------------------\n");
    printf("Address of local variable (Stack)    : %p\n", &l_local_var);
    printf("==========================================================\n");

    // 釋放動態配置的記憶體
    free(heap_var_ptr);

    return 0;
}