/**
 * @file app_file_system.hpp
 * @author ちみ/Chimi(https://github.com/Chimipupu)
 * @brief ファイルシステム ヘッダ
 * @version 0.1
 * @date 2024-10-19
 * 
 * @copyright Copyright (c) 2024 ちみ/Chimi(https://github.com/Chimipupu)
 * 
 */

#ifndef APP_FILE_SYSTEM_HPP
#define APP_FILE_SYSTEM_HPP

#include "common.hpp"

#define HEAP_SRAM                 0
#define HEAP_PSRAM                1
#define PSRAM_MALLOC_TEST_SIZE    32

#ifndef FILE_SYSTEM
    #ifdef YD_ESP32_S3
        #include "FS.h"
        #include "FFat.h"
        #define FILE_SYSTEM     FFat
        void app_fs_psram_init(void);
        void app_fs_psram_test(void);
    #else
        #include "SPIFFS.h"
        #define FILE_SYSTEM     SPIFFS
        #endif /* YD_ESP32_S3 */
#endif /* FILE_SYSTEM */

void app_fs_info(void);
void* app_fs_heap_malloc(size_t size, uint8_t type);

#endif /* APP_FILE_SYSTEM_HPP */