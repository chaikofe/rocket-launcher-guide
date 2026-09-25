# Setup & First Test

Build and test in stages. Never connect a real igniter until the whole chain works with an LED or relay LED standing in for the igniter.

## Step 1 — Install software

- Arduino IDE
- **Arduino Mbed OS GIGA Boards** core (for the GIGA)
- **Arduino UNO R4 / Renesas** core (for the Nano R4)
- **Arduino_GigaDisplay_GFX** library (for the display)

## Step 2 — Prove the radio link

Before anything else, confirm the two HC-12 modules talk. Upload a minimal sender to the GIGA and a minimal receiver to the Nano R4:

**GIGA (sender):**
```cpp
void setup(){ Serial.begin(115200); Serial2.begin(9600); }
void loop(){ Serial2.println("HELLO"); Serial.println("sent"); delay(1000); }
```

**Nano R4 (receiver):**
```cpp
void setup(){ Serial.begin(9600); Serial1.begin(9600); Serial.println("RX ready"); }
void loop(){ if(Serial1.available()){ Serial.write(Serial1.read()); } }
```

The Nano R4 Serial Monitor should print `HELLO` once a second. If not, see [Troubleshooting](09-troubleshooting.md) — usually HC-12 power or the D0/D1 wiring.

## Step 3 — Test firing with LEDs

Upload the real receiver sketch. Put LEDs (or use the relay module's own LEDs) on the relay outputs. Upload the transmitter sketch. Arm a channel and press fire — the matching relay LED should turn on for the hold interval, then off.

## Step 4 — Test continuity

Leave continuity pins unwired: every channel should show **NO LOOP**. Jumper one to GND: that channel shows **ARMED** when armed. Confirms the sensing works.

## Step 5 — Test telemetry on the display

With both boards running, the controller screen should show remote battery, temperature, and the per-channel status updating live.

## Step 6 — Field test without a motor

Take it outside. Connect a real igniter (no motor attached) at a safe distance. Confirm it fires reliably over the full range you plan to use.

## Step 7 — Live fire

Only after every stage above works: insert the arming plug at the pad, walk back, arm, and fire a real motor following the [Safety Procedure](08-safety.md).
