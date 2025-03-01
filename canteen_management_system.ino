#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>
#include "BluetoothSerial.h"

#define EEPROM_SIZE 200

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;


#define RST_PIN 15
#define SS_PIN 5

byte readCard[4];

MFRC522 mfrc522(SS_PIN, RST_PIN);


#define ROW_NUM     4 // four rows
#define COLUMN_NUM  4 // four columns
char keys[ROW_NUM][COLUMN_NUM] = {
  {'F', '1', '2', '3'},
  {'G', '4', '5', '6'},
  {'H', '7', '8', '9'},
  {'I', 'C', '0', 'E'}
};

String RF_Cards[] = {"000000","43955331","102A51A", "C163FA24"};
String current_contact;
String tagID = "";
// String current_card;
int current_user_index = 9;


byte pin_rows[ROW_NUM]      = {32,33,25,26}; // GPIO19, GPIO18, GPIO5, GPIO17 connect to the row pins
byte pin_column[COLUMN_NUM] = {27,14,12,13};   // GPIO16, GPIO4, GPIO0, GPIO2 connect to the column pins
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display

char keypressed;
int current_serial = 0;
int current_qty = 0;
int buzzer = 2;

int max_dishes = 2;
int cart_dishes[] = {0,0,0,0,0};
int cart_qty[] = {0,0,0,0,0};

String dishes[] = {"None","Idly Set", "Plain Dosa", "Fried Rice", "Biriyani", "Vadaa", "Poori","Pongal"};
int prices[] = {0,20, 40, 80, 110, 8, 30, 25};



void setup() {
  Serial.begin(115200);
  SerialBT.begin("SmartCanteen");
  Serial.println("The device started, now you can pair it with bluetooth!");
  EEPROM.begin(EEPROM_SIZE);
  SPI.begin(); // SPI bus
  mfrc522.PCD_Init(); // MFRC522
  pinMode(buzzer, OUTPUT);
  lcd.init();
  lcd.clear();         
  lcd.backlight();      // Make sure backlight is on
