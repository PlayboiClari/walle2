#include <Bluepad32.h>

// --- Pines de los 4 motores ---
int I1 = 21; int I2 = 19;   // Motor 1
int I3 = 18; int I4 = 5;    // Motor 2
int I5 = 17; int I6 = 16;   // Motor 3
int I7 = 4;  int I8 = 0;    // Motor 4

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

// --- Callbacks de conexión ---
void onConnectedController(ControllerPtr ctl) {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      Serial.printf("CALLBACK: Controller conectado, index=%d\n", i);
      ControllerProperties properties = ctl->getProperties();
      Serial.printf("Modelo: %s, VID=0x%04x, PID=0x%04x\n",
                    ctl->getModelName().c_str(),
                    properties.vendor_id,
                    properties.product_id);
      myControllers[i] = ctl;
      return;
    }
  }
  Serial.println("CALLBACK: No hay espacio para más controles");
}

void onDisconnectedController(ControllerPtr ctl) {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      Serial.printf("CALLBACK: Control desconectado index=%d\n", i);
      myControllers[i] = nullptr;
      return;
    }
  }
  Serial.println("CALLBACK: Control no encontrado en la lista");
}

// --- Función rápida para escribir pines ---
void dw(int pin, int val) {
  digitalWrite(pin, val);
}

// --- MOVIMIENTOS ---
void Frente(int t) {
  dw(I1, LOW); dw(I2, HIGH);
  dw(I3, HIGH); dw(I4, LOW);
  dw(I5, HIGH); dw(I6, LOW);
  dw(I7, LOW); dw(I8, HIGH);
  delay(t);
}

void Atras(int t) {
  dw(I1, HIGH); dw(I2, LOW);
  dw(I3, LOW); dw(I4, HIGH);
  dw(I5, LOW); dw(I6, HIGH);
  dw(I7, HIGH); dw(I8, LOW);
  delay(t);
}

void Giro_Derecha(int t) {
  dw(I1, HIGH); dw(I2, LOW);
  dw(I3, HIGH); dw(I4, LOW);
  dw(I5, HIGH); dw(I6, LOW);
  dw(I7, HIGH); dw(I8, LOW);
  delay(t);
}

void Giro_Izquierda(int t) {
  dw(I1, LOW); dw(I2, HIGH);
  dw(I3, LOW); dw(I4, HIGH);
  dw(I5, LOW); dw(I6, HIGH);
  dw(I7, LOW); dw(I8, HIGH);
  delay(t);
}

void Izquierda(int t) {
  dw(I1, LOW); dw(I2, HIGH);
  dw(I3, LOW); dw(I4, HIGH);
  dw(I5, HIGH); dw(I6, LOW);
  dw(I7, HIGH); dw(I8, LOW);
  delay(t);
}

void Derecha(int t) {
  dw(I1, HIGH); dw(I2, LOW);
  dw(I3, HIGH); dw(I4, LOW);
  dw(I5, LOW); dw(I6, HIGH);
  dw(I7, LOW); dw(I8, HIGH);
  delay(t);
}

void Diago_AI(int t) {
  dw(I1, LOW); dw(I2, HIGH);
  dw(I3, LOW); dw(I4, LOW);
  dw(I5, HIGH); dw(I6, LOW);
  dw(I7, LOW); dw(I8, LOW);
  delay(t);
}

void Diago_AD(int t) {
  dw(I1, LOW); dw(I2, LOW);
  dw(I3, HIGH); dw(I4, LOW);
  dw(I5, LOW); dw(I6, LOW);
  dw(I7, LOW); dw(I8, HIGH);
  delay(t);
}

void Diago_aI(int t) {
  dw(I1, LOW); dw(I2, LOW);
  dw(I3, LOW); dw(I4, HIGH);
  dw(I5, HIGH); dw(I6, LOW);
  dw(I7, LOW); dw(I8, LOW);
  delay(t);
}

void Diago_aD(int t) {
  dw(I1, LOW); dw(I2, LOW);
  dw(I3, LOW); dw(I4, HIGH);
  dw(I5, LOW); dw(I6, LOW);
  dw(I7, HIGH); dw(I8, LOW);
  delay(t);
}

void Stop(int t) {
  dw(I1, LOW); dw(I2, LOW);
  dw(I3, LOW); dw(I4, LOW);
  dw(I5, LOW); dw(I6, LOW);
  dw(I7, LOW); dw(I8, LOW);
  delay(t);
}

// --- Mapeo de control ---
void processGamepad(ControllerPtr ctl) {
  int x = ctl->axisX();
  int y = ctl->axisY();

  // Zona muerta para evitar vibraciones
  int deadzone = 100;
  if (abs(x) < deadzone) x = 0;
  if (abs(y) < deadzone) y = 0;

  // --- Control con joystick izquierdo ---
  if (y < -200) Frente(50);
  else if (y > 200) Atras(50);
  else if (x < -200) Izquierda(50);
  else if (x > 200) Derecha(50);
  else Stop(20);

  // --- Botones ---
  if (ctl->a()) Giro_Izquierda(150);
  if (ctl->b()) Giro_Derecha(150);
  if (ctl->l1()) Diago_AI(150);
  if (ctl->r1()) Diago_AD(150);
  if (ctl->l2()) Diago_aI(150);
  if (ctl->r2()) Diago_aD(150);
  if (ctl->x()) Stop(100);
}

// --- Procesar todos los controles ---
void processControllers() {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    ControllerPtr myController = myControllers[i];
    if (myController && myController->isConnected()) {
      if (myController->isGamepad()) {
        processGamepad(myController);
      }
    }
  }
}

// --- Setup ---
void setup() {
  Serial.begin(115200);
  Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
  const uint8_t* addr = BP32.localBdAddress();
  Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n",
                addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.forgetBluetoothKeys();

  pinMode(I1, OUTPUT); pinMode(I2, OUTPUT);
  pinMode(I3, OUTPUT); pinMode(I4, OUTPUT);
  pinMode(I5, OUTPUT); pinMode(I6, OUTPUT);
  pinMode(I7, OUTPUT); pinMode(I8, OUTPUT);
}

// --- Loop ---
void loop() {
  BP32.update();      // No devuelve valor en tu versión
  processControllers();
  delay(100);
}
