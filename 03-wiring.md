# Wiring

## Transmitter (Arduino GIGA R1)

The GIGA Display Shield mounts on the high-density connector in the middle of the board, so all 54 top-side pins stay free.

| Component | GIGA pin | Notes |
| --- | --- | --- |
| HC-12 VCC | 5V | |
| HC-12 GND | GND | |
| HC-12 TXD | D19 (RX2) | Serial2 |
| HC-12 RXD | D18 (TX2) | Serial2 |
| Pad 1 switch | D2 | to GND, INPUT_PULLUP |
| Pad 2 switch | D3 | to GND |
| Pad 3 switch | D4 | to GND |
| Pad 4 switch | D5 | to GND |
| Fire / master button | D6 | to GND |
| Extra ground | A0 | set LOW in code |
| Extra ground | A1 | set LOW in code |

> If you run short of GND pins for all the switches, set spare analog pins LOW in code and use them as ground points. Switches draw almost no current, so this is safe.

## Receiver (Arduino Nano R4)

| Component | Nano R4 pin | Notes |
| --- | --- | --- |
| HC-12 VCC | 5V | Must be a solid 5V |
| HC-12 GND | GND | |
| HC-12 TXD | D0 (RX) | Serial1 |
| HC-12 RXD | D1 (TX) | Serial1 |
| Relay CH1 | D4 | active-LOW |
| Relay CH2 | D5 | active-LOW |
| Relay CH3 | D6 | active-LOW |
| Relay CH4 | D7 | active-LOW |
| Continuity CH1 | A1 | INPUT_PULLUP |
| Continuity CH2 | A2 | INPUT_PULLUP |
| Continuity CH3 | A3 | INPUT_PULLUP |
| Continuity CH4 | A4 | INPUT_PULLUP |
| Ground reference | A5 | set LOW in code |

## Relay to igniter (per channel)

```
12V Battery (+) --> [Arming Plug] --> Relay COM
Relay NO --------> Igniter (+)
Igniter (-) -----> 12V Battery (-)
```

The **arming plug** sits between the battery and the relays. With it removed, the relays have no power to switch and no igniter can fire — this is the physical safety while you are at the pad.

## Continuity sensing (per channel)

Simplest reliable method, using the Nano's internal pull-ups:

```
Continuity pin (A1..A4)  <-- INPUT_PULLUP
        |
    igniter
        |
       GND
```

- Igniter connected: pulls the pin to GND -> reads LOW -> loop OK
- Igniter broken/missing: internal pull-up holds it HIGH -> "NO LOOP"

See [Continuity Sensing](06-continuity.md) for detail.

## Key rules

- **Crossover:** HC-12 TXD always goes to the Arduino's RX pin, HC-12 RXD to the Arduino's TX pin.
- **Same 5V:** power both HC-12 modules from a solid 5V. Low voltage breaks the receive side.
- **Relay boot state:** active-LOW relays are OFF when the pin is HIGH. The code sets them HIGH at boot so nothing fires on power-up.
