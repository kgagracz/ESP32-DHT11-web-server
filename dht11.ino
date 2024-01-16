#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DHTPIN 18
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define OLED_SDA 21
#define OLED_SCL 22
#define WIRE Wire
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &WIRE);

#define LED_HUM_TOO_HIGH_PIN 2
#define LED_TEMP_TOO_HIGH_PIN 4

const String ssid = "TP-LINK_A137";
const String password = "14521706";

float const MAX_TEMPERATURE = 25.0;
float const MAX_HUMIDITY = 50.0;

float readDHTTemperature() {
  // Sensor readings may also be up to 2 seconds
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  if (isnan(t)) {    
    Serial.println("Failed to read from DHT sensor!");
    return -1;
  }
  else {
    Serial.println(t);
    return t;
  }
}

float readDHTHumidity() {
  // Sensor readings may also be up to 2 seconds
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return -1;
  }
  else {
    Serial.println(h);
    return h;
  }
}

void setup() {
  Serial.begin(115200);

  //begining dht reading
  dht.begin();
  //beggining OLED display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32
  display.display();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);

  //setting pinmode for LED's
  pinMode(LED_HUM_TOO_HIGH_PIN, OUTPUT);
  pinMode(LED_TEMP_TOO_HIGH_PIN, OUTPUT);

  //clearing display
  delay(1000);
  display.clearDisplay();
  display.display();
}

bool maxTempExceeded = false;
bool maxHumExceeded = false;

void handleLEDs() {
  digitalWrite(LED_TEMP_TOO_HIGH_PIN, maxTempExceeded ? HIGH : LOW);
  digitalWrite(LED_HUM_TOO_HIGH_PIN, maxHumExceeded ? HIGH : LOW);
}

void handleDisplay(float t, float h) {
  display.setCursor(0,0);
  display.print(t);
  display.print(" C\n");
  display.print(h);
  display.print(" %");
  display.display();
}

void loop() {
  float t = readDHTTemperature();
  float h = readDHTHumidity();
  maxTempExceeded = t > MAX_TEMPERATURE;
  maxHumExceeded = h > MAX_HUMIDITY;
  handleLEDs();
  handleDisplay(t, h);
  delay(1000);
  display.clearDisplay();
}
