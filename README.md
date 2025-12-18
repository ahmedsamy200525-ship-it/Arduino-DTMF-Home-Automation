# Hybrid GSM-Based Home Automation (DTMF Technology)
** Project Overview

This project was developed as part of the Electronics 1 course within the Mechatronics Engineering program. It introduces a reliable, secure, and internet-independent solution for remote appliance control. By utilizing a real SIM card and DTMF (Dual-Tone Multi-Frequency) decoding, the system allows users to control high-voltage devices (like a lamp) via standard mobile phone calls from anywhere in the world.

* Key Features
Internet-Free Operation: The system operates purely on cellular GSM signals, making it ideal for locations with poor or no internet connectivity.

Enhanced Security: Integrated logic ensures the system only responds to authorized phone numbers, preventing unauthorized access and control.

Hybrid Control Interface: Users can still operate the devices using traditional physical buttons, maintaining a manual override alongside the remote mobile functionality.

Real-Time Feedback: Instant decoding of keypad tones (e.g., '1' for ON, '0' for OFF) to trigger the microcontroller.

* Hardware Components
Microcontroller: Arduino Uno (The brain of the system).

Communication: GSM Module with a real, active SIM card.

Decoder: MT8870 DTMF Module (To interpret keypad tones).

Output Control: 5V Relay Module (To safely switch the AC lamp load).

Input: Tactile buttons for manual control.

* How It Works
Call & Connect: The user calls the SIM number attached to the GSM module.

Authentication: The system verifies the caller's number against a secure authorized list.

* Author::

Ahmed Samy || Mechatronics Engineering Student




https://github.com/user-attachments/assets/febc4453-c617-4dff-aa92-26425d01a67b







https://github.com/user-attachments/assets/73516fac-a1c3-42f4-b275-433e0dc94ead






![photo_2025-12-17_14-33-10 (3)](https://github.com/user-attachments/assets/04d65f26-7f45-4e53-bbc9-16faccc43ae2)



Command Execution: Once connected, the user presses a digit on their phone's keypad. The MT8870 decodes this tone and sends a binary signal to the Arduino.

Action: The Arduino toggles the Relay, turning the lamp ON or OFF based on the received command.
