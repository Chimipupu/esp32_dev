/**
 * @file app_file_system.cpp
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief ファイルシステム　アプリ
 * @version 0.1
 * @date 2024-12-13
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */

#include "app_file_system.hpp"

static bool s_is_psram = false;
static void heap_print(uint8_t type);
static void heap_print(uint8_t type)
{
    switch (type)
    {
        case HEAP_SRAM:
            DEBUG_PRINTF_RTOS("Heap Size: %d Byte\n", ESP.getHeapSize());
            DEBUG_PRINTF_RTOS("Heap Free: %d Byte\n", ESP.getFreeHeap());
            break;

        case HEAP_PSRAM:
            DEBUG_PRINTF_RTOS("PSRAM Size: %d Byte\n", ESP.getPsramSize());
            DEBUG_PRINTF_RTOS("PSRAM Free: %d Byte\n", ESP.getFreePsram());
            break;

        default:
            break;
    }
}

/**
 * @brief ファイルシステム関連の情報表示
 * 
 */
void app_fs_info(void)
{
    heap_print(HEAP_SRAM);
    heap_print(HEAP_PSRAM);
}

/**
 * @brief malloc(SRAM or PSRAM)のラッパー
 * 
 * @param size mallocするサイズ
 * @param type SRAM or PSRAM
 * @return void* mallocした領域
 */
void* app_fs_heap_malloc(size_t size, uint8_t type)
{
    void *p_malloc = NULL;

    switch (type)
    {
        case HEAP_SRAM:
            p_malloc = heap_caps_malloc(size, MALLOC_CAP_32BIT);
            break;

        case HEAP_PSRAM:
            p_malloc = heap_caps_malloc(size, MALLOC_CAP_SPIRAM);
            break;

        default:
            break;
    }

    return p_malloc;
}

/**
 * @brief PSRAMの初期化
 * 
 */
void app_fs_psram_init(void)
{
    s_is_psram = psramInit();
    if (s_is_psram) {
        DEBUG_PRINTF_RTOS("PSRAM Inited\n");
    } else {
        DEBUG_PRINTF_RTOS("This ESP32 is PSRAM does not exist\n");
    }
}

/**
 * @brief PSRAMのテスト
 * 
 */
void app_fs_psram_test(void)
{
    s_is_psram = psramFound();
    if (s_is_psram) {
        DEBUG_PRINTF_RTOS("[PSRAM Test]\n");
        heap_print(HEAP_PSRAM);
        DEBUG_PRINTF_RTOS("PSRAM malloc size = %d byte\n", PSRAM_MALLOC_TEST_SIZE);
        void *p_pram_val = app_fs_heap_malloc(PSRAM_MALLOC_TEST_SIZE, HEAP_PSRAM);
        heap_print(HEAP_PSRAM);
        DEBUG_PRINTF_RTOS("PSRAM malloc free\n");
        free(p_pram_val);
        heap_print(HEAP_PSRAM);
    }
}