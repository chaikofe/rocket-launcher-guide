# Continuity Sensing

Continuity answers one question per channel: **is the igniter connected and intact?**

- Loop closed (igniter connected) -> channel can fire -> shows **ARMED** when armed
- Loop open (igniter broken or missing) -> shows **NO LOOP**

You check this from the controller before firing, so you know the launch will actually work — and you find out from a safe distance, not by walking up to a dud.

## The reliable method: internal pull-ups

The simplest approach that does not suffer from floating pins:

```
Continuity pin (A1..A4)  <-- pinMode INPUT_PULLUP
        |
     igniter
        |
       GND
```

In code:

```cpp
for (int i = 0; i < 4; i++) pinMode(contPins[i], INPUT_PULLUP);

bool readContinuity(int ch) {
  return (digitalRead(contPins[ch]) == LOW);  // LOW = connected to GND = loop OK
}
```

- Igniter present: it connects the pin to GND, pin reads **LOW** -> loop OK
- Igniter broken/missing: the pull-up holds the pin **HIGH** -> NO LOOP

## Why not leave pins floating

If the continuity pins are left as plain `INPUT` with nothing wired, they float and read random values — you get channels randomly showing ARMED or NO LOOP. Using `INPUT_PULLUP` gives every unconnected channel a definite HIGH (NO LOOP) reading.

## Test it

1. Leave all continuity pins unwired -> every channel shows **NO LOOP**
2. Jumper one channel's pin (say A1) straight to GND -> that channel shows **ARMED** when armed, the rest stay **NO LOOP**

That confirms the logic and the display are correct.

## Safety note

The continuity test current is tiny (limited by the pull-up, well under a milliamp) — far below what it takes to fire an igniter. It is safe to run continuously.
