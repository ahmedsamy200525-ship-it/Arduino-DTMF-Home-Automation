/*
 * =========================================================================
 * Project: Universal GSM Automation System
 * Developer: Ahmed Samy
 * -------------------------------------------------------------------------
 * SECURITY SETTINGS (AUTHORIZED NUMBER):
 * - If authorizedNumber is set to "", the system will answer ANY call 
 * (Useful for testing and demos).
 * - To enable security, enter your phone number in international format 
 * (e.g., "+201XXXXXXXXX").
 * =========================================================================
 */

#include <SoftwareSerial.h>
#include <avr/wdt.h> 

// Hardware Configuration
#define GSM_TX 9
#define GSM_RX 10
#define RELAY1 5
#define RELAY2 7
#define BTN1 A0
#define BTN2 A1

SoftwareSerial sim800l(GSM_TX, GSM_RX);

// --- USER CONFIGURATION ---
String authorizedNumber = ""; // Leave empty for "Public Mode" or add your number for "Secure Mode"
String pinCode = "1234";      // Change your security PIN here

// System Flags
String inputBuffer = "";
bool isAuthenticated = false;
bool inCall = false;
int light1 = 0, light2 = 0;
int lastState1 = HIGH, lastState2 = HIGH;

void setup() {
  wdt_enable(WDTO_8S); 

  pinMode(RELAY1, OUTPUT); digitalWrite(RELAY1, HIGH);
  pinMode(RELAY2, OUTPUT); digitalWrite(RELAY2, HIGH);
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);

  Serial.begin(9600);
  sim800l.begin(9600);

  Serial.println("System Initializing...");
  
  // Initialization Commands
  sim800l.println("AT"); delay(200);
  sim800l.println("AT+CLIP=1"); delay(200); 
  sim800l.println("AT+DDET=1"); delay(200); 
  sim800l.println("AT+CLVL=100"); delay(200); 
  
  Serial.println("System Ready.");
}

void loop() {
  wdt_reset();
  
  checkPhysicalButtons(); // Manual control always active

  if (sim800l.available()) {
    String data = sim800l.readString();
    Serial.println("GSM Log: " + data);

    // 1. Incoming Call Logic
    if (data.indexOf("RING") != -1) {
      // Check if Caller is Authorized or if Public Mode is active
      if (authorizedNumber == "" || data.indexOf(authorizedNumber) != -1) {
        Serial.println("Authorized Access. Answering...");
        sim800l.println("ATA");
        delay(1000);
        sim800l.println("AT+CMUT=1"); // Mute microphone for clear DTMF detection
        inCall = true;
        
        // 2. Visual Confirmation (Blink Signal)
        confirmSignal(); 
        
      } else {
        Serial.println("Unauthorized Caller. Rejected.");
        sim800l.println("ATH"); 
      }
    }

    // 3. DTMF Decoding Logic
    if (inCall && data.indexOf("+DTMF:") != -1) {
      int index = data.indexOf("+DTMF:");
      char key = data.charAt(index + 7);
      processCommand(key);
    }

    // 4. Call Disconnection
    if (data.indexOf("NO CARRIER") != -1) {
      terminateCall();
    }
  }

  // Monitor Control (Via Serial)
  if (inCall && Serial.available()) {
    processCommand(Serial.read());
  }
}

// Confirmation blink when call starts
void confirmSignal() {
  digitalWrite(RELAY1, LOW); digitalWrite(RELAY2, LOW);
  delay(800);
  digitalWrite(RELAY1, HIGH); digitalWrite(RELAY2, HIGH);
  Serial.println("Signal Sent: Line Open.");
}

void processCommand(char key) {
  if (!isAuthenticated) {
    inputBuffer += key;
    if (inputBuffer == pinCode) {
      isAuthenticated = true;
      inputBuffer = "";
      Serial.println("PIN Verified. Control Unlocked.");
      // Lights turn ON as feedback for correct PIN
      light1 = 1; light2 = 1; updateHardware();
    } else if (inputBuffer.length() >= 4) {
      inputBuffer = ""; 
      Serial.println("Error: Incorrect PIN.");
    }
  } else {
    // Commands active after authentication
    switch (key) {
      case '1': light1 = !light1; break;
      case '2': light2 = !light2; break;
      case '9': light1 = 1; light2 = 1; break;
      case '0': light1 = 0; light2 = 0; break;
      case '*': sim800l.println("ATH"); break; 
    }
    updateHardware();
  }
}

void checkPhysicalButtons() {
  int s1 = digitalRead(BTN1);
  if (s1 == LOW && lastState1 == HIGH) { light1 = !light1; updateHardware(); }
  lastState1 = s1;

  int s2 = digitalRead(BTN2);
  if (s2 == LOW && lastState2 == HIGH) { light2 = !light2; updateHardware(); }
  lastState2 = s2;
  delay(20);
}

void updateHardware() {
  digitalWrite(RELAY1, light1 ? LOW : HIGH);
  digitalWrite(RELAY2, light2 ? LOW : HIGH);
}

void terminateCall() {
  inCall = false;
  isAuthenticated = false;
  light1 = 0; light2 = 0; // Safe reset: Turn off devices
  updateHardware();
  Serial.println("System Reset: Awaiting next call.");
}