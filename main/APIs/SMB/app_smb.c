/*
 * app_smb.c
 *
 *  Created on: Apr 26, 2023
 *      Author: Juan Sebastian Giraldo Duque
 */

#include "app_smb.h"

#include "wifi_app.h"

#include "esp_ota_ops.h"

static const char *TAG = "SMB";

// Used for cat function
#define MAXBUF 16 * 1024
uint8_t buf[MAXBUF];
uint32_t pos;

// Used for returning the SMB configuration
smb_config_t *smb_config = NULL;

smb_config_t* app_smb_get_config(void)
{
	return smb_config;
}

void app_smb_set_password(char *pass)
{
    if(pass != NULL) smb_config->password = pass;
}

void app_smb_set_user(char *user)
{
    if(user != NULL) smb_config->user = user;
}

void app_smb_init(void)
{
    // Allocate memory for the SMB configuration
	smb_config = (smb_config_t*)malloc(sizeof(smb_config_t));
	memset(smb_config, 0x00, sizeof(smb_config_t));

    app_smb_set_user("none");
    app_smb_set_password("none");
}

esp_err_t app_smb_ota(char*path)
{
	esp_err_t err;

	smb_config_t *smb_config = app_smb_get_config();
    if (smb_config == NULL) {
		ESP_LOGE(TAG, "Failed to get smb config");
		return ESP_FAIL;
	}

	struct smb2_context *smb2;
	struct smb2_url *url;
	struct smb2fh *fh;
	int count;

	memset(buf,0x00,MAXBUF);

	smb2 = smb2_init_context();
	if (smb2 == NULL) {
		ESP_LOGE(TAG, "Failed to init context");
		return ESP_FAIL;
	}

    tcpip_adapter_ip_info_t ip_info;
    tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &ip_info);

	char smburl[64];
    sprintf(smburl, "smb://%s@%s/%s", smb_config->user, ip4addr_ntoa(&ip_info.gw), path);
	ESP_LOGI(TAG, "smburl=%s", smburl);

	smb2_set_password(smb2, smb_config->password);


	url = smb2_parse_url(smb2, smburl);
	if (url == NULL) {
		ESP_LOGE(TAG, "Failed to parse url: %s", smb2_get_error(smb2));
		return ESP_FAIL;
	}

	smb2_set_security_mode(smb2, SMB2_NEGOTIATE_SIGNING_ENABLED);

	if (smb2_connect_share(smb2, url->server, url->share, url->user) < 0) {
		ESP_LOGE(TAG, "smb2_connect_share failed. %s", smb2_get_error(smb2));
		return ESP_FAIL;
	}

	ESP_LOGI(TAG, "url->path=%s", url->path);
	fh = smb2_open(smb2, url->path, O_RDONLY);
	if (fh == NULL) {
		ESP_LOGE(TAG, "smb2_open failed. %s", smb2_get_error(smb2));
		while(1){ vTaskDelay(1); }
	}

	esp_ota_handle_t ota_handle;
	const esp_partition_t *update_partition = esp_ota_get_next_update_partition(NULL);

	err = esp_ota_begin(update_partition, OTA_SIZE_UNKNOWN, &ota_handle);

	while ((count = smb2_pread(smb2, fh, buf, MAXBUF, pos)) != 0) {
		if (count == -EAGAIN) {
			continue;
		}
		if (count < 0) {
			ESP_LOGE(TAG, "Failed to read file. %s", smb2_get_error(smb2));
			break;
		}
		ESP_LOGI(TAG,"OTA in progress");
		esp_ota_write(ota_handle, buf, count);
		//write(0, buf, count);
		pos += count;
	};

	err = esp_ota_end(ota_handle);
	if (err == ESP_OK)
	{
		// Lets update the partition
		err = esp_ota_set_boot_partition(update_partition);
		if (err == ESP_OK)
		{
			const esp_partition_t *boot_partition = esp_ota_get_boot_partition();
			ESP_LOGI(TAG,"http_server_OTA_update_handler: Next boot partition subtype %d at offset 0x%x", boot_partition->subtype, boot_partition->address);
		}
		else
		{
			ESP_LOGE(TAG,"http_server_OTA_update_handler: FLASHED ERROR!!!");
		}
	}
	else
	{
		ESP_LOGE(TAG,"http_server_OTA_update_handler: esp_ota_end ERROR!!!");
	}

	smb2_close(smb2, fh);
	smb2_disconnect_share(smb2);
	smb2_destroy_url(url);
	smb2_destroy_context(smb2);

	pos = 0;
    return err;
}

const char *app_smb_ls(char *path)
{

    smb_config_t *smb_config = app_smb_get_config();
    if (smb_config == NULL) {
		ESP_LOGE(TAG, "Failed to get smb config");
		return NULL;
	}

    struct smb2_context *smb2;
	struct smb2_url *url;
	struct smb2dir *dir;
	struct smb2dirent *ent;
	char *link;

	smb2 = smb2_init_context();
	if (smb2 == NULL) {
		ESP_LOGE(TAG, "Failed to init context");
		return NULL;
	}

    tcpip_adapter_ip_info_t ip_info;
    tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &ip_info);

	char smburl[64];
    sprintf(smburl, "smb://%s@%s/%s", smb_config->user, ip4addr_ntoa(&ip_info.gw), path);
	ESP_LOGI(TAG, "smburl=%s", smburl);

	smb2_set_password(smb2, smb_config->password);


	url = smb2_parse_url(smb2, smburl);
	if (url == NULL) {
		ESP_LOGE(TAG, "Failed to parse url: %s", smb2_get_error(smb2));
		return smb2_get_error(smb2);
	}

	smb2_set_security_mode(smb2, SMB2_NEGOTIATE_SIGNING_ENABLED);

	if (smb2_connect_share(smb2, url->server, url->share, url->user) < 0) {
		ESP_LOGE(TAG, "smb2_connect_share failed. %s", smb2_get_error(smb2));
		return smb2_get_error(smb2);
	}

	dir = smb2_opendir(smb2, url->path);
	if (dir == NULL) {
		ESP_LOGE(TAG, "smb2_opendir failed. %s", smb2_get_error(smb2));
		return smb2_get_error(smb2);
	}

    char ls_content[255];
    char ls_content_temp[50];

	while ((ent = smb2_readdir(smb2, dir))) {
		char *type;

		switch (ent->st.smb2_type) {
		case SMB2_TYPE_LINK:
			type = "LINK";
			break;
		case SMB2_TYPE_FILE:
			type = "FILE";
			break;
		case SMB2_TYPE_DIRECTORY:
			type = "DIRECTORY";
			break;
		default:
			type = "unknown";
			break;
		}

		printf("%-9s %-20s\n", type, ent->name);
        memset(ls_content_temp,0x00,sizeof(ls_content_temp));
        sprintf(ls_content_temp,"%-9s %-20s\n", type, ent->name);
        strcat(ls_content,ls_content_temp);

		if (ent->st.smb2_type == SMB2_TYPE_LINK) {
			char buf[256];

			if (url->path && url->path[0]) {
				asprintf(&link, "%s/%s", url->path, ent->name);
			} else {
				asprintf(&link, "%s", ent->name);
			}
			smb2_readlink(smb2, link, buf, 256);
			printf("	-> [%s]\n", buf);
			free(link);
		}
	}

	smb2_closedir(smb2, dir);
	smb2_disconnect_share(smb2);
	smb2_destroy_url(url);
	smb2_destroy_context(smb2);

    const char *return_str = ls_content;
    return return_str;
}