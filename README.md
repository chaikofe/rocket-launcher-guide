# Wireless Rocket Launch Controller

A complete guide to building a wireless rocket motor ignition system using an Arduino GIGA R1 (with Display Shield) as the handheld controller and an Arduino Nano R4 as the launch-pad receiver, linked by HC-12 433 MHz radio.

Built and flight-tested for student rocketry.

![status](https://img.shields.io/badge/status-flight%20tested-brightgreen)

## What this guide covers

This guide walks through building a launch system that can:

- Fire up to 4 rocket motors wirelessly from a safe distance
- Arm each pad independently with physical switches
- Show live status on a touchscreen: armed state, per-channel continuity, radio link, battery, temperature
- Detect igniter continuity ("NO LOOP" warning) before firing
- Send telemetry back from the pad to the controller

It's written for someone with basic electronics knowledge (breadboarding, soldering, multimeter use) and beginner Arduino experience.

## Safety first

Static-firing and launching rocket motors is dangerous. This controller is designed around multiple safety layers, but **you are responsible for safe operation**. Always:

- Keep a safe standoff distance (50 m+ for the motors in question)
- Never stand in line with a motor's thrust axis
- Have fire suppression on hand
- Check local laws before igniting any rocket motor
- Use the physical arming plug (see safety docs) so the firing circuit is dead while anyone is near the pad

## Sections

- [Overview & How It Works](docs/01-overview.md)
- [Bill of Materials](docs/02-bill-of-materials.md)
- [Wiring](docs/03-wiring.md)
- [Transmitter Code](docs/04-transmitter.md)
- [Receiver Code](docs/05-receiver.md)
- [Continuity Sensing](docs/06-continuity.md)
- [Setup & First Test](docs/07-setup-and-test.md)
- [Safety Procedure](docs/08-safety.md)
- [Troubleshooting](docs/09-troubleshooting.md)

## Code

- [Transmitter (GIGA)](code/transmitter/) — handheld controller with display
- [Receiver (Nano R4)](code/receiver/) — launch-pad unit with relays

## License

MIT — free to use, modify, and share. Build responsibly.
