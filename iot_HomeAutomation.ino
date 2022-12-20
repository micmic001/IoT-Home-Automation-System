#include <SPI.h>
#include<LiquidCrystal_I2C.h>
#include <MFRC522.h>
#include <Wire.h>
 
#define SS_PIN 2
#define RST_PIN 0
#define RELAY 16 //relay pin
#define PiezoPin 15 //buzzer pin
#define ACCESS_DELAY 5000
#define DENIED_DELAY 300
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
LiquidCrystal_I2C lcd(0x27, 16, 2);
 
void setup() 
{
  Wire.begin(D2,D1);
  Serial.begin(115200);   // Initiate a serial communication
  lcd.begin(16, 2);   // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight 
  lcd.home(); 
  lcd.print("Hello"); // Start Printing
  lcd.setCursor(0, 1);
  SPI.begin();          // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  pinMode(RELAY, OUTPUT);
  pinMode(PiezoPin, OUTPUT);
  digitalWrite(RELAY, HIGH);
  Serial.println("Put your card to the reader...");
  Serial.println();
  lcd.print("Tap Card/Key");

}
void loop() 
{
  Serial.println("Ready");
  delay(300);
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  lcd.setCursor(0, 1);
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "43 7C 16 97") //change here the UID of the card/cards that you want to give access
  {
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(content.substring(1));
    lcd.setCursor(0, 0);
    Serial.println("Authorized");
    lcd.print("Authorized");
    Serial.println();
    digitalWrite(PiezoPin, HIGH), delay(300), digitalWrite(PiezoPin, LOW);
    delay(500);
    digitalWrite(RELAY, LOW);
    delay(ACCESS_DELAY);
    digitalWrite(RELAY, HIGH);
  }
 
 else   {
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(content.substring(1));
    lcd.setCursor(0, 0);
    Serial.println(" Access denied");
    lcd.print("Access denied");
    for (int i = 0; i <= 2; i++)delay(100), digitalWrite(PiezoPin, HIGH), delay(100), digitalWrite(PiezoPin, LOW);
    delay(500);
    delay(DENIED_DELAY);
  }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Tap Card/Key");
    lcd.setCursor(0, 1);
    lcd.print("Ready");
}
