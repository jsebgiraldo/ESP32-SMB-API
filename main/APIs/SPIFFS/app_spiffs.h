/*
 * tasks_common.h
 *
 *  Created on: Apr 08, 2022
 *      Author: Juan Sebastian Giraldo Duque
 */

#ifndef MAIN_LOCAL_DB_H
#define MAIN_LOCAL_DB_H

#include "esp_spiffs.h"
#include "esp_vfs.h"

#include "esp_http_server.h"

/* Max length a file path can have on storage */
#define FILE_PATH_MAX (ESP_VFS_PATH_MAX + CONFIG_SPIFFS_OBJ_NAME_LEN)

/* Max size of an individual file. Make sure this
 * value is same as that set in upload_script.html */
#define MAX_FILE_SIZE   (200*1024) // 200 KB
#define MAX_FILE_SIZE_STR "200KB"

/* Scratch buffer size */
#define SCRATCH_BUFSIZE  8192

enum LOCAL_DB_RESULT{
	LOCAL_DB_ERROR,
	LOCAL_DB_SUCCESS,
};

struct file_server_data {
    /* Base path of file storage */
    char base_path[ESP_VFS_PATH_MAX + 1];

    /* Scratch buffer for temporary storage during file transfer */
    char scratch[SCRATCH_BUFSIZE];
};

static const char FILE_SYSTEM_BASE_PATH[] = "/spiffs";

/*!
   \brief "local_db_mount_file_system"
   \param "Param description"
   \pre "Pre-conditions"
   \post "Post-conditions"
   \return "Return of the function"
*/
char app_spiffs_mount_file_system(void);

esp_vfs_spiffs_conf_t* app_spiffs_get_conf(void);

esp_err_t view_get_handler(httpd_req_t *req);
esp_err_t upload_post_handler(httpd_req_t *req);
esp_err_t download_get_handler(httpd_req_t *req);
esp_err_t delete_post_handler(httpd_req_t *req);

#endif /* MAIN_LOCAL_DB_H */