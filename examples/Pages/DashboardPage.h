const char DASHBOARD_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <script src="https://unpkg.com/axios/dist/axios.min.js"></script>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>EspBaseFirmware</title>
    
        <script>
        
        function setSpeed(){
        	console.log("holla");
        	let speed = document.getElementById("speed").value;
        	let cw = document.getElementById("cw").checked;
        	console.log(cw);
       	console.log(speed);
       	
       	
       	let formData = new FormData();
		formData.append("speed", speed);
		formData.append("cw", cw);

       	
  		axios({method: 'post',  url: '/motor1/speed',  data: formData,})
  		.then(function (response) {
    			//handle success
    			console.log(response);
  		})
  		.catch(function (response) {
    			//handle error
    			console.log(response);
  		});
        }
        </script>
    
</head>
<body>
<header>
    <h1>Dashbaord</h1>
</header>
	<div id="dashboard">
		<h1>EspBaseFramework</h1>
	</div>
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
    #dashbaord{
        width: 90%;
        max-width: 400px;
        display: flex;
        margin: 0 auto;
        justify-content: center;
        flex-wrap: wrap;
    }
    #dashbaord input{
        width: 100%;
        padding: .7rem;
        margin: .5rem 0 1rem;
    }
    #dashbaord input[type="submit"]{
        background: #000;
        color: #fff;
        border: none;
        padding: 1rem;
        margin-top: 1.5rem;
        font-size: 1rem;
        font-weight: 900;
    }
    #dashbaord label {
        width: 100%;
        font-family: arial, "helvetica";
    }

    #dashbaord select {
        width: 100%;
        padding: .7rem;
        margin: .5rem 0 1rem;
        border: solid 1px #8e8e8e;
        background: #fff;
    }
</style>
</html>
)=====";
