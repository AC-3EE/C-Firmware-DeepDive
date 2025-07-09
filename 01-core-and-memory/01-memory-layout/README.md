# 01-memory-layout

## 學習目標：
本練習是為了理解 C 語言的五大記憶體區段(`Text`, `Data`, `BSS`, `Stack`, `Heap`)。

C 語言在執行時，記憶體會被劃分為幾個主要的區段（Memory Segments），每個區段都有其特定的用途。了解記憶體如何被分配和使用，才能有效地管理資源，在嵌入式系統這種記憶體受限的環境中，尤為重要。

以下是 C 語言的五大記憶體區段：

## 1. 文字區段 (.text) 與唯讀資料區段 (.rodata)
在現代的編譯器和連結器中，傳統的「文字區段」概念通常會被細分為兩個主要的唯讀區段：.text 和 .rodata。
#### .text 區段 (Text Segment / Code Segment)
- 用途：專門儲存程式的機器碼（Machine Code），也就是 C 語言編譯後的 CPU 指令。
- 特性：
  - 唯讀（Read-Only）：防止程式意外修改自身的指令。
  - 可執行（Executable）：此區段的內容是可被 CPU 執行的。
#### .rodata 區段 (Read-Only Data Segment)
- 用途：專門儲存唯讀的資料，最常見的就是字串常數 (String Literals) 和用 const 修飾的全域變數。
- 特性：
  - 唯讀（Read-Only）：此區段的資料在執行期間不能被修改。
  - 不可執行（Non-Executable）：與 .text 不同，此區段的內容是資料，不是指令，因此被作業系統標記為不可執行，以增加安全性。
### 韌體考量
在微控制器（MCU）中，.text 和 .rodata 這兩個區段的內容都會被燒錄在 **Flash Memory** 或 **ROM** 中。因為它們都是在程式執行前就已確定且不會改變的，將它們放在非揮發性記憶體中是標準做法。將兩者區分開，有助於更精細地分析程式佔用的 Flash 空間。

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

本節簡述 `main.c` 中各個全域、靜態、區域、動態變數以及函式和字串常數，是如何被分配到對應的記憶體區段。
- **文字區段 (.text) 與唯讀資料區段 (.rodata)**
  - `print_memory_layout()` 函式：作為可執行指令，其程式碼本身會被儲存在 **.text 區段**。
  - 字串常數 `"Hello, Memory Layout!"`：這是一個唯讀的字串常數，它會被編譯器放置在 **.rodata 區段**。
- **資料區段 (Data Segment)**
  - `g_initialized_var` 和 `s_initialized_var`：這兩個分別是已初始化的全域變數和靜態變數，其值在程式啟動前就已確定，因此會被儲存在 **Data 區段**。
- **BSS 區段 (Block Started by Symbol Segment)**
  - `g_uninitialized_var` 和 `s_uninitialized_var`：這兩個分別是未初始化的全域變數和靜態變數，它們會被儲存在 **BSS 區段**，並在程式啟動時由系統初始化為 0。
- **堆疊區段 (Stack Segment)**
  - `l_local_var`：這是 `main()` 函式內宣告的區域變數，會被分配在 **Stack 區段**。
- **堆積區段 (Heap Segment)**
  - `heap_var_ptr` 指向的記憶體：透過 `malloc()` 動態配置的記憶體空間，其位址來自於 **Heap 區段**。

## 執行結果與分析：

- **執行結果**
```bash
==================== Memory Addresses ====================
Address of a function (Text)         : 00007FF63A061450
Address of a string literal (rodata) : 00007FF63A064037
----------------------------------------------------------
Address of initialized global (Data) : 00007FF63A063000
Address of initialized static (Data) : 00007FF63A063004
Address of uninitialized global (BSS): 00007FF63A067030
Address of uninitialized static (BSS): 00007FF63A067034
----------------------------------------------------------
Address of heap variable (Heap)      : 000001CBFC33D710
----------------------------------------------------------
Address of local variable (Stack)    : 0000009C269FFB7C
==========================================================
```
- **分析**
從執行結果的記憶體位址可以看出：
1. **Text/rodata 區段**：函式與字串常數的位址位於程式記憶體空間的較低位址，符合其唯讀且最先被載入的特性。
2. **Data 區段**：已初始化的全域變數 `g_initialized_var` 和靜態變數 `s_initialized_var` 的位址非常接近，且緊跟在 Text/rodata 之後。這證實了**無論是全域還是靜態，只要經過初始化，都會被放在 Data 區段**。
3. **BSS 區段**：未初始化的全域變數 `g_uninitialized_var` 和靜態變數 `s_uninitialized_var` 的位址也很接近，並且位於 Data 區段之後。這也證實了**未初始化的全域和靜態變數都會被歸入 BSS 區段**。
4. **Heap 區段**：`malloc` 分配的位址明顯高於靜態儲存區（Data/BSS），符合 **Heap 由低位址向高位址生長的特性**。
5. **Stack 區段**：區域變數 `l_local_var` 的位址在所有觀察到的區段中最高，驗證了 **Stack 由高位址向低位址生長的特性**。
**總結**：
本次執行結果的記憶體位址分佈，再次驗證了 C 語言的記憶體佈局模型。各區段的相對位址高低關係 **（Text/rodata -> Data -> BSS -> Heap -> Stack）** 與理論知識相符，並且**確認了 `static` 變數與 `global` 變數遵循相同的分區規則**。

## 對嵌入式系統的啟示 (Implications for Embedded Systems)：
這次的實驗結果對於資源受限的嵌入式系統開發尤其重要：

- `.data` 與 `.bss` 的權衡：`g_initialized_var` (位於 .data) 會直接佔用 **Flash 空間** (因為初始值需要被儲存) 和 **RAM 空間** (程式啟動時，需要從 Flash 搬運初始值到 RAM)。而 `g_uninitialized_var` (位於 .bss) **只佔用 RAM 空間**，它在 Flash 中只是一個「符號」，不佔據實際儲存空間。因此，在韌體開發中，若非必要，應盡量將全域變數宣告為未初始化的形式（或初始化為 0），以節省寶貴的 Flash 空間。

- **Stack 的大小**：Stack 的位址最高，且向下成長。在沒有作業系統記憶體管理單元 (MMU) 的裸機 (Bare-metal) 環境中，如果函式呼叫層次太深、或是宣告了過大的區域變數，Stack 就有可能向下「踩」到 Heap 或 BSS 區，造成 **堆疊溢位 (Stack Overflow)**，這是韌體中最隱蔽且致命的錯誤之一。因此，精確估算 Stack 的最大用量是韌體穩定性的關鍵。