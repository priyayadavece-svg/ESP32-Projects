#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "ESP32_LED";      // WiFi name that will appear
const char* password = "12345678";   // Minimum 8 characters

WebServer server(80);

const int ledPin = 2;        // Change if needed
const int pwmFreq = 5000;
const int pwmResolution = 8; // 0–255

// HTML Webpage
String webpage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 LED Brightness</title>
</head>
<body style="text-align:center;">
  <h2>ESP32 LED Brightness Control</h2>
  <input type="range" min="0" max="255" value="0"
    oninput="updateSlider(this.value)">
  <p>Brightness: <span id="value">0</span></p>

<script>
function updateSlider(val) {
  document.getElementById('value').innerHTML = val;
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/set?value=" + val, true);
  xhr.send();
}
</script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", webpage);
}

void handleBrightness() {
  if (server.hasArg("value")) {
    int brightness = server.arg("value").toInt();
    ledcWrite(ledPin, brightness);
  }
  server.send(200, "text/plain", "OK");
}

void setup() {
  Serial.begin(115200);

  // Start ESP32 as Access Point
  WiFi.softAP(ssid, password);

  Serial.println();
  Serial.println("WiFi Created Successfully!");
  Serial.print("WiFi Name: ");
  Serial.println(ssid);
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  // PWM Setup (Core 3.x)
  ledcAttach(ledPin, pwmFreq, pwmResolution);

  server.on("/", handleRoot);
  server.on("/set", handleBrightness);

  server.begin();
}

void loop() {
  server.handleClient();
}
