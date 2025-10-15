#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

#define RST_PIN 12 // (RFID) Reset pin
#define SS_PIN  13 // (RFID) Serial input
#define SERVO_PIN 9 // (Servo) input pin
#define LCD_SCL 16 // (Display) Serial Clock synchronization
#define LCD_SDA 2 // (Display) Serial Data


const int BUFFER_SIZE = 14;
const int DATA_TAG_SIZE = 8; 

SoftwareSerial ssrfid = SoftwareSerial(RST_PIN, SS_PIN); 
LiquidCrystal_I2C lcd(0x27, LCD_SCL, LCD_SDA);
Servo s1;

int accepted_tags[2] = {52865, // blå
                        41721}; // grøn
String names[2] = {"Anders", "Jonatan"};


uint8_t buffer[BUFFER_SIZE];
int buffer_index = 0;

void setup() {
 Serial.begin(9600); 

  // SERVO
 s1.attach(9);
 s1.write(180);

 // RFID
 ssrfid.begin(9600);
 ssrfid.listen(); 

 // LCD
 lcd.begin();
 lcd.backlight();

  s1.detach();
  
 Serial.println("Programmet er indlæst");

  lcd.setCursor(2, 0);
  lcd.print("Kort er klar");
}

bool opening = false;
void open_door() {
  if(!opening) {
    s1.attach(9);
    s1.write(0);  
  }
}

void loop() {
  if(Serial.available() > 0) {
    String msg = Serial.readString();
    Serial.println(msg);
    for(int i = 0; i < 2; i++) {
      if(names[i] == msg) {
        Serial.print("Velkommen hjem, ");
        Serial.print(names[i] + "\n");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Velkommen Hjem");
        lcd.setCursor(0, 1);
        lcd.print(names[i]);
        open_door();
      } 
    }
  }
  
  if (ssrfid.available() > 0 && !opening){
    bool call_extract_tag = false;
    
    int ssvalue = ssrfid.read();
    if (ssvalue == -1) { // -1 = ingen data
      return;
    }

    if (ssvalue == 2) { // fundet tag => reset buffer 
      buffer_index = 0;
    } else if (ssvalue == 3) { // alt data er extracted    
      call_extract_tag = true;
    }

    if (buffer_index >= BUFFER_SIZE) {
      Serial.println("Fejl: Buffer overflow!");
      return;
    }
    
    buffer[buffer_index++] = ssvalue; 
    
    bool accepted = false;
    if (call_extract_tag == true) {
      if (buffer_index == BUFFER_SIZE) {
        unsigned tag = extract_tag();
        for(int i = 0; i < 2; i++) {
          if(accepted_tags[i] == tag) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Kort Verificeret");
            lcd.setCursor(1, 1);
            lcd.print("Velkommen hjem");
            open_door();
            accepted = true;
            
          }
        }
        if(!(accepted)) {
          lcd.clear();
          lcd.setCursor(6, 0);
          lcd.print("Fejl");
          lcd.setCursor(2, 1);
          lcd.print("Fremmed kort");
        }
      } else {
        buffer_index = 0;
        return;
      }
    }    
  }    
}

unsigned extract_tag() {
    uint8_t msg_head = buffer[0];
    uint8_t *msg_data = buffer + 1; // 10 bytes (2 byte version og 8 byte tag)
    uint8_t *msg_data_tag = msg_data + 2;

    // skriv besked fra tag 
    Serial.println("--------");
    long tag = hexstr_to_value(msg_data_tag, DATA_TAG_SIZE);
    Serial.print("Extracted Tag: ");
    Serial.println(tag);
    Serial.println("");
    Serial.println("--------");

    return tag;
}

long hexstr_to_value(char *str, unsigned int length) {
  char* c = malloc((sizeof(char) * length) + 1); 
  memcpy(c, str, sizeof(char) * length);
  c[length] = '\0';
  long val = strtol(c, NULL, 16);  
  free(c); 
  return val;
}
