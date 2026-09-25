# Transmitter Code

Full sketch: [`code/transmitter/transmitter.ino`](../code/transmitter/transmitter.ino)

## What it does

- Reads the arm switches and fire button
- Sends a 4-character command (e.g. `1010`) over the HC-12 when fired
- Reads telemetry back from the receiver
- Draws the dashboard on the GIGA Display Shield

## Library needed

Install via Arduino IDE Library Manager:

- **Arduino_GigaDisplay_GFX**

Also make sure the **Arduino Mbed OS GIGA Boards** core is installed and up to date.

## Radio port

The GIGA uses hardware **Serial2** (pins D18/D19) for the HC-12:

```cpp
Serial2.begin(9600);   // HC-12
```

## Message format

When you press fire, the sketch builds a string from the pad switches and sends it:

```
"1010"  -> fire channels 1 and 3
```

## Telemetry it expects back

```
TELEM,batt,temp,cont1,cont2,cont3,cont4
```

Example `TELEM,7.8,22,1,1,1,1` -> battery 7.8V, 22°C, all four channels have continuity.

## Display notes

The screen is drawn once as a static layout, then only the values that change are redrawn. This avoids the full-screen flicker you get if you call `fillScreen()` every loop. If you modify the UI, keep that pattern: draw the static frame in `setup()`, update only changed regions in `loop()`.

## Tuning points

- **Switch pins** — set at the top of the sketch; change to match your wiring.
- **Extra ground pins** — set spare analog pins LOW in `setup()` if you need more grounds for switches.
