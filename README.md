# C 語言深度實踐筆記 (C Language Deep Dive & Practice)

此專案旨在系統性地、從零開始 (from scratch) 實作 C 語言的核心觀念與資料結構。其目標是為了鞏固嵌入式系統 (Embedded Systems) 開發所需的最核心基礎，並作為我 [STM32 韌體專案](https://github.com/AC-3EE/firmware-practice-log) 的理論與實踐基石。

## 專案目標

透過親手實現底層資料結構與演算法，我期望能達到以下目標：

1.  **深化對記憶體管理的理解**：不僅僅是「會用」指標，而是能從記憶體佈局的層面思考問題。
2.  **掌握韌體開發常用模式**：精通位元操作、狀態機、回呼函式等關鍵技術。
3.  **建立可重用的程式碼庫**：產出高品質、高內聚、低耦合的基礎模組 (例如：Ring Buffer, Linked List)。
4.  **為作業系統 (RTOS) 的學習打下基礎**：透過模擬簡易的排程器與同步機制，理解 RTOS 的運作原理。

## 開發環境

本專案所有程式碼皆為平台獨立 (Platform-Independent)，在標準 C 環境下開發與測試。
* **編譯器 (Compiler)**: GCC
* **建置系統 (Build System)**: Make
* **編輯器 (Editor)**: Visual Studio Code
* **版本控制 (Version Control)**: Git

## 學習路徑 (Learning Roadmap)

### 第一部分：核心語法與記憶體探索 (Core Syntax & Memory Exploration)
- [01-memory-layout](./01-core-and-memory/01-memory-layout/README.md)：剖析程式記憶體分區 (Text, Data, BSS, Stack, Heap)
- [02-bitwise-operations](./01-core-and-memory/02-bitwise-operations/README.md)：精通位元運算與**前置處理器巨集 (Preprocessor Macros)**，實現**可讀性高**的硬體暫存器操作
- [03-advanced-pointers](./01-core-and-memory/03-advanced-pointers/README.md)：深入函式指標、`void*`、`const` 與指標的組合應用
- [04-struct-union-enum](./01-core-and-memory/04-struct-union-enum/README.md)：資料結構定義與記憶體對齊 (Memory Alignment)
- [05-keywords-for-firmware](./01-core-and-memory/05-keywords-for-firmware/README.md)：**剖析 `volatile`, `static`, `extern` 在韌體開發中的關鍵作用與場景**

### 第二部分：韌體常用資料結構 (Common Firmware Data Structures)
- [01-linked-list](./part2_linked_list/README.md)：從零實現單向連結串列 (Singly Linked List)，**並設計穩健的 API (例如：邊界條件與錯誤碼回傳)**
- [02-ring-buffer](./part2_ring_buffer/README.md)：實現高效能、**具備執行期錯誤處理 (Runtime Error Handling) 的環形緩衝區 (Ring Buffer)**，用於 UART/SPI 等通訊場景
- [03-state-machine](./part2_state_machine/README.md)：使用函式指標陣列，建構可擴展的狀態機

### 第三部分：系統程式設計概念 (System Programming Concepts)
- [01-producer-consumer](./part3_producer_consumer/README.md)：模擬生產者-消費者模型，理解同步問題
- [02-cooperative-scheduler](./part3_cooperative_scheduler/README.md)：建構一個簡易的協作式排程器 (Cooperative Scheduler)

>📚 詳細說明
>每個練習資料夾內皆包含一個獨立的 `README.md`，用以說明該主題的學習目標、核心觀念筆記、程式碼實現思路以及最終的測試結果。

## 程式碼風格與測試 (Code Style & Testing)

* **程式碼註解**：力求註解清晰，解釋函式用途、參數以及關鍵邏輯。未來考慮導入 Doxygen 風格。
* **單元測試**：每個模組都會附上一個 `main.c` 作為測試程式，使用 `assert.h` 進行簡單的斷言測試，以確保函式功能符合預期。

## 授權 (License)

MIT License.