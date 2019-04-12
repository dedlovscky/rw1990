#include <OneWire.h>

#define pin 10

OneWire ibutton (pin); // Подключаем ключ к 10 пину дуни

byte addr[8]; //Буфер, хранящий в себе ид ключа

void setup(){
 Serial.begin(115200); 
}

void loop(){
  if(ibutton.search (addr))//Если устройство найдено
  {
    readOrWriteKey();
  }
  else
  {
    ibutton.reset_search();
    delay(200);
  }
} 

void readOrWriteKey(){
  Serial.print(millis()/1000);
  Serial.print("> ");
  
  for (byte i = 0; i < 8; i++){  
    Serial.print(addr[i], HEX); //Печатаем содержимое буфера в консоль 
    Serial.print(" ");
  }
  
  Serial.print("CRC: ");
  Serial.println(ibutton.crc8(addr, 7), HEX);
  
  if ( Serial.read() == 'w' ){
    ibutton.skip();
    ibutton.reset();
    ibutton.write(0x33);
    
    // send reset
    ibutton.skip();
    ibutton.reset();
    
    // send 0xD1
    ibutton.write(0xD1);
    
    // send logical 0
    digitalWrite(10, LOW); 
    pinMode(10, OUTPUT); 
    delayMicroseconds(60);
    pinMode(10, INPUT); 
    digitalWrite(10, HIGH); 
    delay(10);
    
    // Хочу записать вот это ид 1:CF:E0:62:E:0:0:3
    byte newID[8] = {0x01, 0x72, 0xA5, 0x18, 0xA0, 0x00, 0x00, 0xF8};
                     
    ibutton.skip();
    ibutton.reset();
    ibutton.write(0xD5);

    Serial.println("Идет запись нового ключа");
    
    for (byte i = 0; i < 8; i++){
      writeByte(newID[i]);
      Serial.print('*');
    }
    
    Serial.println();
    ibutton.reset();
    
    // send 0xD1
    ibutton.write(0xD1);
    
    //send logical 1
    digitalWrite(10, LOW); 
    pinMode(10, OUTPUT); 
    delayMicroseconds(10);
    pinMode(10, INPUT); 
    digitalWrite(10, HIGH); 
    delay(10);
  }
}

int writeByte(byte data){
  for(byte i = 0; i < 8; i++){
    if (data & 1){
      digitalWrite(pin, LOW);
      pinMode(pin, OUTPUT);
      delayMicroseconds(60);
      pinMode(pin, INPUT);
      digitalWrite(pin, HIGH);
      delay(10);
    } else {
      digitalWrite(pin, LOW);
      pinMode(pin, OUTPUT);
      pinMode(pin, INPUT); 
      digitalWrite(pin, HIGH);
      delay(10);
    }
    data = data >> 1;
  }
  
  return 0;
}
