/**
 * @file app_file_system.hpp
 * @author ちみ/Chimi（https://github.com/Chimipupu）
 * @brief ファイルシステム ヘッダ
 * @version 0.1
 * @date 2024-10-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef APP_FILE_SYSTEM_HPP
#define APP_FILE_SYSTEM_HPP

#include "common.hpp"

#ifndef FILE_SYSTEM
#ifdef YD_ESP32_S3
#include "FS.h"
#include "FFat.h"
#define FILE_SYSTEM     FFat
#else
#include "SPIFFS.h"
#define FILE_SYSTEM     SPIFFS
#endif
#endif

#endif /* APP_FILE_SYSTEM_HPP */