/**
 * @file app_file_system.cpp
 * @author your name (you@domain.com)
 * @brief ファイルシステム　アプリ
 * @version 0.1
 * @date 2024-12-13
 * 
 * @copyright Copyright (c) 2024 ちみ/Chimi(https://github.com/Chimipupu)
 * 
 */

#include "app_file_system.hpp"

#define PSRAM_MALLOC_TEST_SIZE    100
static bool s_is_psram = false;

void app_fs_info(void)
{
    DEBUG_PRINTF_RTOS("Heap Size: %d Byte\n", ESP.getHeapSize());
    DEBUG_PRINTF_RTOS("Heap Free: %d Byte\n", ESP.getFreeHeap());
#ifdef YD_ESP32_S3
    DEBUG_PRINTF_RTOS("PSRAM Size: %d Byte\n", ESP.getPsramSize());
    DEBUG_PRINTF_RTOS("PSRAM Free: %d Byte\n", ESP.getFreePsram());
#endif /* YD_ESP32_S3 */
}

#ifdef YD_ESP32_S3
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
        void *p_pram_val = heap_caps_malloc(PSRAM_MALLOC_TEST_SIZE, MALLOC_CAP_SPIRAM);
        free(p_pram_val);
    }
}
#endif /* YD_ESP32_S3 */