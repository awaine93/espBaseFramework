#include "../EspBaseFramework.h"

char DASHBOARD_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Dashboard</title>
</head>
<body>
<header>
    <h1>Dashboard</h1>
</header>

<div id="dashboard">
    
</div>

</body>

<script></script>

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

    #dashboard {
        width: 90%;
        max-width: 400px;
        display: flex;
        margin: 0 auto;
        justify-content: center;
        flex-wrap: wrap;
    }

    #dashboard input {
        width: 100%;
        padding: .7rem;
        margin: .5rem 0 1rem;
    }

    #dashboard input[type="submit"] {
        background: #000;
        color: #fff;
        border: none;
        padding: 1rem;
        margin-top: 1.5rem;
        font-size: 1rem;
        font-weight: 900;
    }

    #dashboard label {
        width: 100%;
        font-family: arial, "helvetica";
    }

    #dashboard select {
        width: 100%;
        padding: .7rem;
        margin: .5rem 0 1rem;
        border: solid 1px #8e8e8e;
        background: #fff;
    }
</style>
</html>
)=====";
