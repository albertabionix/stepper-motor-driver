# Bionix Stepper Motor Driver

![Assembled driver and motor](images/assembled.png)

USB serial control of a NEMA 17 stepper motor through an Arduino Nano and an A4988 driver.

| Directory | Content |
|---|---|
| `firmware/` | Arduino firmware source |
| `solidworks/` | Enclosure CAD |
| `images/` | Figures |

## 1 Safety

> **WARNING:** The motor starts as soon as `start` is sent. Keep hands clear of the transmission, and send `stop` before reaching in.

> **CAUTION:** The motor driver heatsink gets hot. Let it cool before touching it. 

> **CAUTION:** After `stop`, the motor has no holding torque, so a load can backdrive it.

> **NOTICE:** Disconnect the DC supply before disconnecting the motor. 

> **NOTICE:** Use only a centre-positive DC supply between 8 V and 35 V.

## 2 Specifications

| Parameter | Value |
|---|---|
| Motor supply | 8 V to 35 V DC, 5.5 mm × 2.1 mm barrel jack, centre-positive |
| Logic supply | 5 V over Mini-USB |
| Motor connector | Molex 50-57-9404 |
| Speed range | 5 rpm to 375 rpm |
| Microstepping | 1/16 (3200 microsteps per revolution) |
| Serial | 115200 baud, 8N1 |

## 3 Components

| Qty | Part | Specification |
|---|---|---|
| 1 | Arduino Nano | Rev3, ATmega328P, Mini-USB |
| 1 | Stepper driver module | Pololu A4988, with heatsink |
| 1 | Electrolytic capacitor | 100 µF, 50 V |
| 2 | Ceramic capacitor | 100 nF |
| 1 | Solderless breadboard | Full size, 830 tie points |
| 1 | DC jack adapter | Female barrel jack, 5.5 mm × 2.1 mm, with screw terminals |
| 1 | Stepper motor with Molex connector | Prusa 2042-000013 |
| 2 | Motor housing | `left_top`, `left_bottom`, printed in PLA |
| 2 | Power housing | `right_top`, `right_bottom`, printed in PLA |
| 8 | Heat-set insert | M2, 3 mm long, 3.5 mm outer diameter |
| 4 | Screw | M2 × 8 mm, for the motor housing |
| 4 | Screw | M2 × 16 mm, for the power housing |

## 4 Setup

1. Connect the motor to the Molex 4-pin connector.
2. Connect the DC supply to the DC-in jack.
3. Connect the Mini-USB port to the computer.
4. In the Arduino IDE, open Tools > Serial Monitor and select the Nano's COM port.
5. Set the baud rate to 115200 and the line ending to "Newline".

Result: The Serial Monitor shows `Commands: start | stop | cw | ccw | speed <rpm>`.

- If the Nano has no firmware, upload `firmware/firmware.ino` with Tools > Board > Arduino Nano. If the upload fails, select Tools > Processor > ATmega328P (Old Bootloader).

## 5 Operation

![Serial Monitor session](images/serial_monitor.png)

| Command | Effect |
|---|---|
| `start` | Starts the motor at the set speed. |
| `stop` | Stops the motor and removes coil power. |
| `cw` | Sets the direction to clockwise. |
| `ccw` | Sets the direction to counter-clockwise. |
| `speed <rpm>` | Sets the speed in rpm. |

- Commands are not case-sensitive and take effect immediately, including while running.
- At power-up the motor is off, the direction is CW, and the speed is 100 rpm.
- Speeds outside 5 rpm to 375 rpm are clamped to the nearest limit.
- Above about 300 rpm, start at a lower speed and raise it in steps to avoid a stall.
- Opening the Serial Monitor resets the Nano, which stops the motor and restores the defaults.

## 6 Troubleshooting

| Symptom | Remedy |
|---|---|
| No response in the Serial Monitor | Check the COM port, 115200 baud, and "Newline". |
| `Unknown command` or `Usage: speed <rpm>` | Check the command spelling and send a positive speed. |
| Motor does not turn after `start` | Check that the DC supply is connected and on. |
| Motor hums without turning | Send `stop`, lower the speed, and send `start`. |
| Motor stops when the Serial Monitor opens | Send `start` again. |
