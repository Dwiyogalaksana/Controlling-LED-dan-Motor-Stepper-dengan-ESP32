#include <WiFi.h>
#include <WebServer.h>

// Definisi SSID dan password jaringan WiFi
const char* ssid = "AndroidAP21ac";
const char* password = "123456799";

WebServer server(80);

// Definisi pin LED
const int PinLed[5] = {25, 33, 32, 14, 26};

// Definisi pin motor stepper
const int PinStepper[4] = {18, 19, 22, 23}; 
int stepperSteps = 2048; // jenis motor stepper

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  for (int i = 0; i < 5; i++) {
    pinMode(PinLed[i], OUTPUT);
    digitalWrite(PinLed[i], LOW);
  }

  for (int i = 0; i < 4; i++) {
    pinMode(PinStepper[i], OUTPUT);
    digitalWrite(PinStepper[i], LOW);
  }

  server.on("/", handleRoot);
  server.on("/Led_Menyala", handleLedMenyala);
  server.on("/Stepper_Bergerak", handleStepperMenyala);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String html = "<!DOCTYPE html><html><head><title>Controlling LED dan Motor Stepper</title>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; text-align: center; padding: 50px; background-color: white; }";
  html += "h1 { color: black; }";
  html += ".ledButton { background-color: red; color: white; padding: 15px 20px; border: none; border-radius: 5px; font-size: 16px; cursor: pointer; margin: 50px; }";
  html += ".stepperButton { background-color: blue; color: white; padding: 15px 20px; border: none; border-radius: 5px; font-size: 16px; cursor: pointer; margin: 50px; }";
  html += "button:disabled { background-color: grey; cursor: not-allowed; }";
  html += ".ledButton:hover:enabled { background-color: darkred; }";
  html += ".stepperButton:hover:enabled { background-color: darkblue; }";
  html += "#status { margin-top: 20px; font-size: 18px; color: grey; }";
  html += "</style></head><body>";
  html += "<h1>Controlling LED dan Motor Stepper dengan ESP32</h1>";
  html += "<button id='blinkButton' class='ledButton' onclick='sendCommand()'>LED</button>";
  html += "<button id='moveButton' class='stepperButton' onclick='moveStepper()'>Motor Stepper</button>";
  html += "<p id='status'></p>";
  html += "<p>Dibuat oleh Dwi Yoga Laksana</p>";
  html += "<script>";
  html += "function sendCommand() {";
  html += "document.getElementById('status').innerText = 'Mengirim perintah ke LED...';";
  html += "document.getElementById('blinkButton').disabled = true;";
  html += "fetch('/Led_Menyala').then(response => {";
  html += "if (!response.ok) { throw new Error('Network response was not ok'); }";
  html += "document.getElementById('status').innerText = 'LED menyala!';";
  html += "setTimeout(() => { window.location.replace('/'); }, 1000);"; // Redirect after 1 second
  html += "}).catch(error => {";
  html += "document.getElementById('status').innerText = 'Terjadi kesalahan!';";
  html += "document.getElementById('blinkButton').disabled = false;";
  html += "console.error('Error:', error);";
  html += "});";
  html += "}";
  html += "function moveStepper() {";
  html += "document.getElementById('status').innerText = 'Mengirim perintah ke motor stepper...';";
  html += "document.getElementById('moveButton').disabled = true;";
  html += "fetch('/Stepper_Bergerak').then(response => {";
  html += "if (!response.ok) { throw new Error('Network response was not ok'); }";
  html += "document.getElementById('status').innerText = 'Motor stepper bergerak!';";
  html += "setTimeout(() => { window.location.replace('/'); }, 1000);"; // Redirect after 1 second
  html += "}).catch(error => {";
  html += "document.getElementById('status').innerText = 'Terjadi kesalahan!';";
  html += "document.getElementById('moveButton').disabled = false;";
  html += "console.error('Error:', error);";
  html += "});";
  html += "}";
  html += "</script></body></html>";
  server.send(200, "text/html", html);
}

void handleLedMenyala() {
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 5; j++) {
      digitalWrite(PinLed[j], HIGH);
      delay(500);
      digitalWrite(PinLed[j], LOW);
      delay(500);
    }
  }
  server.send(200, "text/plain", "Done"); // Mengirim respons singkat
}

void handleStepperMenyala() {
  moveStepperClockwise();
  delay(1000);
  moveStepperCounterClockwise();
  server.send(200, "text/plain", "Done"); // Mengirim respons singkat
}

// Fungsi untuk menggerakkan motor stepper searah jarum jam
void moveStepperClockwise() {
  for (int i = 0; i < stepperSteps; i++) {
    stepperStep(i % 4);
    delay(2);
  }
}

// Fungsi untuk menggerakkan motor stepper berlawanan arah jarum jam
void moveStepperCounterClockwise() {
  for (int i = stepperSteps; i > 0; i--) {
    stepperStep(i % 4);
    delay(2);
  }
}

// Fungsi untuk menentukan langkah motor stepper
void stepperStep(int step) {
  switch (step) {
    case 0:
      digitalWrite(PinStepper[0], HIGH);
      digitalWrite(PinStepper[1], LOW);
      digitalWrite(PinStepper[2], LOW);
      digitalWrite(PinStepper[3], LOW);
      break;
    case 1:
      digitalWrite(PinStepper[0], LOW);
      digitalWrite(PinStepper[1], HIGH);
      digitalWrite(PinStepper[2], LOW);
      digitalWrite(PinStepper[3], LOW);
      break;
    case 2:
      digitalWrite(PinStepper[0], LOW);
      digitalWrite(PinStepper[1], LOW);
      digitalWrite(PinStepper[2], HIGH);
      digitalWrite(PinStepper[3], LOW);
      break;
    case 3:
      digitalWrite(PinStepper[0], LOW);
      digitalWrite(PinStepper[1], LOW);
      digitalWrite(PinStepper[2], LOW);
      digitalWrite(PinStepper[3], HIGH);
      break;
  }
}
