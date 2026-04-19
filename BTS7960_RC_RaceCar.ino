// ==========================================
// --- Hardware Pin Definitions ---
// ==========================================

// Motor 1 (Left) - BTS7960 High Power Driver
int RPWM_L = 5;
int LPWM_L = 6;
int R_EN_L = 7;
int L_EN_L = 8;

// Motor 2 (Right) - BTS7960 High Power Driver
int RPWM_R = 9;
int LPWM_R = 10;
int R_EN_R = 11;
int L_EN_R = 12;

// RC Receiver Input Pins
int receiver_pins[] = {A0, A1, A2, A3, A4, A5};
int receiver_values[] = {0, 0, 0, 0, 0, 0};

// Calibration Constants
int res_min = 950;
int res_max = 2020;
int working_range = 255;

// System Variables
boolean prt = true; // Set to false to disable Serial monitoring
int mode = 0;
int rp = 0;

// ==========================================
// --- Setup ---
// ==========================================

void setup() {
  pinMode(RPWM_L, OUTPUT);
  pinMode(LPWM_L, OUTPUT);
  pinMode(RPWM_R, OUTPUT);
  pinMode(LPWM_R, OUTPUT);

  pinMode(R_EN_L, OUTPUT);
  pinMode(L_EN_L, OUTPUT);
  pinMode(R_EN_R, OUTPUT);
  pinMode(L_EN_R, OUTPUT);

  // Enable both drivers (Set EN pins HIGH)
  digitalWrite(R_EN_L, HIGH);
  digitalWrite(L_EN_L, HIGH);
  digitalWrite(R_EN_R, HIGH);
  digitalWrite(L_EN_R, HIGH);

  Serial.begin(115200);
}

// ==========================================
// --- Main Loop ---
// ==========================================

void loop() {
  receive();

  int m1 = 0;
  int m2 = 0;

  int rot = receiver_values[0]; // Rotation/Steering input

  if (mode == 1) {
    // Low Power Mode
    m1 = receiver_values[1] / 2 + rot / 1.5;
    m2 = receiver_values[1] / 2 - rot / 1.5;
  } 
  else if (mode == 2) {
    // High Power Mode
    m1 = receiver_values[1] + rot / 1.75;
    m2 = receiver_values[1] - rot / 1.75;
  }

  driveMotor(RPWM_L, LPWM_L, m1);
  driveMotor(RPWM_R, LPWM_R, m2);
}

// ==========================================
// --- Receiver Functions ---
// ==========================================

void receive() {
  // Read PWM pulse from the receiver and map to motor range
  receiver_values[rp] = map(pulseIn(receiver_pins[rp], HIGH),
                            res_min, res_max,
                            -working_range, working_range);

  rp++;
  if (rp == 6) rp = 0;

  boolean activevalues = true;

  for (int i = 0; i < 6; i++) {
    if (prt) {
      Serial.print("CH");
      Serial.print(i);
      Serial.print(": ");
      Serial.print(receiver_values[i]);
      Serial.print("\t");
    }
    // Check if signal is lost
    if (receiver_values[i] < -500) {
      activevalues = false;
    }
  }

  mode = 0;

  if (!activevalues) {
    mode = -1; // Failsafe Mode
  } else if (receiver_values[4] > -100) {
    mode = 2; // Performance Mode
  } else if (receiver_values[5] > -100) {
    mode = 1; // Training Mode
  }

  if (prt) Serial.println();
}

// ==========================================
// --- Motor Control (BTS7960 Logic) ---
// ==========================================

void driveMotor(int RPWM, int LPWM, int spd) {
  // Constrain speed to PWM limits
  if (spd > 255) spd = 255;
  if (spd < -255) spd = -255;

  if (spd > 0) {
    analogWrite(RPWM, spd);
    analogWrite(LPWM, 0);
  } else if (spd < 0) {
    analogWrite(RPWM, 0);
    analogWrite(LPWM, -spd); // Use absolute value for backward speed
  } else {
    analogWrite(RPWM, 0);
    analogWrite(LPWM, 0);
  }
}
