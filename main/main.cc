#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "dirgamochi_assets.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Variabel untuk kontrol kecepatan 16 FPS (~62.5 ms per frame)
unsigned long lastFrameTime = 0;
const int frameInterval = 62; 
int currentFrame = 0;

void setup() {
  Wire.begin(21, 20); // Pin I2C ESP32-C3 SuperMini
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for(;;);
  }

  display.clearDisplay();
  
  // Gambar Wajah Dasar Dirgamochi Sekali Saja di Background (0,0)
  display.drawBitmap(0, 0, dirgamochi_base, 128, 64, 1);
  display.display();
}

void loop() {
  unsigned long currentMillis = millis();

  // Logika pengatur waktu 16 FPS (16 frame per detik)
  if (currentMillis - lastFrameTime >= frameInterval) {
    lastFrameTime = currentMillis;

    // 1. Timpa/Update HANYA area mata saja (Koordinat X=48, Y=24) 
    // Tanpa melakukan clearDisplay() seluruh layar agar tidak berkedip (flicker)
    display.drawBitmap(48, 24, animasi_mata[currentFrame], 32, 16, 1);
    display.display();

    // 2. Pindah ke frame animasi berikutnya secara berputar (Looping)
    currentFrame = (currentFrame + 1) % TOTAL_MATA_FRAMES;
  }

  // Di sini kamu bisa meletakkan fungsi lain seperti pembacaan sensor sentuh atau jam
}
