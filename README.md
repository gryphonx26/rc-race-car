# PROJECT: HIGH-POWER RC RACE CAR
# SYSTEM ARCHITECTURE & DOCUMENTATION

## 1. FILE NAME
* **Source Code:** `BTS7960_RC_RaceCar.ino`

---

## 2. SYSTEM OVERVIEW
This project is a high-performance Radio Controlled (RC) race car. Unlike standard L298N-based projects, this system utilizes **BTS7960 High-Current Motor Drivers**, which are capable of driving heavy-duty motors with up to 43A of current. The system reads PWM signals from a standard 6-channel RC Receiver and uses differential mixing logic to translate stick movements into speed and direction.

---

## 3. CORE BEHAVIORS

### **A. Signal Mapping (PWM to Motor)**
The system uses the `pulseIn()` function to read raw signals from the RC Receiver (typically ranging from 1000ms to 2000ms). These values are mapped to a range of **-255 to 255**, allowing for natural center-stick "zero" positioning with forward and backward acceleration.

### **B. Dual Performance Modes**
The car features real-time performance toggling via the transmitter switches (Channels 5 and 6):
* **Mode 1 (Training Mode):** Power is dampened and steering sensitivity is reduced for easier handling.
* **Mode 2 (Race Mode):** Full PWM duty cycle is unlocked for maximum speed and aggressive handling.
* **Failsafe:** If the signal is lost or the receiver is disconnected, the system sets `mode = -1`, which effectively neutralizes motor output to prevent runaways.

---

## 4. HARDWARE PINOUTS (BTS7960 Driver)

| Component             | Arduino Pin | Description                    |
|-----------------------|-------------|--------------------------------|
| **Motor L - RPWM** | 5           | Left Motor Forward (PWM)       |
| **Motor L - LPWM** | 6           | Left Motor Backward (PWM)      |
| **Motor L - R_EN/L_EN**| 7, 8        | Left Driver Enable (High)      |
| **Motor R - RPWM** | 9           | Right Motor Forward (PWM)      |
| **Motor R - LPWM** | 10          | Right Motor Backward (PWM)     |
| **Motor R - R_EN/L_EN**| 11, 12      | Right Driver Enable (High)     |
| **RC Receiver CH1-6** | A0 to A5    | Analog Pins used as Digital In |

---

## 5. TECHNICAL SPECIFICATIONS
* **Motor Driver:** BTS7960 (43A Max)
* **Baud Rate:** 115200 (Optimized for real-time telemetry)
* **Input Range:** 950ms - 2020ms
* **Control Logic:** Differential Thrust Mixing (Tank-style steering)

---


}
