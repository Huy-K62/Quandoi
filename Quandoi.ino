#include <TimerOne.h> 
//#include <u8glib.h>
#include "U8glib.h"
//#include <Arduino_FreeRTOS.h>
#include <EEPROM.h>
U8GLIB_ST7920_128X64_4X u8g(5, 6, 7, 8, 9, 10, 11, 12, 4, 2, 3);
unsigned long waitTime = 200;
boolean buttonLongPress = 0;
boolean lastButtonStatus = 1;
boolean flag_press = 0;
unsigned long lastChangedTime;
unsigned short value_channel = 0;
uint8_t flag = 0, flag_number = 0, flag_fre = 0, flag_key_pm = 0, 
flag_PM = 0, flag_key_v = 0, flag_sp = 0, flag_key_sp = 0, check_speak = 0,
flag_signal = 0, flag_blink = 0, blink_position = 0, _channel;
int ledState1 = LOW, ledState2 = LOW, ledState3 = LOW, ledState4 = LOW, 
ledState5 = LOW, ledState6 = LOW, value_txrx;
int fre[5] = {};
int _blink[5][2] = {{34,24},{44,24},{64,24},{74,24},{84,24}};
String frequency, vol, pm, speaker, channel;
char* PM[] = {"LP", "HP", "MP"};
char* Speaker[] =  {"O", "T", "Q"};

const uint8_t rook_bitmap[] U8G_PROGMEM = {
  0x02,          
  0x06,       
  0x0a,         
  0x72,        
  0x62,
  0x62,        
  0x62,
  0x62,      
  0x72,         
  0x0a,           
  0x06,
  0x02
};

const uint8_t rook_bitmap_s[] U8G_PROGMEM = {
  0x00,         
  0x03,         
  0x00,         
  0x03,         
  0x00,
  0x03,         
  0x03,
  0x00,         
  0x03,         
  0x00,
  0x03,
  0x00
};

const uint8_t rook_bitmap_x[] U8G_PROGMEM = {
  0x00,      
  0x80,        
  0x40,       
  0x21,         
  0x12,
  0x0c,        
  0x0c,
  0x12,
  0x21,
  0x40,
  0x80,
  0x00
};

void writeString(char add,String data)
{
  int _size = data.length();
  int i;
  for(i=0;i<_size;i++)
  {
    EEPROM.write(add+i,data[i]);
  }
  EEPROM.write(add+_size,'\0');
}

String read_String(char add)
{
  int i;
  char data[100]; //Max 100 Bytes
  int len=0;
  unsigned char k;
  k=EEPROM.read(add);
  while(k != '\0' && len<500)   //Read until null character
  {    
    k=EEPROM.read(add+len);
    data[len]=k;
    len++;
  }
  data[len]='\0';
  return String(data);
}

byte buttonPressed()
{
  for(int i=22; i<37; i++)
  {
    if(digitalRead(i))
    {
      while(digitalRead(i))
      {
        return i;
      }
    }
  }
  return 0;
}

void check_number(int _position, int x, int y)
{ 
  byte key = buttonPressed();
  boolean reading = digitalRead(key);
  if (reading != lastButtonStatus){  
    lastButtonStatus = reading;
    lastChangedTime = millis();
  }
  if(millis() - lastChangedTime > waitTime)
  {
    buttonLongPress = reading;
    lastChangedTime = millis();
  }
  if(buttonLongPress == true) {
    _position = _position % 5;
    blink_position = _position; 
    switch(key)
    {
      case 22:
        fre[_position] = 0;
        flag_number = 1;
        flag_fre++;
        break; 
      case 23:
       // Serial.println(key);
        fre[_position] = 1;
        flag_number = 1;
        flag_fre++;
        break; 
      case 24:
        //Serial.println(key);
        fre[_position] = 2;
        flag_number = 1;
        flag_fre++;
        break; 
      case 25:
        fre[_position] = 3;
        flag_number = 1;
        flag_fre++;
        break; 
      case 26:
        fre[_position] = 4;
        flag_number = 1;
        flag_fre++;
        break; 
      case 27:
        fre[_position] = 5;
        flag_number = 1;
        flag_fre++;
        break; 
      case 28:
        fre[_position] = 6;
        flag_number = 1;
        flag_fre++;
        break; 
      case 29:
        fre[_position] = 7;
        flag_number = 1;
        flag_fre++;
        break; 
      case 30:
        fre[_position] = 8;
        flag_number = 1;
        flag_fre++;
        break; 
      case 31:
        fre[_position] = 9;
        flag_number = 1;
        flag_fre++;
        break; 
    } 
    //flag_press = !flag_press; 
    buttonLongPress = false;
  }
}

void check_yellow_function(void)
{
  byte key = buttonPressed();
  boolean reading = digitalRead(key);
  if (reading != lastButtonStatus){  
    lastButtonStatus = reading;
    lastChangedTime = millis();
  }
  if(millis() - lastChangedTime > waitTime)
  {
    buttonLongPress = reading;
    lastChangedTime = millis();
  }
  if(buttonLongPress == true) {
    switch(key)
    {
      case 22:
        flag_number = 2;
        //flag_key = 1;
        break; 
      case 24:
        flag_number = 3;
        //flag_key = 1;
        break; 
      case 29:
        flag_number = 4;
        //flag_key = 1;
        break; 
    } 
    buttonLongPress = false;
  }  
}
void TasksetFRQ(void)
{
  byte key = buttonPressed();
  switch(key)
  {
    case 32:
      flag = 1;
      flag_key_v = 1;
      flag_key_pm = 1;
      flag_key_sp = 1;
      flag_number = 0;
      flag_signal = 1;
      break;
    case 33:
      flag = 2;
      break;
    case 34:
      flag = 3;
      break;
    case 35:
      flag = 4;
      break;
    case 36:
      flag = 5;
      break;
  }          
}
void draw_fre(void)
{
  String data = read_String(10);
  frequency = data;
  if(flag_number == 1)
    frequency = String(fre[0]) +String(fre[1]) + "." + String(fre[2]) +String(fre[3]) +String(fre[4]) + "MHz";  
  u8g.setFont(u8g_font_10x20);
  u8g.setPrintPos(33, 37); 
  u8g.print(frequency);
}

void draw_PM(void)
{
  flag_PM = flag_PM % 3;
  //Serial.println(flag_PM);
  pm = PM[flag_PM];
  u8g.setFont(u8g_font_6x12);
  u8g.setPrintPos(112, 61); 
  u8g.print(pm);
}

void draw_speak(void)
{
  flag_sp = flag_sp % 3;
  if(flag_sp == 0 && check_speak == 0)
  {
    u8g.drawBitmapP(3, 50, 1, 12, rook_bitmap);
    u8g.drawBitmapP(5, 50, 1, 12, rook_bitmap_s);
  }
  
  else
  {
    u8g.drawBitmapP(3, 50, 1, 12, rook_bitmap);
    u8g.drawBitmapP(3, 50, 1, 12, rook_bitmap_x);
  }
  speaker = Speaker[ flag_sp];
  u8g.setFont(u8g_font_6x12);
  u8g.setPrintPos(19, 61); 
  u8g.print(speaker);
}

void draw_channel(void)
{
  _channel = map(value_channel,0,1023,1,10); 
  //Serial.println(_channel);
  channel = "CH0" + String(_channel); 
  u8g.setFont(u8g_font_6x12);
  u8g.setPrintPos(2, 18); 
  u8g.print(channel);
}

void draw_txrx()
{
  if(check_speak == 1)
  {
    u8g.setFont(u8g_font_6x12);
    u8g.drawBitmapP(3, 50, 1, 12, rook_bitmap);
    u8g.drawBitmapP(3, 50, 1, 12, rook_bitmap_x);
    u8g.drawStr(34, 61, "TX");
  }
   if(check_speak == 0)
  {
    u8g.setFont(u8g_font_6x12);
    u8g.drawStr(34, 61, "RX"); 
  }
}

void blinkLED(void)
{
  if (ledState1 == LOW) {
    ledState1 = HIGH;
  } else {
    ledState1 = LOW;
  }
  
  if (ledState2 == LOW) {
    ledState2 = HIGH;
  } else {
    ledState2 = LOW;
  }
  if (ledState3 == LOW) {
    ledState3 = HIGH;
  } else {
    ledState3 = LOW;
  }
  
  if (ledState4 == LOW) {
    ledState4 = HIGH;
  } else {
    ledState4 = LOW;
  }
  
  if (ledState5 == LOW) {
    ledState5 = HIGH;
  } else {
    ledState5 = LOW;
  }
  
  if (ledState6 == LOW) {
    ledState6 = HIGH;
  } else {
    ledState6 = LOW;
  }
  if(value_txrx == HIGH)
  {
    Serial.println("huy");
    check_speak = 1;
    digitalWrite(40, ledState1);
    digitalWrite(48, ledState2);
    digitalWrite(42, ledState3);
    digitalWrite(44, LOW);
    digitalWrite(38, LOW);
    digitalWrite(46, LOW);
  }
  else
  {
    check_speak = 0;
    digitalWrite(44, ledState4);
    digitalWrite(38, ledState5);
    digitalWrite(46, ledState6);
    digitalWrite(40, LOW);
    digitalWrite(48, LOW);
    digitalWrite(42, LOW);
  }
  
}

void draw()
{
  draw_fre(); 
  draw_PM();
  draw_speak();
  draw_channel();
  draw_txrx();
  u8g.setFont(u8g_font_6x12);
  u8g.drawStr(2, 9, "FIX");
  u8g.drawStr(40, 9, "CLEAR");
  u8g.drawStr(95, 9, "VOICE");
  u8g.drawFrame(0,0,128,64);
  TasksetFRQ();
  if(flag == 3)
  {
    u8g.setFont(u8g_font_6x12);
    u8g.setPrintPos(57, 61); 
    u8g.print("FRESET");
    u8g.drawBox(_blink[blink_position][0],_blink[blink_position][1],8,13);
    check_number(flag_fre,36,24);
  }
  if(flag == 2)
  {
    Serial.println("Save FRQ");
    writeString(10, frequency); 
  }
  if(flag == 1)
  {
    if(flag_signal == 1)
    {
       u8g.drawBox(103, 54,5,7);
    }
    check_yellow_function();
    if(flag_number == 3 && flag_key_v == 1)
    {
      vol = String(16)+"."+String(25)+String("V");
      u8g.setFont(u8g_font_6x12);
      u8g.drawStr(58, 61, vol.c_str());
      flag_key_pm = 0;
      flag_key_sp = 0;
      flag_signal = 0; 
    }
    if(flag_number == 4 && flag_key_pm == 1)
    {
      flag_PM++ ;
      flag_key_v = 0;
      flag_key_pm = 0;
      flag_key_sp = 0;
      flag_signal = 0;
    }
    if(flag_number == 2 && flag_key_sp == 1)
    {
      flag_sp++ ;
      flag_key_v = 0;
      flag_key_pm = 0;
      flag_key_sp = 0;
      flag_signal = 0;
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A3, INPUT);
  pinMode(A0, INPUT);
  pinMode(37, OUTPUT);
  digitalWrite(37, HIGH);
  for(int i=22; i<37; i++)
  {
    pinMode(i, INPUT);
  }
  for(int i=38; i<50; i+=2)
  {
    pinMode(i, OUTPUT);
  }
  attachInterrupt(digitalPinToInterrupt(18),buttonPressed,FALLING);
  Timer1.initialize(500000); 
  Timer1.attachInterrupt(blinkLED);
}

void loop() {
  // picture loop
  u8g.firstPage();  
  do {
    value_txrx = digitalRead(A0);
    value_channel = analogRead(A3);
    draw();
  } while( u8g.nextPage() );
}
