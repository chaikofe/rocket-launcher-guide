/*
 * ============================================================
 *  ROCKET LAUNCH CONTROLLER — RECEIVER
 *  Board: Arduino Nano R4
 *  HC-12 on Serial1 (D0=RX, D1=TX)
 * ============================================================
 *
 *  Based on the proven working receiver:
 *    - Relays D4-D7, ACTIVE LOW (LOW = ON, HIGH = OFF)
 *    - Receives 4-char string like "1010"
 *    - Non-blocking timer turns relays off after interval
 *
 *  Added:
 *    - Continuity sensing on A1-A4 (one per channel)
 *    - Sends telemetry back to transmitter:
 *        TELEM,batt,temp,cont1,cont2,cont3,cont4
 *
 *  Continuity wiring per channel:
 *    5V -> 1kohm -> A(n) -> igniter -> GND
 *    Igniter intact  = current flows = A(n) reads LOW-ish  -> continuity OK
 *    Igniter broken  = no path       = A(n) reads HIGH-ish -> no loop
 *    (adjust CONT_THRESHOLD to your circuit)
 * ============================================================
 */

// ── Relay pins (active LOW) ────────────────────────────
const int pins[4] = {4, 5, 6, 7};

// ── Continuity sense pins ──────────────────────────────
const int contPins[4] = {A1, A2, A3, A4};

// Ground reference (A5 as GND, low current only)
const int PIN_GND = A5;

// Continuity threshold — tune to your resistor divider
// Reading below threshold = igniter connected (loop closed)
const int CONT_THRESHOLD = 512;   // midpoint of 0-1023

// ── Timing ─────────────────────────────────────────────
unsigned long previousMillis = 0;
const long interval = 5000;       // relays on for 5 seconds
bool pinsActive = false;

// ── Setup ──────────────────────────────────────────────
void setup() {
  Serial.begin(9600);      // USB debug
  Serial1.begin(9600);     // HC-12 on D0/D1

  // Relays — active low, HIGH = OFF (safe boot)
  for (int i = 0; i < 4; i++) {
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], HIGH);
  }

  // Continuity inputs
  for (int i = 0; i < 4; i++) pinMode(contPins[i], INPUT);

  // A5 as ground reference
  pinMode(PIN_GND, OUTPUT);
  digitalWrite(PIN_GND, LOW);

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.println("Receiver ready (R4) — continuity + telemetry");
}

// ── Read continuity for one channel ────────────────────
bool readContinuity(int ch) {
  int val = analogRead(contPins[ch]);
  // Below threshold = loop closed = igniter connected
  return (val < CONT_THRESHOLD);
}

// ── Read receiver battery voltage ──────────────────────
// Placeholder — wire a voltage divider to an analog pin for real reading.
// For now returns a simulated value. Replace A0 read with your divider.
float readBattery() {
  // Example if you wire 12V through divider to A0:
  //   float v = analogRead(A0) * (5.0/1023.0) * dividerRatio;
  // For now, simulate:
  return 7.8;
}

// ── Read temperature ───────────────────────────────────
// Placeholder — add a temp sensor or use internal if available.
int readTemp() {
  return 22;
}

// ── Send telemetry ─────────────────────────────────────
void sendTelemetry() {
  float batt = readBattery();
  int temp   = readTemp();

  String telem = "TELEM," + String(batt, 1) + "," + String(temp);
  for (int i = 0; i < 4; i++) {
    telem += "," + String(readContinuity(i) ? 1 : 0);
  }
  Serial1.println(telem);
  Serial.println("Sent: " + telem);
}

// ── Main loop ──────────────────────────────────────────
void loop() {

  // 1. Check for incoming fire command
  if (Serial1.available() > 0) {
    String msg = Serial1.readStringUntil('\n');
    msg.trim();

    if (msg.length() >= 4) {
      for (int i = 0; i < 4; i++) {
        char c = msg.charAt(i);
        // Active low: '1' = ON (LOW), '0' = OFF (HIGH)
        if (c == '1') digitalWrite(pins[i], LOW);
        else          digitalWrite(pins[i], HIGH);
      }

      // Flash onboard LED
      digitalWrite(LED_BUILTIN, HIGH);
      delay(20);
      digitalWrite(LED_BUILTIN, LOW);

      previousMillis = millis();
      pinsActive = true;

      Serial.println("Fired: " + msg);
    }
  }

  // 2. Turn relays off after interval (non-blocking)
  if (pinsActive && (millis() - previousMillis >= interval)) {
    pinsActive = false;
    for (int i = 0; i < 4; i++) digitalWrite(pins[i], HIGH);  // OFF
    Serial.println("Relays off");
  }

  // 3. Send telemetry every second
  static unsigned long lastTelem = 0;
  if (millis() - lastTelem > 1000) {
    sendTelemetry();
    lastTelem = millis();
  }
}
