# EspBaseFramework

GitHub : https://github.com/awaine93/espBaseFramework

Latest Release : 1.2.0
README / Docs version : V1.2.0

[Release Notes](https://www.notion.so/Release-Notes-11cf19cb7e91806b9e3aeee0a949afde?pvs=21)

The EspBaseFramework is a foundation that contains basic functionality for creating personal IoT-based projects. It's designed to be flashed to the ESP8266 micro-controller, though it could potentially be used on other micro-controllers. However, it has only been tested on ESP8266 devices.

## Functionality

- WiFi Access point mode  for non network connection.
- Captive portal for WiFi network connection
    - WiFi SSID & Password Authentication
- Over The Air updates - From IDE only at present (looking to expand this to a more user friendly method)
- WiFi Client mode for network connection
- MDNS host name assignment
- EEPROM storage

## Getting Started

To get started make sure you have [visual studio code](https://code.visualstudio.com/) installed and the [platformIO](https://platformio.org/) extension installed and enabled.

### Submodule

Navigate to your project’s root folder in a terminal :

```bash
cd /path/to/your/project
```

and clone the EspBaseFramework into a lib directory as a submodule using the following command:

```bash
git submodule add git@github.com:awaine93/espBaseFramework.git lib/EspBaseFramework
```

### PlatformIO

in your project’s root directory create a [platform.io](http://platform.io) file containing the following:

```bash
[env:wemos_d1_mini]
platform = espressif8266
board = d1_mini
framework = arduino
monitor_speed = 115200

upload_port = /dev/ttyUSB0
upload_speed = 115200

lib_extra_dirs = EspBaseFramework

lib_deps =
  ESP8266WiFi
  DNSServer
  EEPROM
  WiFiUdp
  ArduinoOTA
  ESP8266WebServer
  ArduinoJson
  ESP8266mDNS
  https://github.com/lacamera/ESPAsyncWebServer.git
  ESPAsyncTCP@1.2.4  
```

This file will setup your platformIO environment for the ESP tool chain and compiler, it also defines the upload and serial monitor baudrate and com port to use. It includes a `lib_extra_dirs` flag that includes the EspBaseFramework library, and a `lib_deps` that defines the external libraries to be used within the framework, you should add any libraries your project uses to this list.

PlatformIO requires that any project files need to be located in a src directory. Make sure your ino and any other files or project based directories, such as “/html_pages/” are located in a `src` directory

  

```bash
/path/to/your/project/src/<Project Files & Directories>
```

### Configuring The Project

To use the EspBaseFramework in your project you will need to include the EspBaseFramework and AsyncWebServer library header files.

```arduino
#include "EspBaseFramework.h"
#include <ESPAsyncWebServer.h>
```

If you are using a ESP32, you will need to define this as some libraries used in the framework are different.

```arduino
#define ESP32
```

Create an instance of a `AsyncWebServer` with your desired port, and this is then passed into the framework class as a parameter on initialization.

```arduino
AsyncWebServer server(80);
Framework framework(server);
```

You will need to define configuration that will be used within the framework within your project 

```arduino
const String DEVICE_NAME = "CHANGE-ME";
const String loggedInRoute = "/dashboard";
const int WIFI_CON_WAIT = 20;
```

- DEVICE_NAME - This is the name you want to give to your device, it will be the host-name of your device and the WiFi Access point SSID
- loggedInRoute - This is the route that you want your user to be redirected to after they login to the device, this route will also need to be handled within your application.
- WIFI_CON_WAIT - This is the timer in seconds the device waits for a WiFi Connection before starting the WiFi access point

Within the setup and loop functions of your project you need to call the Frameworks begin() and run() methods. 

```arduino
void setup() {
  framework.begin();
	// Your Code Here
}

void loop(){
	framework.run
	// Your code here
}
```

### Routes & Endpoints

As each project is different, you will have to define the route a user is directed too after a successful admin login. To define the route a user is redirected to, you will need to declare the `loggedInRoute` string and this will be used by the EspBaseFramework

```bash
const String loggedInRoute = "/dashboard";
```

At this point there will be nothing to handle this route and nothing to return to the user when they land on your route. You will need to register the route with the `framework._server` object, and any logic behind the route. You should define a route and its logic as part of or as any of the code that is called within the `setup()` method

```bash
void setup() {
  framework.begin();
  
  framework._server.on("/dashboard", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(200, "html", "<html>My Dashboard Page</html>");
  });
}
```

### HTML

To include a HTML page to use as your response, you can either declare a string with the HTML in the string

```bash
const String dashboardHtml = "<html>my html string</html>";
```

Another cleaner method to serve your HTML pages and modulating your code base, is to store HTML in a separate .h file, defined in progmem and include it within your projects ino file. First create a `Pages` directory in your projects `src` directory and create a `dashboard.h` file. 

```bash
/path/to/your/project/src/Pages/dashboard.h
```

In the `dashboard.h` file we will define our HTML string to be returned in progmem as a const char.
The variable name, in this example `DASHBOARD_page`, is what will be referenced to obtain the HTML string. It is a good practice to capitalize the file name and append _page as the variable name.

```bash
const char DASHBOARD_page[] PROGMEM = R"=====(
	<!DOCTYPE html>
	<html lang="en">
		YOUR HTML CODE HERE
	</html>
)=====";
```

The `dashboard.h` file should now be included in the projects using the relative path. We can now access the HTML using the defined variable set in the `dashboard.h` file.
In your project include the .h file and store the html string (pointer) 

```bash
#include "Pages/DashboardPage.h"

const String dashboardPage = FPSTR(DASHBOARD_page);
```

The project defined dashboardPage can now be used as a response on server requests

```bash
framework._server.on("/dashboard", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(200, "html", dashboardPage);
});
```

## Endpoints

### Non-connected / WiFi Access Point Mode

| Method | Endpoint | Description | Parameters |
| --- | --- | --- | --- |
| GET | /generate_204/captive.apple.com/fwlink | Captive portal endpoints. Returns WiFi Form page with pre-scanned SSID options. | None |
| POST | /wificreds | Stores submitted WiFi credentials in EEPROM and resets device to connect. | ssid: string
pass: string |

### Connected / WiFi Station Mode

| Method | Endpoint | Description | Parameters |
| --- | --- | --- | --- |
| GET | / | Returns admin password setup page or login page based on EEPROM state. | None |
| GET | /eeprom/clear/all | Factory reset: Clears all EEPROM and resets device. | None |
| GET | /eeprom/clear/adminpass | Clears admin password from EEPROM. | None |
| POST | /admin/set/pass | Sets admin configuration password in EEPROM. | pass: string
confirm: string |
| POST | /admin/login | Validates admin login password and redirects accordingly. | pass: string |

### Framework Class

Void begin() - This method does the setup for the framework such as getting stored data from EEPROM, initialising routes connecting to WiFi ect. 

Void run() - This method runs the handlers and  listener’s for the framework such as MDNS for the captive portal, OTA service and web server requests.

### Feature / function ideas

Automatic versioning - git hub workflow to update a version number in code when merge  to main??

OTA Check for updates - an endpoint that checks for updates from a sever address 

HTML Pages - research a efficient way to serve HTML pages 

Admin authentication - adding auth check on all page requests

404 - optional project defined 404 page, has framework default

EEPROM management - project public functions :

- Check available space - checks how much un used free space is left for the project (account for framework eeprom usage)
- Write - validation checks for available space against data length
- Read  - ability for project to read from eeprom (not read framework eeprom space)

Memory performance -

- Replace `String` usage with `char`
- See where pointers and references can be used
- Garbage collection ?
- Replace Arduino libraries with native functions?
