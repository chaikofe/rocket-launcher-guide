# Receiver Code

Full sketch: [`code/receiver/receiver.ino`](../code/receiver/receiver.ino)

## What it does

- Listens for fire commands over the HC-12
- Closes the relay for each armed channel, holds it for a set interval, then opens it
- Reads igniter continuity on each channel
- Sends telemetry back to the controller once per second

## Radio port

The Nano R4 uses hardware **Serial1** (pins D0/D1) for the HC-12:

```cpp
Serial1.begin(9600);   // HC-12 on D0/D1
```

> This is the single most important detail for the R4. The classic `SoftwareSerial` library on D10/D11 does not work reliably on the R4 — use hardware `Serial1` on D0/D1.

## Relay logic (active-LOW)

The common 4-channel relay modules are active-LOW:

```cpp
digitalWrite(pin, HIGH);  // relay OFF
digitalWrite(pin, LOW);   // relay ON
```

At boot the code sets all relay pins HIGH so nothing fires on power-up. Confirm this: the relay LEDs should be OFF when the board starts.

## Fire timing

When a fire command arrives, the matching relays close and a non-blocking timer opens them again after the interval (default 5 seconds). Non-blocking means the sketch keeps reading the radio and sending telemetry while the relays are held.

```cpp
const long interval = 5000;   // relay ON time, ms
```

## Continuity

Continuity uses the internal pull-ups (see [Continuity Sensing](06-continuity.md)):

```cpp
pinMode(contPins[i], INPUT_PULLUP);
...
bool readContinuity(int ch) {
  return (digitalRead(contPins[ch]) == LOW);  // LOW = loop closed = OK
}
```

## Telemetry format

```
TELEM,batt,temp,cont1,cont2,cont3,cont4
```

Battery and temperature are placeholders until you wire real sensors — replace `readBattery()` and `readTemp()` with real analog reads.
