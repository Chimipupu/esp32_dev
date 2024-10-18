/**
 * @file app_ftp.cpp
 * @author ちみ/Chimi（https://github.com/Chimipupu）
 * @brief ESP32 FTP アプリ処理
 * @version 0.1
 * @date 2024-10-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "app_ftp.hpp"

FtpServer ftpSrv;
static void ftp_cbk(FtpOperation ftpOperation, unsigned int freeSpace, unsigned int totalSpace);
static void ftp_transfer_cbk(FtpTransferOperation ftpOperation, const char *name, unsigned int transferredSize);

static void ftp_cbk(FtpOperation ftpOperation, unsigned int freeSpace, unsigned int totalSpace)
{
    switch (ftpOperation)
    {
        case FTP_CONNECT:
            Serial.println(F("FTP: Connected!"));
            break;
        case FTP_DISCONNECT:
            Serial.println(F("FTP: Disconnected!"));
            break;
        case FTP_FREE_SPACE_CHANGE:
            Serial.printf("FTP: Free space change, free %u of %u!\n", freeSpace, totalSpace);
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
            Serial.println(F("FTP: Upload start!"));
            break;
        case FTP_UPLOAD:
            Serial.printf("FTP: Upload of file %s byte %u\n", name, transferredSize);
            break;
        case FTP_TRANSFER_STOP:
            Serial.println(F("FTP: Finish transfer!"));
            break;
        case FTP_TRANSFER_ERROR:
            Serial.println(F("FTP: Transfer error!"));
            break;
        default:
            break;
    }
};

void app_ftp_init(void)
{
    ftpSrv.setCallback(ftp_cbk);
    ftpSrv.setTransferCallback(ftp_transfer_cbk);

    Serial.println("Start FTP Server");
    Serial.println("ID : ftp, Password : esp32");
    ftpSrv.begin("ftp", "esp32");
}

void app_ftp_main(void)
{
    ftpSrv.handleFTP();
}
