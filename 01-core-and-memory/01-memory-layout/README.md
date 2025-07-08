# 01-memory-layout

## 學習目標：
本練習是為了理解 C 語言的五大記憶體區段(`Text`, `Data`, `BSS`, `Stack`, `Heap`)。

C 語言在執行時，記憶體會被劃分為幾個主要的區段（Memory Segments），每個區段都有其特定的用途。了解記憶體如何被分配和使用，才能有效地管理資源，在嵌入式系統這種記憶體受限的環境中，尤為重要。

以下是 C 語言的五大記憶體區段：

## 1. 文字區段 (Text Segment / Code Segment)
- **用途**：儲存程式的機器碼（Machine Code），也就是 C 語言編譯後的指令。
- **特性**：
  - **唯讀（Read-Only）**：為了防止程式意外修改自身的指令，這個區段通常是唯讀的。這也意味著多個相同程式的實例可以共享同一個文字區段，例如在 Linux 系統中，這樣可以節省記憶體。
  - **可執行（Executable）**：包含了 CPU 要執行的指令。
 - **韌體考量**：在微控制器（Microcontroller）中，這部分通常儲存在 **Flash memory** 或 **ROM** 中，因為程式碼在燒錄後是不會變動的。

## 2. 資料區段 (Data Segment / Initialized Data Segment)
- **用途**：儲存已經初始化（Initialized）的 **全域變數（Global Variables）** 和 **靜態變數（Static Variables）**。
- **特性**：
  - **可讀寫（Read-Write）**：這些變數的值在程式執行期間是可以被修改的。
  - **編譯時已知大小**：這些變數的大小在編譯時期就已經確定。
- **範例**：
```C
int global_initialized_var = 10; // 儲存在 Data Segment
static int static_initialized_var = 20; // 儲存在 Data Segment
```
- **韌體考量**：這部分通常會從 Flash memory 載入到 **RAM** 中。

## 3. BSS 區段 (Block Started by Symbol Segment)
- **用途**：儲存未初始化（Uninitialized）的 **全域變數** 和 **靜態變數**。當程式啟動時，這些變數會被自動初始化為 0（對於數值型別）或空指標（對於指標型別）。
- **特性**：
  - **可讀寫**。
  - **編譯時已知大小**：只記錄了變數的名稱和大小，並不儲存實際的初始值。這樣做的好處是，在可執行檔（Executable File）中不需要為這些變數分配空間，可以減少可執行檔的大小。
- **範例**：
```C
int global_uninitialized_var; // 儲存在 BSS Segment，啟動時會被初始化為 0
static int static_uninitialized_var; // 儲存在 BSS Segment，啟動時會被初始化為 0
```
- **韌體考量**：程式載入器（或啟動程式碼）會在程式執行前，在 RAM 中為 BSS 區段分配記憶體，並將其內容清零。減少可執行檔的大小對於韌體來說很重要，因為 Flash memory 空間通常有限。

## 4. 堆疊區段 (Stack Segment)

- **用途**：儲存 **區域變數（Local Variables）**、**函數參數（Function Parameters）**、**函數回傳位址（Return Address）** 等。它遵循 **後進先出（LIFO - Last-In, First-Out）** 的原則。
- **特性**：
  - **自動管理**：記憶體的分配和釋放由編譯器和作業系統自動處理。當函數被呼叫時，會在堆疊上分配空間；函數回傳時，這些空間會被自動釋放。
  - **向下生長（Typically Grows Downwards）**：在大多數系統中，堆疊從高位址向低位址生長。
  - **大小有限**：堆疊的大小是固定的，如果過度使用（例如函數遞迴呼叫過深或定義了過大的區域變數），會發生 **堆疊溢位（Stack Overflow）**。
- **韌體考量**：堆疊記憶體位於 **RAM** 中，管理不當容易導致系統崩潰。在 FreeRTOS 等即時作業系統（RTOS）中，每個任務（Task）都有自己的堆疊。需要仔細計算每個任務所需的堆疊大小，以避免溢位。

## 5. 堆積區段 (Heap Segment)
- **用途**：用於 **動態記憶體分配（Dynamic Memory Allocation）**。程式可以在執行時動態地申請和釋放記憶體。
- **特性**：
  - **手動管理**：需要程式設計師使用 `malloc()`, `calloc()`, `realloc()` 來申請記憶體，並使用 `free()` 來釋放記憶體。如果忘記釋放，會導致 **記憶體洩漏（Memory Leak）**。
  - **向上生長（Typically Grows Upwards）**：通常從低位址向高位址生長。
  - **彈性大小**：大小不是固定的，可以根據需求擴展，直到耗盡所有可用記憶體。
- **韌體考量**：堆積記憶體也在 **RAM** 中。在嵌入式系統中，尤其是有 FreeRTOS 的情況下，動態記憶體分配需要特別小心。頻繁地 `malloc`/`free` 可能會導致 **記憶體碎片（Memory Fragmentation）**，這會讓大塊連續記憶體難以分配，從而影響系統穩定性。因此，業界習慣上在韌體開發中盡量避免或限制 `malloc`/`free` 的使用，傾向於在啟動時預先分配好所需的**記憶體池（Memory Pool）**。

## 程式碼說明：

本節簡述 `main.c` 中各個全域、區域、動態變數以及函式和字串常數，是如何被分配到對應的記憶體區段。
- **文字區段 (Text Segment / Code Segment)**
  - `print_memory_layout()` 函式：作為可執行指令，其程式碼本身會被儲存在 **Text 區段**。
  - 字串常數 `"Hello, Memory Layout!"`：這是一個唯讀的字串常數，它會被編譯器放置在 **Text 區段** 或專門的唯讀資料區段（`rodata 區段`，通常緊鄰 `Text` 區段或被視為其一部分）。
- **資料區段 (Data Segment / Initialized Data Segment)**
  - `g_initialized_var`：這是一個已初始化（Initialized）的全域變數（`int g_initialized_var = 10;`），其值在程式啟動前就已確定，因此會被儲存在 **Data** 區段。
- **BSS 區段 (Block Started by Symbol Segment)**
  - `g_uninitialized_var`：這是一個未初始化（Uninitialized）的全域變數（`int g_uninitialized_var;`），C 語言標準保證其在程式啟動時會被初始化為 0。它會被儲存在 **BSS 區段**，這樣可以減少可執行檔的大小，因為檔案中只需記錄其存在，實際的 0 值是在程式載入記憶體時才填充的。
- **堆疊區段 (Stack Segment)**
  - `l_local_var`：這是 `main()` 函式內宣告的區域變數（`int l_local_var = 20;`）。區域變數的生命週期僅限於其作用域（即 `main()` 函式被呼叫期間），因此它會被分配在 **Stack 區段**。當 `main()` 函式執行完畢並回傳時，`l_local_var` 所佔用的堆疊空間會自動被釋放。
- **堆積區段 (Heap Segment)**
  - `heap_var_ptr` 指向的記憶體：透過 `malloc(sizeof(int))` 動態配置的記憶體空間，其位址來自於 **Heap 區段**。這塊記憶體由程式設計師手動管理，需要明確使用 `free(heap_var_ptr)` 來釋放，以避免記憶體洩漏。

## 執行結果與分析：

- **執行結果**
```bash
==================== Memory Addresses ====================
Address of a function (Text)         : 00007FF72CB91450
Address of a string literal (rodata) : 00007FF72CB94037
----------------------------------------------------------
Address of initialized global (Data) : 00007FF72CB93000
Address of uninitialized global (BSS): 00007FF72CB97030
----------------------------------------------------------
Address of heap variable (Heap)      : 0000015D6F29E250
----------------------------------------------------------
Address of local variable (Stack)    : 000000141AFFF92C
==========================================================
```
- **分析**
從執行結果的記憶體位址可以看出：
1. **Text 區段 (0x00007FF72CB91450)** 和 **rodata (字串常數 0x00007FF72CB94037)** 位於程式記憶體空間的較低位址。這符合程式碼和唯讀資料通常被放置在記憶體底部的預期。其中，`rodata`（字串常數）與 `Text` 區段的位址非常接近，進一步證實了它們通常相鄰或共享類似的唯讀屬性。
2. **Data 區段 (0x00007FF72CB93000)** 和 **BSS 區段 (0x00007FF72CB97030)** 緊隨 `Text`/`rodata` 之後，位於中低位址區塊。這與資料區段通常在程式碼區段之後的典型記憶體佈局相符。值得注意的是，`g_uninitialized_var` (BSS) 的位址 `(0x00007FF72CB97030)` 確實比 `g_initialized_var` (Data) 的位址 (`0x00007FF72CB93000`) 高，表明 BSS 區段通常位於 Data 區段之後。
3. **Heap 區段 (0x0000015D6F29E250)** 的位址明顯高於 Text、Data 和 BSS 區段。這符合 **Heap 區段通常從較低位址向高位址增長** 的特性，它動態地分配記憶體，並在程式運行期間按需擴展。
4. **Stack 區段 (0x000000141AFFF92C)** 具有最高的記憶體位址。這驗證了 **Stack 區段通常從高位址向低位址增長** 的特性。區域變數 `l_local_var` 被分配在堆疊上，其位址在所有觀察到的區段中最高。
**總結**：
本次執行結果的記憶體位址分佈，**完全驗證了 C 語言程式在典型作業系統環境下的記憶體佈局模型**。各區段的相對位址高低關係 **（Text/rodata -> Data -> BSS -> Heap -> Stack）** 與理論知識相符。這對理解韌體開發中資源受限的記憶體管理至關重要，例如區分靜態分配（Text, Data, BSS）與動態分配（Stack, Heap）的記憶體特性與管理方式。

## 對嵌入式系統的啟示 (Implications for Embedded Systems)：
這次的實驗結果對於資源受限的嵌入式系統開發尤其重要：

- `.data` 與 `.bss` 的權衡：`g_initialized_var` (位於 .data) 會直接佔用 **Flash 空間** (因為初始值需要被儲存) 和 **RAM 空間** (程式啟動時，需要從 Flash 搬運初始值到 RAM)。而 `g_uninitialized_var` (位於 .bss) **只佔用 RAM 空間**，它在 Flash 中只是一個「符號」，不佔據實際儲存空間。因此，在韌體開發中，若非必要，應盡量將全域變數宣告為未初始化的形式（或初始化為 0），以節省寶貴的 Flash 空間。

- **Stack 的大小**：Stack 的位址最高，且向下成長。在沒有作業系統記憶體管理單元 (MMU) 的裸機 (Bare-metal) 環境中，如果函式呼叫層次太深、或是宣告了過大的區域變數，Stack 就有可能向下「踩」到 Heap 或 BSS 區，造成 **堆疊溢位 (Stack Overflow)**，這是韌體中最隱蔽且致命的錯誤之一。因此，精確估算 Stack 的最大用量是韌體穩定性的關鍵。