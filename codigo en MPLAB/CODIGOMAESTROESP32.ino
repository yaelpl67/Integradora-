#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define SLAVE_ADDRESS 0x08

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("No se pudo inicializar la pantalla OLED"));
    while(true);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
}

void loop() {
  uint16_t dato_sensor1, dato_sensor2;

  // Solicitar datos al esclavo
  Wire.requestFrom(SLAVE_ADDRESS, 4); // Solicitar 4 bytes al esclavo
  while (Wire.available() < 4); // Esperar a que lleguen los datos
  
  // Leer datos recibidos del esclavo
  dato_sensor1 = Wire.read() << 8; // Leer el byte más significativo
  dato_sensor1 |= Wire.read(); // Leer el byte menos significativo
  dato_sensor2 = Wire.read() << 8;
  dato_sensor2 |= Wire.read();
  
  // Limpiar la pantalla antes de imprimir nuevos datos
  display.clearDisplay();
  
  // Imprimir datos en la pantalla OLED
  display.setCursor(0, 0);
  display.print("Dato sensor 1: ");
  display.println(dato_sensor1);
  display.print("Dato sensor 2: ");
  display.println(dato_sensor2);
  display.display();

  delay(1000); // Esperar un segundo antes de la próxima solicitud
}
