/*

 * app_smb.h
 *
 *  Created on: Apr 26, 2023
 *      Author: Juan Sebastian Giraldo Duque
 */

#ifndef APP_SMB_H
#define APP_SMB_H

#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "smb2.h"
#include "libsmb2.h"
#include "libsmb2-raw.h"

typedef struct {
    char *user;
    char *password;
}smb_config_t;

void app_smb_init(void);

void app_smb_set_password(char *pass);
void app_smb_set_user(char *user);

smb_config_t* app_smb_get_config(void);

const char *app_smb_ls(char *path);
esp_err_t app_smb_ota(char *path);

#endif /* APP_SMB_H */