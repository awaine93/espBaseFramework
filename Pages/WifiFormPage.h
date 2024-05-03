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
char WIFIFORM_page_2[] PROGMEM = R"=====(
    </select>
    <label for="pass">Password: </label>
    <input type="password" placeholder="Password" name="pass"/>
    <input type="submit"  style="margin-top: 20px" value="Sumbit"/>
</form>

</body>
<style>
    header {
        width: 100%;
        max-width: 400px;
        margin: 0 auto;
        display: flex;
        justify-content: center;
        align-items: center;
        text-align: center;
        flex-wrap: wrap;
        margin-bottom: 1rem;
    }
    header h1 {
        font-size: 2rem;
        font-weight: bolder;
        margin-bottom: 0;
    }
    header h1, header p {
        width: 100%;
        text-align: center;
        font-family: arial, "helvetica";
    }
    #wifi_config{
        width: 90%;
        max-width: 400px;
        display: flex;
        margin: 0 auto;
        justify-content: center;
        flex-wrap: wrap;
    }
    #wifi_config input{
        width: 100%;
        padding: .7rem;
        margin: .5rem 0 1rem;
    }
    #wifi_config input[type="submit"]{
        background: #000;
        color: #fff;
        border: none;
        padding: 1rem;
        margin-top: 1.5rem;
        font-size: 1rem;
        font-weight: 900;
    }
    #wifi_config label {
        width: 100%;
        font-family: arial, "helvetica";
    }

    #wifi_config select {
        width: 100%;
        padding: .7rem;
        margin: .5rem 0 1rem;
        border: solid 1px #8e8e8e;
        background: #fff;
    }
</style>
</html>
)=====";
