
file(GLOB srcs
        "*.c"
        "APIs/HTTP_SERVER/*.c"
        "APIs/WIFI_API/*.c"
        "APIs/SPIFFS/*c"
        "APIs/SMB/*.c"
        "APIs/NVS/*.c"
        )

set(dirs
        "APIs/HTTP_SERVER"
        "APIs/WIFI_API"
        "APIs/SPIFFS"
        "APIs/SMB"
        "APIs/NVS"
        )

set(files
        "../doc/web/style.css"
        "../doc/web/app.js"
        "../doc/web/favicon.ico"
        "../doc/web/index.html"
        "../doc/web/upload_script.html"
        "../doc/web/jquery-3.3.1.min.js"
)

idf_component_register(
        SRCS ${srcs}
        INCLUDE_DIRS ${dirs}
        EMBED_FILES ${files}
        )