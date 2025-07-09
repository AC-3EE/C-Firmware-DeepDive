#include <stdio.h>
#include <stdint.h>

/* =================================================================================
 * 1. 虛擬硬體與暫存器定義 (Virtual Hardware & Register Definitions)
 * ================================================================================= */

typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t ODR;
} GPIO_TypeDef;

static GPIO_TypeDef g_gpiob_regs;
#define GPIOB (&g_gpiob_regs)

/* =================================================================================
 * 2. 可讀性高的巨集定義 (Readable Macro Definitions)
 * ================================================================================= */

// --- 通用位元操作巨集 ---
#define BIT_SET(REG, MASK)      ((REG) |= (MASK))
#define BIT_CLEAR(REG, MASK)    ((REG) &= ~(MASK))
#define BIT_READ(REG, MASK)     (((REG) & (MASK)) != 0)
// Exercise 2: New macro to get pin mode
// Takes REG (the MODER register) and PIN number as input
// Returns the 2-bit mode value (0b00 to 0b11)
#define GET_PIN_MODE(REG, PIN)  (((REG) >> ((PIN) * 2)) & 0b11)

// --- 特定功能的巨集 ---

// GPIO Pin Definitions
#define LED_PIN           5
#define USER_BUTTON_PIN   8

// GPIO 模式 (每個 Pin 用 2 個位元)
#define GPIO_MODE_INPUT     0b00
#define GPIO_MODE_OUTPUT    0b01
#define GPIO_MODE_ALT       0b10
#define GPIO_MODE_ANALOG    0b11

// --- Exercise 1: Combined Masks for Pin 5 and Pin 8 ---
// 1. Positions in MODER register (each pin takes 2 bits)
#define LED_PIN_MODER_POS           (LED_PIN * 2)
#define USER_BUTTON_MODER_POS       (USER_BUTTON_PIN * 2)

// 2. Masks for clearing the mode bits
#define LED_PIN_MODER_CLEAR_MASK        (0b11 << LED_PIN_MODER_POS)
#define USER_BUTTON_MODER_CLEAR_MASK    (0b11 << USER_BUTTON_MODER_POS)
#define PINS_MODER_CLEAR_MASK           (LED_PIN_MODER_CLEAR_MASK | USER_BUTTON_MODER_CLEAR_MASK)

// 3. Values for setting the new modes
#define LED_PIN_MODE_VALUE          (GPIO_MODE_OUTPUT << LED_PIN_MODER_POS)
#define USER_BUTTON_MODE_VALUE      (GPIO_MODE_INPUT << USER_BUTTON_MODER_POS)
#define PINS_MODE_SET_VALUE         (LED_PIN_MODE_VALUE | USER_BUTTON_MODE_VALUE)

// 4. Mask for ODR register (each pin takes 1 bit)
#define LED_PIN_ODR_MASK (1U << LED_PIN)


/* =================================================================================
 * 3. 應用程式 (Application)
 * ================================================================================= */

void print_gpio_regs() {
    printf("GPIOB->MODER = 0x%08X\n", GPIOB->MODER);
    printf("GPIOB->ODR   = 0x%08X\n", GPIOB->ODR);
    printf("----------------------------------------\n");
}

int main() {
    printf("Bitwise Operations Tutorial Start\n");
    printf("Initial State:\n");
    GPIOB->MODER = 0x00000000;
    GPIOB->ODR   = 0x00000000;
    print_gpio_regs();

    // --- Step 1: Set Pin 5 to Output mode and Pin 8 to Input mode ---
    printf("Step 1: Set Pin 5 to Output and Pin 8 to Input\n");
    // a. Clear the mode bits for both Pin 5 and Pin 8 simultaneously
    BIT_CLEAR(GPIOB->MODER, PINS_MODER_CLEAR_MASK);
    // b. Set the new modes for both pins simultaneously
    BIT_SET(GPIOB->MODER, PINS_MODE_SET_VALUE);
    print_gpio_regs();

    // --- Step 2: Set Pin 5 Output to High ---
    printf("Step 2: Set Pin 5 Output to High\n");
    BIT_SET(GPIOB->ODR, LED_PIN_ODR_MASK);
    print_gpio_regs();

    // --- Step 3: Set Pin 5 Output to Low ---
    printf("Step 3: Set Pin 5 Output to Low\n");
    BIT_CLEAR(GPIOB->ODR, LED_PIN_ODR_MASK);
    print_gpio_regs();

    // --- Step 4: Toggle Pin 5 Output ---
    printf("Step 4: Toggle Pin 5 Output (becomes High)\n");
    GPIOB->ODR ^= LED_PIN_ODR_MASK;
    print_gpio_regs();

    // --- Step 5: Read Pin 5 Output State ---
    printf("Step 5: Read Pin 5 Output State\n");
    if (BIT_READ(GPIOB->ODR, LED_PIN_ODR_MASK)) {
        printf("Pin 5 is currently High\n");
    } else {
        printf("Pin 5 is currently Low\n");
    }
    printf("========================================\n");


    // --- Exercise 2: Use the new macro to verify pin mode ---
    printf("\nExercise 2: Verifying Pin Modes\n");

    uint32_t pin5_mode = GET_PIN_MODE(GPIOB->MODER, LED_PIN);
    printf("Reading Pin %d mode... Value: %u\n", LED_PIN, pin5_mode);

    if (pin5_mode == GPIO_MODE_OUTPUT) {
        printf("Verification PASSED: Pin %d is configured as Output.\n", LED_PIN);
    } else {
        printf("Verification FAILED: Pin %d is NOT configured as Output.\n", LED_PIN);
    }

    uint32_t pin8_mode = GET_PIN_MODE(GPIOB->MODER, USER_BUTTON_PIN);
    printf("Reading Pin %d mode... Value: %u\n", USER_BUTTON_PIN, pin8_mode);

    if (pin8_mode == GPIO_MODE_INPUT) {
        printf("Verification PASSED: Pin %d is configured as Input.\n", USER_BUTTON_PIN);
    } else {
        printf("Verification FAILED: Pin %d is NOT configured as Input.\n", USER_BUTTON_PIN);
    }
    printf("========================================\n");
    
    return 0;
}