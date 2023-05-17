/**
 * Add gobals here
 */
var seconds = null;
var otaTimerVar = null;
var wifiConnectInterval = null;


/**
 * Initialize functions here.
 */
$(document).ready(function() {



    getUpdateStatus(); // JSON

    $("#connect_wifi").on("click", function() {
        checkCredentials();
    });
    $("#disconnect_wifi").on("click", function() {
        disconnectWifi();
    });

});


/**
 * Handles the firmware update.
 */
function updateFirmware() {
    // Form Data
    var formData = new FormData();
    var fileSelect = document.getElementById("selected_file");

    if (fileSelect.files && fileSelect.files.length == 1) {
        var file = fileSelect.files[0];
        formData.set("file", file, file.name);
        document.getElementById("ota_update_status").innerHTML = "Uploading " + file.name + ", Firmware Update in Progress...";

        // Http Request
        var request = new XMLHttpRequest();

        request.upload.addEventListener("progress", updateProgress);
        request.open('POST', "/OTAupdate");
        request.responseType = "blob";
        request.send(formData);
    } else {
        window.alert('Select A File First')
    }
}

/**
 * Progress on transfers from the server to the client (downloads).
 */
function updateProgress(oEvent) {
    if (oEvent.lengthComputable) {
        getUpdateStatus();
    } else {
        window.alert('total size is unknown')
    }
}

/**
 * Posts the firmware udpate status.
 */
function getUpdateStatus() {
    var xhr = new XMLHttpRequest();
    var requestURL = "/OTAstatus";
    xhr.open('POST', requestURL, false);
    xhr.send('ota_update_status');

    if (xhr.readyState == 4 && xhr.status == 200) {
        var response = JSON.parse(xhr.responseText); // Return JSON from string in server

        document.getElementById("latest_firmware").innerHTML = response.compile_date + " - " + response.compile_time

        // If flashing was complete it will return a 1, else -1
        // A return of 0 is just for information on the Latest Firmware request
        if (response.ota_update_status == 1) {
            // Set the countdown timer time
            seconds = 10;

            // Start the countdown timer
            otaTimerVar = setInterval(otaRebootTimer, 1000);
        } else if (response.ota_update_status == -1) {
            document.getElementById("ota_update_status").innerHTML = "!!! Upload Error !!!";
        }
    }
}

/**
 * Displays the reboot countdown.
 */
function otaRebootTimer() {
    --seconds;
    document.getElementById("ota_update_status").innerHTML = "OTA Firmware Update Complete. This page will close shortly, Rebooting in: " + seconds;

    if (seconds == 0) {
        clearTimeout(otaTimerVar);
        window.location.reload();
    }
}



/**
 * Clears the connection status interval.
 */
function stopWifiConnectStatusInterval() {
    if (wifiConnectInterval != null) {
        clearInterval(wifiConnectInterval);
        wifiConnectInterval = null;
    }
}

/**
 * Gets the WiFi connection status.
 */
function getWifiConnectStatus() {
    var xhr = new XMLHttpRequest();
    var requestURL = "/wifiConnectStatus";
    xhr.open('POST', requestURL, false);
    xhr.send('wifi_connect_status');

    if (xhr.readyState == 4 && xhr.status == 200) {
        var response = JSON.parse(xhr.responseText);

        document.getElementById("wifi_connect_status").innerHTML = "Connecting...";

        if (response.wifi_connect_status == 2) {
            document.getElementById("wifi_connect_status").innerHTML = "<h4 class='rd'>Failed to Connect. Please check your AP credentials and compatibility</h4>";
            stopWifiConnectStatusInterval();
        } else if (response.wifi_connect_status == 3) {
            document.getElementById("wifi_connect_status").innerHTML = "<h4 class='gr'>Connection Success!</h4>";
            stopWifiConnectStatusInterval();
            getConnectInfo();
        }
    }
}

/**
 * Starts the interval for checking the connection status.
 */
function startWifiConnectStatusInterval() {
    wifiConnectInterval = setInterval(getWifiConnectStatus, 2800);
}

/**
 * Connect WiFi function called using the SSID and password entered into the text fields.
 */
function connectWifi() {
    // Get the SSID and password
    selectedSSID = $("#connect_ssid").val();
    pwd = $("#connect_pass").val();

    $.ajax({
        url: '/wifiConnect.json',
        dataType: 'json',
        method: 'POST',
        cache: false,
        headers: { 'my-connect-ssid': selectedSSID, 'my-connect-pwd': pwd },
        data: { 'timestamp': Date.now() }
    });

    startWifiConnectStatusInterval();
}

/**
 * Checks credentials on connect_wifi button click.
 */
function checkCredentials() {
    errorList = "";
    credsOk = true;

    selectedSSID = $("#connect_ssid").val();
    pwd = $("#connect_pass").val();

    if (selectedSSID == "") {
        errorList += "<h4 class='rd'>SSID cannot be empty!</h4>";
        credsOk = false;
    }
    if (pwd == "") {
        errorList += "<h4 class='rd'>Password cannot be empty!</h4>";
        credsOk = false;
    }

    if (credsOk == false) {
        $("#wifi_connect_credentials_errors").html(errorList);
    } else {
        $("#wifi_connect_credentials_errors").html("");
        connectWifi();
    }
}

/**
 * Shows the WiFi password if the box is checked.
 */
function showPassword() {
    var x = document.getElementById("connect_pass");
    if (x.type === "password") {
        x.type = "text";
    } else {
        x.type = "password";
    }
}

/**
 * Disconnects Wifi once the disconnect button is pressed and reloads the web page.
 */
function disconnectWifi() {
    $.ajax({
        url: '/wifiDisconnect.json',
        dataType: 'json',
        method: 'DELETE',
        cache: false,
        data: { 'timestamp': Date.now() }
    });
    // Update the web page
    setTimeout("location.reload(true);", 2000);
}


document.getElementById("catForm").addEventListener("submit", function(event) {
    event.preventDefault(); // Prevent form submission

    // Get form data
    var form = document.getElementById("catForm");
    var formData = new FormData(form);

    // Get values of the fields
    var filepathValue = document.getElementById("filepath").value;
    var userValue = document.getElementById("username").value;
    var passValue = document.getElementById("password").value;
    var ipValue = document.getElementById("ip").value;
    var operationValue = document.getElementById("Operation").value;

    // Make to JSON object
    var data = {
        filepath: filepathValue,
        username: userValue,
        password: passValue,
        host: ipValue,
        operation: operationValue,
    };

    // Convert object to JSON
    var jsonData = JSON.stringify(data);

    // create and send request to server
    var xhr = new XMLHttpRequest();
    xhr.open("POST", "/smb-post"); // URL Server
    xhr.setRequestHeader("Content-Type", "application/json");
    xhr.onload = function() {
        if (xhr.status === 200) {
            // Request succeess
            console.log("POST request successful");
            console.log(xhr.responseText);
            document.getElementById("result").innerText = xhr.responseText;
        } else {
            // Request fail
            console.log("POST request failed");
            console.log("Error: " + xhr.status);
            document.getElementById("result").innerText = "Error: " + xhr.status;
        }
    };
    xhr.send(jsonData);
});