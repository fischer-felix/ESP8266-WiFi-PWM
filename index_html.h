const char indexHTML[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
html {
 /* height: 100%;*/
  font-family: 'Work Sans', sans-serif;
  text-align: center;
}

html h1 {
  color: #eeeeee;
  font-weight: 5000;
  margin-bottom: 35px;
  /*text-align: left;*/
}

html h3 {
  color: #c3c3c3;
  font-weight: 700;
  margin-left: 14%;
  margin-right: 14%;
  /*display: inline;*/
}

html h4 {
  color: #999;
  font-weight: 500;
}

/*Chrome*/
@media screen and (-webkit-min-device-pixel-ratio:0) {
    input[type='range'] {
      overflow: hidden;
      width: 75%;
      border-radius: 14px;
      outline: none;
      -webkit-appearance: none;
      background-color: #1b3756;
    }
    
    input[type='range']::-webkit-slider-runnable-track {
      height: 40px;
      -webkit-appearance: none;
      color: #052aff;
    }
    
    input[type='range']::-webkit-slider-thumb {
      width: 0px;
      -webkit-appearance: none;
      height: 40px;
      border-radius: 50%;
      cursor: ew-resize;
      background: #1a73e8;
      box-shadow: -800px 0 0 800px #1a73e8;
    }

}
/** FF*/
input[type=range]{
    /* fix for FF unable to apply focus style bug  */
    border: 0px #242424; 

    /*required for proper track sizing in FF*/
    width: 75%;
    height: 40px;
}
input[type=range]::-moz-range-thumb {
  background-color: #1a73e8;
  height: 0px;
  width: 0px;
  border: none;
}
input[type="range"]::-moz-range-progress {
  background-color: #1a73e8; 
  height: 40px;
}
input[type="range"]::-moz-range-track {  
  background-color: #1b3756;
  border: none;
  height: 40px;
  width: 75%;
  border-radius: 14px;
}

</style>
</head>
<body style="background-color: #242424">

<h1> Lamp Control </h1>

<div style="clear: both">
<h3 style="float: left"> TOP </h3> 
<h3 style="float: right"> <span id="value1"></span> </h3>
</div>

<div class="slidecontainer">
  <input type="range" min="0" max="100" value="50" class="slider" id="slider1">
</div>

<br>

<div style="clear: both">
<h3 style="float: left"> BOTTOM </h3>
<h3 style="float: right"> <span id="value2"></span> </h3>
</div>

<div class="slidecontainer">
  <input type="range" min="0" max="100" value="50" class="slider" id="slider2">
</div>


<script>
var xhr = new XMLHttpRequest();
var slider1 = document.getElementById("slider1");
var slider2 = document.getElementById("slider2");
var value1 = document.getElementById("value1");
var value2 = document.getElementById("value2");
value1.innerHTML = slider1.value;
value2.innerHTML = slider2.value;
var url = "/";     
window.onload = init;

slider1.oninput = function() {
	value1.innerHTML = this.value;
	xhr.open("GET", url + "?ch1=" + this.value, true);
	xhr.send();
}
slider2.oninput = function() {
	value2.innerHTML = this.value;
	xhr.open("GET", url + "?ch2=" + this.value, true);
	xhr.send();
}

function init() {
	xhr = new XMLHttpRequest();
	xhr.onreadystatechange = processResponse;
	setInterval(update, 1000);
	update();
}

function update() {
	xhr.open("GET", url+"?dummy=x", true);
	xhr.send();
}

function processResponse() {
	if (4 == xhr.readyState && 200 == xhr.status) {
		var response = JSON.parse(xhr.responseText);
		value1.innerHTML = response.ch1;
		slider1.value = response.ch1;
		value2.innerHTML = response.ch2;
		slider2.value = response.ch2;
	}
}

</script>

</body>
</html>
)=====" ;
