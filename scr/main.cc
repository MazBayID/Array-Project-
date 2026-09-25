#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "mochi_assets.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Pin Sensor Sentuh (TTP223)
#define PIN_TOUCH 4

// Pengaturan Animasi 16 FPS (~62.5 ms per frame)
unsigned long lastFrameTime = 0;
const int frameInterval = 62; 
int animationState = 0; // 0: Normal, 1: Berkedip, 2: Happy

void setup() {
  Serial.begin(115200);
  
  // Inisialisasi I2C ESP32-C3 SuperMini (SDA=8, SCL=9 atau sesuaikan pin Anda)
  Wire.begin(8, 9);
  
  pinMode(PIN_TOUCH, INPUT);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED Gagal Ditemukan!"));
    for(;;);
  }

  display.clearDisplay();
  // Render wajah dasar di awal
  display.drawBitmap(0, 0, mochi_base_face, 128, 64, 1);
  display.display();
}

void loop() {
  unsigned long currentMillis = millis();

  // Membaca sentuhan untuk mengubah ekspresi ala Mochi
  if (digitalRead(PIN_TOUCH) == HIGH) {
    animationState = 2; // Ubah ke ekspresi senang saat disentuh
    delay(150);         // Debounce sederhana
  }

  // Render animasi berkecepatan 16 FPS
  if (currentMillis - lastFrameTime >= frameInterval) {
    lastFrameTime = currentMillis;

    // Logika pergantian ekspresi mata parsial (Tanpa clearDisplay agar tidak flicker)
    if (animationState == 0) {
      display.drawBitmap(48, 24, mochi_eyes_open, 32, 16, 1);
      // Secara acak berkedip setiap beberapa waktu
      if (random(0, 30) == 5) animationState = 1; 
    } 
    else if (animationState == 1) {
      display.drawBitmap(48, 24, mochi_eyes_blink, 32, 16, 1);
      animationState = 0; // Kembali normal setelah 1 frame kedip
    } 
    else if (animationState == 2) {
      display.drawBitmap(48, 24, mochi_eyes_happy, 32, 16, 1);
      // Kembali ke normal setelah beberapa saat
      if (random(0, 20) == 1) animationState = 0; 
    }

    display.display();
  }
}
