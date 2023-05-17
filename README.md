<!-- Improved compatibility of back to top link: See: https://github.com/othneildrew/Best-README-Template/pull/73 -->
<a name="readme-top"></a>
<!--
*** Thanks for checking out the Best-README-Template. If you have a suggestion
*** that would make this better, please fork the repo and create a pull request
*** or simply open an issue with the tag "enhancement".
*** Don't forget to give the project a star!
*** Thanks again! Now go create something AMAZING! :D
-->



<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->


<!-- PROJECT LOGO -->
<br />
<div align="center">
  <h3 align="center">SMB Client ESP32</h3>

  <p align="center">
    Server Message Block in ShareFolder!
    <br />
    <a href="https://github.com/othneildrew/Best-README-Template"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/othneildrew/Best-README-Template">View Demo</a>
    ·
    <a href="https://github.com/othneildrew/Best-README-Template/issues">Report Bug</a>
    ·
    <a href="https://github.com/othneildrew/Best-README-Template/issues">Request Feature</a>
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

[![Product Name Screen Shot][product-screenshot]](https://example.com)

This project demonstrates the use of the SMB protocol to extract information from a shared folder on the network using the Windows or Linux OS

The main functions of this demo are the use of OTA via SMB, performing "ls" and "cat" functions, and the possibility to extract a file from the share folder and place it in a SPIFFS partition inside the ESP32.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## How to Use Example

### Hardware Required

This example can be executed on any ESP32 board, the only required interface is WiFi and connection to internet or a local server.

### Build and Flash


This section the main frameworks and components used in the repository are listed.

* [ESP IDF V4.4.2](https://github.com/espressif/esp-idf)
* [SMB Component](https://github.com/nopnop2002/esp-idf-smb-client)

Build the project and flash it to the board, then run monitor tool to view serial output:

```
idf.py -p PORT flash monitor
```

(To exit the serial monitor, type ``Ctrl-]``.)

See the Getting Started Guide for full steps to configure and use ESP-IDF to build projects.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started

This demo shows how to use the SMB service to read, upload data to local memory and perform the OTA function via a shared folder.

### Prerequisites

Make sure you have an SMB server configured on any operating system.
Here is a [guide](https://support.microsoft.com/en-us/windows/file-sharing-over-a-network-in-windows-b58704b2-f53a-4b82-7bc1-80f9994725bf) for windwos 10.

<!-- USAGE EXAMPLES -->
## Usage

Through an STA enters the server's AP.

These are the default settings of the wifi network, they are located in the file main/APIs/WIFI_API/wifi_app.h line 18-32
```c
// WiFi application settings
#define WIFI_AP_SSID				"smb_api"			// AP name
#define WIFI_AP_PASSWORD			"123456789"			// AP password
#define WIFI_AP_CHANNEL				1					// AP channel
#define WIFI_AP_SSID_HIDDEN			0					// AP visibility
#define WIFI_AP_MAX_CONNECTIONS		5					// AP max clients
#define WIFI_AP_BEACON_INTERVAL		100					// AP beacon: 100 milliseconds recommended
#define WIFI_AP_IP					"192.168.5.1"		// AP default IP
#define WIFI_AP_GATEWAY				"192.168.5.1"		// AP default Gateway (should be the same as the IP)
#define WIFI_AP_NETMASK				"255.255.255.0"		// AP netmask
#define WIFI_AP_BANDWIDTH			WIFI_BW_HT20		// AP bandwidth 20 MHz (40 MHz is the other option)
#define WIFI_STA_POWER_SAVE			WIFI_PS_NONE		// Power save not used
#define MAX_SSID_LENGTH				32					// IEEE standard maximum
#define MAX_PASSWORD_LENGTH			64					// IEEE standard maximum
#define MAX_CONNECTION_RETRIES		5					// Retry number on disconnect

```

Once the code is running, enter the AP generated by the ESP, in order to access the end points via HTTP.

This demo has 3 html pages,

* **Index page:** this page provides the OTA functionality via HTTP and a form to connect the ESP to an STA.

```c
// register index.html handler
httpd_uri_t index_html = {
		.uri = "/",
		.method = HTTP_GET,
		.handler = http_server_index_html_handler,
		.user_ctx = NULL
};
httpd_register_uri_handler(http_server_handle, &index_html);
```

![index](/doc/img/index.png)

* **SMB Dashboard:** This page has a form for using the SMB API.

```c
// register smb.html handler
httpd_uri_t smb_html = {
    .uri = "/smb",
    .method = HTTP_GET,
    .handler = http_server_smb_html_handler,
    .user_ctx = NULL
};
httpd_register_uri_handler(http_server_handle, &smb_html);
```

![smb](/doc/img/smb.png)

* **File Server:** This page provides all the tools to manage the SPIFFS partition contained in the ESP

```c
// register wifiConnectInfo.json handler
httpd_uri_t spiffs_view = {
    .uri = "/spiffs/",
    .method = HTTP_GET,
    .handler = view_get_handler,
    .user_ctx = server_data
};
httpd_register_uri_handler(http_server_handle, &spiffs_view);
```

![spiffs](/doc/img/spiffs.png)

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- CONTACT -->
## Contact

Juan Sebastian Giraldo Duque - jsgiraldod@hotmail.com

Linkedin contact: [https://www.linkedin.com/in/juan-sebastian-giraldo-duque-25301718b/](https://www.linkedin.com/in/juan-sebastian-giraldo-duque-25301718b/)

Project Link: [https://github.com/jsebgiraldo/ESP32-SMB-API/tree/main](https://github.com/jsebgiraldo/ESP32-SMB-API/tree/main)

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/othneildrew/Best-README-Template.svg?style=for-the-badge
[contributors-url]: https://github.com/othneildrew/Best-README-Template/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/othneildrew/Best-README-Template.svg?style=for-the-badge
[forks-url]: https://github.com/othneildrew/Best-README-Template/network/members
[stars-shield]: https://img.shields.io/github/stars/othneildrew/Best-README-Template.svg?style=for-the-badge
[stars-url]: https://github.com/othneildrew/Best-README-Template/stargazers
[issues-shield]: https://img.shields.io/github/issues/othneildrew/Best-README-Template.svg?style=for-the-badge
[issues-url]: https://github.com/othneildrew/Best-README-Template/issues
[license-shield]: https://img.shields.io/github/license/othneildrew/Best-README-Template.svg?style=for-the-badge
[license-url]: https://github.com/othneildrew/Best-README-Template/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/othneildrew
[product-screenshot]: images/screenshot.png
[Next.js]: https://img.shields.io/badge/next.js-000000?style=for-the-badge&logo=nextdotjs&logoColor=white
[Next-url]: https://nextjs.org/
[React.js]: https://img.shields.io/badge/React-20232A?style=for-the-badge&logo=react&logoColor=61DAFB
[React-url]: https://reactjs.org/
[Vue.js]: https://img.shields.io/badge/Vue.js-35495E?style=for-the-badge&logo=vuedotjs&logoColor=4FC08D
[Vue-url]: https://vuejs.org/
[Angular.io]: https://img.shields.io/badge/Angular-DD0031?style=for-the-badge&logo=angular&logoColor=white
[Angular-url]: https://angular.io/
[Svelte.dev]: https://img.shields.io/badge/Svelte-4A4A55?style=for-the-badge&logo=svelte&logoColor=FF3E00
[Svelte-url]: https://svelte.dev/
[Laravel.com]: https://img.shields.io/badge/Laravel-FF2D20?style=for-the-badge&logo=laravel&logoColor=white
[Laravel-url]: https://laravel.com
[Bootstrap.com]: https://img.shields.io/badge/Bootstrap-563D7C?style=for-the-badge&logo=bootstrap&logoColor=white
[Bootstrap-url]: https://getbootstrap.com
[JQuery.com]: https://img.shields.io/badge/jQuery-0769AD?style=for-the-badge&logo=jquery&logoColor=white
[JQuery-url]: https://jquery.com 
