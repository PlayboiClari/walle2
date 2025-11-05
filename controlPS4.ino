#include <PS4Controller.h>

void setup() {
  Serial.begin(115200);
  Serial.println("Esperando conexión del mando PS4...");

  // Cambiá la dirección por la de tu ESP32
  // PS4.begin("CC:50:E3:A9:C2:8C");
  PS4.begin("D8:C0:A6:52:FE:AE");

  Serial.println("Listo para emparejar. Mantén presionado Share + PS.");
}

void loop() {
  if (PS4.isConnected()) {
    Serial.println("🎮 Mando PS4 conectado correctamente!");

    // Ejemplo: mostrar botones
    if (PS4.Cross()) Serial.println("❌ Botón X presionado");
    if (PS4.Circle()) Serial.println("⭕ Botón Círculo presionado");
    if (PS4.Square()) Serial.println("🔲 Botón Cuadrado presionado");
    if (PS4.Triangle()) Serial.println("🔺 Botón Triángulo presionado");

    delay(200);
  }

  }
