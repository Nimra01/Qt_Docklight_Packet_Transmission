# Qt-Based Packet Sender and Receiver Using Docklight

## Overview

This project demonstrates a **Qt-based GUI application** that sends and receives serial data packets to and from **Docklight** using **virtual COM ports** created with **com0com**.

The application simulates real-world serial communication by allowing a Qt program and Docklight to exchange packets over paired virtual ports, making it ideal for:

- Protocol testing  
- Packet validation  
- GUI-based serial debugging  
- Embedded communication prototyping  

This setup removes the need for physical hardware during early development and testing.

---

## Features

- Qt GUI for packet transmission and reception  
- Full-duplex serial communication  
- Virtual COM port connection using com0com  
- Packet-based data exchange with Docklight  
- Real-time display of received data  
- User-configurable COM port and baud rate  
- Suitable for protocol development and validation  

---

## Tools & Technologies

- Qt (Qt Creator, C++)  
- Docklight (Serial communication test tool)  
- com0com (Virtual COM port driver)  
- Windows OS  

---

## System Architecture

Qt Application ←→ Virtual COM Port Pair ←→ Docklight
(COM5) (COM6)


com0com creates paired ports (e.g., COM5 ↔ COM6).  
Qt opens one port, Docklight opens the other.

---

## Setup Instructions

### 1. Install com0com

Download and install com0com:

https://sourceforge.net/projects/com0com/

After installation, create a virtual port pair (example):

```bash
setupc install PortName=COM5 PortName=COM6
```
Verify ports using Device Manager.

2. Configure Docklight
Open Docklight
```bash
Select COM6 (or your paired port)

Set baud rate (must match Qt application)

Configure send/receive packets
```
3. Build Qt Application
Clone your repository:
```
git clone <your_repo_url>
cd <your_project_folder>
```
Open the project in Qt Creator:

Select kit (MinGW/MSVC)

Build the project

Run

4. Run Communication
Launch Docklight on COM6

Launch Qt GUI on COM5

Send packets from either side

Observe bidirectional communication

Data Flow
Qt GUI
  ↓
Virtual COM Port
  ↓
Docklight
  ↑
Virtual COM Port
  ↑
Qt GUI
Use Cases
Serial protocol development

Packet structure validation

GUI-based communication testing

Embedded system simulation

UART debugging without hardware

## Notes
Baud rate and port settings must match on both sides

com0com ports must be created before running the application
