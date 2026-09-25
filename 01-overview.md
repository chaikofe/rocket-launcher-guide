# Overview & How It Works

## The system in one picture

```
[ Handheld Controller ]                      [ Launch Pad Receiver ]
   Arduino GIGA R1              433 MHz          Arduino Nano R4
   + Display Shield      <----- HC-12 ----->     + 4 relays
   + 5 arm switches                              + 4 continuity sensors
   + fire button                                 + 4 igniters
```

Two boxes. One in your hand, one at the pad. They talk over a 433 MHz HC-12 radio link. You arm the pads you want and press fire; the receiver closes the matching relays and lights the igniters.

## What each side does

**Transmitter (GIGA):**
- Reads 5 switches — one master arm plus one per pad (or a simpler layout, your choice)
- Reads a fire button
- Sends a short coded message over the radio when you fire
- Shows everything on the display: armed state, continuity per channel, radio link, remote battery and temperature

**Receiver (Nano R4):**
- Listens for fire commands over the radio
- Closes the relay for each armed channel, holds it briefly, then opens it
- Continuously checks igniter continuity on each channel
- Sends telemetry back so the controller can display pad status

## The radio link

The HC-12 is a 433 MHz transparent-serial radio. Whatever one side sends over serial comes out the other side. Two HC-12 modules with matching settings talk to each other with no pairing needed.

Messages are short strings — for example `1010` means "fire channels 1 and 3." Telemetry comes back as `TELEM,7.8,22,1,1,1,1` (battery, temperature, and continuity for each of the 4 channels).

## Why these boards

- **GIGA R1** — plenty of GPIO, hardware serial ports, and it accepts the GIGA Display Shield for a proper touchscreen dashboard.
- **Nano R4** — small, cheap, 5V logic (matches the HC-12 and relays), and a real hardware UART on D0/D1 for the radio.

> Note: the classic ATmega Nano and the Nano R4 are very different chips. On the R4 the radio must use the hardware `Serial1` on D0/D1 — the old `SoftwareSerial`-on-D10/D11 trick does not work reliably. This tripped up the original build; see [Troubleshooting](09-troubleshooting.md).

## Safety layers

The system is designed so no single failure fires a motor:

1. **Master arm switch** — nothing arms unless it is on
2. **Per-pad arm switches** — each pad is separately enabled
3. **Coded radio message** — random RF will not match a valid fire command
4. **Physical arming plug at the pad** — the firing relay has no power until a plug is inserted by hand, so the pad is electrically dead while anyone is near it

See [Safety Procedure](08-safety.md) for the full operating sequence.
