# Bill of Materials

Approximate prices in USD. Sources: AliExpress, OZON Armenia, local electronics shops.

## Transmitter (handheld controller)

| Component | Qty | Approx price | Notes |
| --- | --- | --- | --- |
| Arduino GIGA R1 WiFi | 1 | $75 | Main controller |
| GIGA Display Shield | 1 | $50 | 3.4" touchscreen dashboard |
| HC-12 433 MHz module | 1 | $4 | With antenna |
| Toggle switches | 5 | $8 | 1 master + 4 pad arms |
| Fire button (momentary) | 1 | $5 | Any momentary push button |
| Battery (7–12V pack) | 1 | $10 | Powers the GIGA |
| Project case | 1 | $15–30 | Plastic, big enough for GIGA + display |
| Hookup wire, headers | — | $5 | |

## Receiver (launch pad)

| Component | Qty | Approx price | Notes |
| --- | --- | --- | --- |
| Arduino Nano R4 | 1 | $20 | Receiver |
| HC-12 433 MHz module | 1 | $4 | With antenna |
| 4-channel relay module | 1 | $6 | Active-LOW (see note) |
| Arming plug (2-pin connector) | 1 | $2 | Physical safety |
| 1 kohm resistors | 4 | $0.20 | Continuity sensing (if using analog method) |
| 12V battery (SLA or LiPo) | 1 | $12 | Powers relays / igniters |
| Screw terminals | 4 | $2 | Igniter connections |
| Weatherproof case | 1 | $12 | Field use |
| Hookup wire | — | $5 | |

## Igniters

| Component | Qty | Approx price | Notes |
| --- | --- | --- | --- |
| Commercial e-matches | 5+ | $2 each | Most reliable |
| Nichrome wire (DIY backup) | 1 m | $5 | 32 AWG |

## Safety gear

| Component | Qty | Approx price | Notes |
| --- | --- | --- | --- |
| Fire extinguisher | 1 | $25 | Mandatory |
| Safety glasses | 2 | $6 | |

## Notes

- **Relay polarity:** the common blue 4-channel relay modules are usually **active-LOW** (a LOW signal turns the relay ON). The receiver code accounts for this. Confirm yours at boot — the relay LEDs should be OFF when the board starts.
- **HC-12 power:** both modules need a solid ~5V. Under-volting (e.g. 3.6V) makes the receive side fail. See [Troubleshooting](09-troubleshooting.md).
