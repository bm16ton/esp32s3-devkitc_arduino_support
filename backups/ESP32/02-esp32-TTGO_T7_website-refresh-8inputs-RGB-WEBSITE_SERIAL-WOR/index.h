const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<style>
.card{
    max-width: 400px;
     min-height: 250px;
     background: #02b875;
     padding: 30px;
     box-sizing: border-box;
     color: #FFF;
     margin:20px;
     box-shadow: 0px 2px 18px -4px rgba(0,0,0,0.75);
}
</style>
<body>

<div class="card">
  <h4>The ESP32S3 ANALOG IN</h4><br>
  <h1>A0 Value: <span id="ADCValue">0</span>V</h1><br>
  <h1>A17 Value: <span id="ADCValue2">0</span>V</h1><br>
  <h1>A18 Value: <span id="ADCValue3">0</span>V</h1><br>
  <h1>A19 Value: <span id="ADCValue4">0</span>V</h1><br>
  <h1>A15 Value: <span id="ADCValue5">0</span>V</h1><br>
  <h1>A5 Value: <span id="ADCValue6">0</span>V</h1><br>
  <h1>A6 Value: <span id="ADCValue7">0</span>V</h1><br>
  <h1>A7 Value: <span id="ADCValue8">0</span>V</h1><br>
</div>
<script>

setInterval(function() {
  // Call a function repetatively with 2 Second interval
  getData();
}, 100); //2000mSeconds update rate

function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("ADCValue").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "readADC", true);
  xhttp.send();

// analog 2

  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("ADCValue2").innerHTML =
      this.responseText;
    }
  };
   xhttp.open("GET", "readADC2", true);
   xhttp.send();

// analog 3

  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("ADCValue3").innerHTML =
      this.responseText;
    }
  };
   xhttp.open("GET", "readADC3", true);
   xhttp.send();

// analog 3

  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("ADCValue4").innerHTML =
      this.responseText;
    }
  };
   xhttp.open("GET", "readADC4", true);
   xhttp.send();

// analog 3

  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("ADCValue5").innerHTML =
      this.responseText;
    }
  };
   xhttp.open("GET", "readADC5", true);
   xhttp.send();

// analog 3

  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("ADCValue6").innerHTML =
      this.responseText;
    }
  };
   xhttp.open("GET", "readADC6", true);
   xhttp.send();

// analog 3

  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {

    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("ADCValue7").innerHTML =
      this.responseText;
   }
  };
   xhttp.open("GET", "readADC7", true);
   xhttp.send();

 // analog 3

  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("ADCValue8").innerHTML =
      this.responseText;
    }
  };
   xhttp.open("GET", "readADC8", true);
   xhttp.send();
}
</script>
</body>
</html>
)=====";
