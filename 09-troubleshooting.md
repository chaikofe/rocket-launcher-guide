# Troubleshooting

Real problems hit during this build, and how they were solved.

## Receiver receives nothing (transmit works)

The most common failure. Checklist in order:

1. **HC-12 power.** Measure VCC on the receiver's HC-12 — it must be a solid ~5V. At 3.6V the transmit side works but the receive side fails silently. Move VCC to a real 5V pin; if the board's own 5V reads low, fix the USB cable/port or power it better.
2. **Wrong serial on the Nano R4.** The R4 must use hardware `Serial1` on **D0/D1**. The classic `SoftwareSerial` on D10/D11 does not work reliably on the R4. Move the HC-12 to D0/D1 and use `Serial1`.
3. **TX/RX crossover.** HC-12 TXD -> Arduino RX, HC-12 RXD -> Arduino TX. Swapped is the number-one wiring mistake.
4. **Mismatched HC-12 settings.** If a module was ever reconfigured, reset both to defaults: connect the SET pin to GND, open a 9600 serial passthrough, send `AT+DEFAULT`, expect `OK+DEFAULT`. Do both modules.

## Upload fails: "not in sync" or "programmer not responding"

- Wrong board selected. If the compile output says "Maximum is 30720 bytes," the IDE is building for a classic Nano. Select the correct board (Nano R4 / GIGA).
- Wrong COM port. Check Tools -> Port; with two boards connected it is easy to pick the wrong one.
- Bad USB cable. A charge-only cable will not upload. Use a known data cable, and a data-capable port (some laptop USB-C ports are charge-only).

## Upload fails on GIGA: "No DFU capable USB device"

- Double-tap the reset button to force bootloader mode, then upload.
- Use a data USB-C cable on a data port.
- The "Invalid DFU suffix signature" line is only a warning — if the sketch runs, ignore it.

## Relays all ON at boot

The relay module is active-LOW. Set the relay pins HIGH at boot (HIGH = OFF) and drive them LOW to fire. Confirm: relay LEDs OFF at power-up.

## Display flickers constantly

Do not call `fillScreen()` every loop. Draw the static layout once in `setup()`, then update only the values that changed. Full-screen redraws every frame cause the flash.

## Continuity channels random / all showing wrong

Floating analog pins read garbage. Use `INPUT_PULLUP` and `digitalRead()`:
- Igniter to GND -> LOW -> loop OK
- Nothing connected -> pull-up HIGH -> NO LOOP

## Fire button does nothing

- Confirm it is wired to GND with `INPUT_PULLUP` (pressed = LOW).
- If it has 4 legs, only two are the switch; the others are the built-in LED (or duplicate legs). Use a multimeter's continuity mode to find the switched pair.
