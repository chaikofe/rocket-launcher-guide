/*
 * ============================================================
 *  ROCKET LAUNCH CONTROLLER — TRANSMITTER
 *  Board: Arduino GIGA R1 + GIGA Display Shield
 *  HC-12 on Serial2 (D18=TX2, D19=RX2)
 * ============================================================
 *
 *  Based on the proven working transmitter:
 *    - Switches D2-D5 (pad arm select)
 *    - Trigger button D6 (fire)
 *    - Sends 4-char string like "1010" on button press
 *
 *  Added:
 *    - GIGA Display Shield UI (4 channel dashboard)
 *    - Reads telemetry back from receiver:
 *        TELEM,batt,temp,cont1,cont2,cont3,cont4
 *
 *  Libraries needed (install via Library Manager):
 *    - Arduino_GigaDisplay_GFX
 * ============================================================
 */

#include "Arduino_GigaDisplay_GFX.h"

GigaDisplay_GFX display;

// ── Colors (RGB565) ────────────────────────────────────
#define BLACK   0x0000
#define WHITE   0xFFFF
#define RED     0xF800
#define GREEN   0x07E0
#define YELLOW  0xFFE0
#define ORANGE  0xFC00
#define BLUE    0x2C7F
#define DKBLUE  0x18E3
#define TILEBG  0x2185
#define GRAY    0x8410
#define LTGRAY  0xC618

// ── Radio / switch pins (from working code) ────────────
const int switchPins[4] = {2, 3, 4, 5};   // pad arm switches
const int triggerPin    = 6;              // fire button
bool lastTriggerState   = HIGH;

// ── State ──────────────────────────────────────────────
bool padArmed[4]   = {false, false, false, false};  // local switch state
bool padCont[4]    = {false, false, false, false};  // continuity from receiver
bool overallArmed  = false;

// Telemetry values from receiver
float remoteBatt   = 0.0;
int   remoteTemp   = 0;
int   rfRssi       = -99;
unsigned long lastPacketMs = 0;
bool  linkAlive    = false;

// Controller's own battery (optional — read from analog if wired)
float localBatt    = 12.4;

// ── Screen layout constants ────────────────────────────
const int SCREEN_W = 800;
const int SCREEN_H = 480;

// ── Setup ──────────────────────────────────────────────
void setup() {
  Serial.begin(115200);    // USB debug
  Serial2.begin(9600);     // HC-12 on D18/D19

  for (int i = 0; i < 4; i++) pinMode(switchPins[i], INPUT_PULLUP);
  pinMode(triggerPin, INPUT_PULLUP);

  display.begin();
  display.setRotation(1);          // landscape
  display.fillScreen(BLACK);

  Serial.println("Transmitter + display ready");
  drawScreen();
}

// ── Send fire command (from working code) ──────────────
void sendFire() {
  String message = "";
  for (int i = 0; i < 4; i++) {
    message += (digitalRead(switchPins[i]) == LOW) ? "1" : "0";
  }
  Serial2.println(message);
  Serial.println("GIGA sent: " + message);
}

// ── Read local switch states ───────────────────────────
void readSwitches() {
  overallArmed = false;
  for (int i = 0; i < 4; i++) {
    padArmed[i] = (digitalRead(switchPins[i]) == LOW);
    if (padArmed[i]) overallArmed = true;
  }
}

// ── Parse telemetry from receiver ──────────────────────
// Format: TELEM,batt,temp,cont1,cont2,cont3,cont4
void parseTelemetry(String t) {
  if (!t.startsWith("TELEM,")) return;

  int idx = 6; // after "TELEM,"
  String fields[6];
  for (int i = 0; i < 6; i++) {
    int comma = t.indexOf(',', idx);
    if (comma == -1) { fields[i] = t.substring(idx); break; }
    fields[i] = t.substring(idx, comma);
    idx = comma + 1;
  }

  remoteBatt = fields[0].toFloat();
  remoteTemp = fields[1].toInt();
  for (int i = 0; i < 4; i++) {
    padCont[i] = (fields[2 + i].toInt() == 1);
  }

  lastPacketMs = millis();
  linkAlive = true;
}

// ── Draw the full screen ───────────────────────────────
void drawScreen() {
  display.fillScreen(BLACK);

  // ─ Top bar ─
  // Local battery (left)
  display.fillRoundRect(20, 15, 40, 22, 4, GREEN);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(70, 18);
  display.print(localBatt, 1);
  display.print("V");
  display.setTextColor(GRAY);
  display.setTextSize(1);
  display.setCursor(160, 24);
  display.print("CHARGED");

  // ARMED / SAFE (center)
  if (overallArmed) {
    display.fillRoundRect(320, 12, 160, 30, 6, ORANGE);
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(360, 19);
    display.print("ARMED");
  } else {
    display.drawRoundRect(320, 12, 160, 30, 6, GREEN);
    display.setTextColor(GREEN);
    display.setTextSize(2);
    display.setCursor(365, 19);
    display.print("SAFE");
  }

  // RF link (right)
  display.setTextColor(GRAY);
  display.setTextSize(1);
  display.setCursor(600, 18);
  display.print("RF LINK");
  display.fillRoundRect(660, 15, 40, 18, 3, linkAlive ? GREEN : RED);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(710, 16);
  display.print(rfRssi);
  display.setTextSize(1);
  display.setCursor(760, 22);
  display.print("dBm");

  // ─ 4 Channel tiles ─
  drawChannelTile(0, 20,  60,  370, 175);  // Ch1 top-left
  drawChannelTile(1, 410, 60,  370, 175);  // Ch2 top-right
  drawChannelTile(2, 20,  245, 370, 175);  // Ch3 bottom-left
  drawChannelTile(3, 410, 245, 370, 175);  // Ch4 bottom-right

  // ─ Bottom bar ─
  display.setTextColor(GRAY);
  display.setTextSize(1);
  display.setCursor(20, 445);
  display.print("REMOTE BATT");
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(115, 440);
  display.print(remoteBatt, 1);
  display.print("V");

  display.setTextColor(GRAY);
  display.setTextSize(1);
  display.setCursor(320, 445);
  display.print("REMOTE TEMP");
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(415, 440);
  display.print(remoteTemp);
  display.print("C");

  display.setTextColor(GRAY);
  display.setTextSize(1);
  display.setCursor(620, 445);
  display.print("LAST PACKET");
  display.setTextColor(linkAlive ? GREEN : RED);
  display.setTextSize(2);
  display.setCursor(720, 440);
  float age = (millis() - lastPacketMs) / 1000.0;
  display.print(age, 1);
  display.print("s");
}

// ── Draw one channel tile ──────────────────────────────
void drawChannelTile(int ch, int x, int y, int w, int h) {
  // Tile background
  display.fillRoundRect(x, y, w, h, 8, TILEBG);
  display.drawRoundRect(x, y, w, h, 8, BLUE);

  // Channel label
  display.setTextColor(LTGRAY);
  display.setTextSize(2);
  display.setCursor(x + 15, y + 15);
  display.print("CHANNEL ");
  display.print(ch + 1);

  // Status indicator dot (top right of tile)
  uint16_t dotColor;
  if (!padArmed[ch])      dotColor = GRAY;      // not armed
  else if (padCont[ch])   dotColor = GREEN;     // armed + continuity OK
  else                    dotColor = YELLOW;    // armed but no loop
  display.fillCircle(x + w - 25, y + 23, 6, dotColor);

  // Big status text
  display.setTextSize(3);
  display.setCursor(x + 15, y + h - 55);

  if (!padArmed[ch]) {
    display.setTextColor(LTGRAY);
    display.print("DISARMED");
  } else if (padCont[ch]) {
    display.setTextColor(GREEN);
    display.print("ARMED");
  } else {
    display.setTextColor(YELLOW);
    display.print("NO LOOP");
  }
}

// ── Main loop ──────────────────────────────────────────
void loop() {
  // Read switches for display
  readSwitches();

  // Fire button — press event (HIGH -> LOW), from working code
  bool currentTriggerState = digitalRead(triggerPin);
  if (lastTriggerState == HIGH && currentTriggerState == LOW) {
    sendFire();
    delay(300);  // debounce
  }
  lastTriggerState = currentTriggerState;

  // Read telemetry from receiver
  if (Serial2.available() > 0) {
    String telem = Serial2.readStringUntil('\n');
    telem.trim();
    parseTelemetry(telem);
  }

  // Link timeout — no packet for 3 seconds = link down
  if (millis() - lastPacketMs > 3000) {
    linkAlive = false;
  }

  // Redraw screen periodically (every 250ms)
  static unsigned long lastDraw = 0;
  if (millis() - lastDraw > 250) {
    drawScreen();
    lastDraw = millis();
  }
}
