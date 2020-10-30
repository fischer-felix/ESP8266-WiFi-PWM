const char indexHTML[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
.slidecontainer {
  width: 100%;
  margin: 10px;
}

.slider {
  -webkit-appearance: none;
  width: 80%;
  height: 25px;
  background: #d3d3d3;
  outline: none;
  opacity: 0.7;
  -webkit-transition: .2s;
  transition: opacity .2s;
}

.slider:hover {
  opacity: 1;
}

.slider::-webkit-slider-thumb {
  -webkit-appearance: none;
  appearance: none;
  width: 25px;
  height: 25px;
  background: #4CAF50;
  cursor: pointer;
}

.slider::-moz-range-thumb {
  width: 25px;
  height: 25px;
  background: #4CAF50;
  cursor: pointer;
}
</style>
</head>
<body>


<div class="slidecontainer">
  <input type="range" min="0" max="100" value="50" class="slider" id="slider1">
  <span id="value1"></span>
</div>

<div class="slidecontainer">
  <input type="range" min="0" max="100" value="50" class="slider" id="slider2">
  <span id="value2"></span>
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
