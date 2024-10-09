#include "../EspBaseFramework.h"

char WIFIFORM_page_1[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <script src="/vue"></script>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>WIFI Setup</title>
</head>
<body>
<header>
    <h1>WIFI Setup</h1>
    <p>Enter your Wifi credentials: </p>
</header>
<form id="wifi_config" method="post" action="/wificreds">
    <label for="ssid">SSID: </label>
    <select name="ssid">
)=====";
