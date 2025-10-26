/**
 * @file app_file_system.hpp
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief ファイルシステム ヘッダ
 * @version 0.1
 * @date 2025-10-26
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */

#ifndef APP_FILE_SYSTEM_HPP
#define APP_FILE_SYSTEM_HPP

#include "common.hpp"

#define HEAP_SRAM                 0
#define HEAP_PSRAM                1
#define PSRAM_MALLOC_TEST_SIZE    32

#ifndef FILE_SYSTEM
    #include "FS.h"
    #include "FFat.h"
    #define FILE_SYSTEM     FFat
    void app_fs_psram_init(void);
    void app_fs_psram_test(void);
#endif /* FILE_SYSTEM */

void app_fs_info(void);
void* app_fs_heap_malloc(size_t size, uint8_t type);

#endif /* APP_FILE_SYSTEM_HPP */