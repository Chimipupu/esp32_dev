/**
 * @file app_ftp.cpp
 * @author ちみ/Chimi(https://github.com/Chimipupu)
 * @brief ESP32 FTP アプリ処理
 * @version 0.1
 * @date 2024-10-18
 * 
 * @copyright Copyright (c) 2024 ちみ/Chimi(https://github.com/Chimipupu)
 * 
 */

#ifdef __WIFI_ENABLE__
#include "app_ftp.hpp"
#include "app_file_system.hpp"

FtpServer ftpSrv;
static void ftp_cbk(FtpOperation ftpOperation, unsigned int freeSpace, unsigned int totalSpace);
static void ftp_transfer_cbk(FtpTransferOperation ftpOperation, const char *name, unsigned int transferredSize);

static void ftp_cbk(FtpOperation ftpOperation, unsigned int freeSpace, unsigned int totalSpace)
{
    switch (ftpOperation)
    {
        case FTP_CONNECT:
            DEBUG_PRINTF_RTOS("FTP: Connected!\n");
            break;
        case FTP_DISCONNECT:
            DEBUG_PRINTF_RTOS("FTP: Disconnected!\n");
            break;
        case FTP_FREE_SPACE_CHANGE:
            DEBUG_PRINTF_RTOS("FTP: Free space change, free %u of %u!\n", freeSpace, totalSpace);
            break;
        default:
            break;
    }
};

static void ftp_transfer_cbk(FtpTransferOperation ftpOperation, const char *name, unsigned int transferredSize)
{
    switch (ftpOperation)
    {
        case FTP_UPLOAD_START:
            DEBUG_PRINTF_RTOS("FTP: Upload start!\n");
            break;
        case FTP_UPLOAD:
            DEBUG_PRINTF_RTOS("FTP: Upload of file %s byte %u\n", name, transferredSize);
            break;
        case FTP_TRANSFER_STOP:
            DEBUG_PRINTF_RTOS("FTP: Finish transfer!\n");
            break;
        case FTP_TRANSFER_ERROR:
            DEBUG_PRINTF_RTOS("FTP: Transfer error!\n");
            break;
        default:
            break;
    }
};

void app_ftp_init(void)
{
    ftpSrv.setCallback(ftp_cbk);
    ftpSrv.setTransferCallback(ftp_transfer_cbk);

    DEBUG_PRINTF_RTOS("Start FTP Server\n");
    DEBUG_PRINTF_RTOS("ID : ftp, Password : esp32\n");
    ftpSrv.begin("ftp", "esp32");
}

void app_ftp_main(void)
{
    ftpSrv.handleFTP();
}
#endif /* __WIFI_ENABLE__ */