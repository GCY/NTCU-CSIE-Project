#include <WiServer.h>
#include <dht.h>

#define WIRELESS_MODE_INFRA	1
#define WIRELESS_MODE_ADHOC	2

unsigned char local_ip[] = {192,168,140,111};	// IP address of WiShield
unsigned char gateway_ip[] = { 192.168,1,1};	// router or gateway IP address
unsigned char subnet_mask[] = { 255,255,255,0};	// subnet mask for the local network
const prog_char ssid[] PROGMEM = { "GCY-OpenWrt"};		// max 32 bytes

unsigned char security_type = 3;	// 0 - open; 1 - WEP; 2 - WPA; 3 - WPA2

// WPA/WPA2 passphrase
const prog_char security_passphrase[] PROGMEM = { "rozen123"};	// max 64 characters

// WEP 128-bit keys
// sample HEX keys
prog_uchar wep_keys[] PROGMEM = { 
  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d,	// Key 0
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Key 1
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Key 2
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00	// Key 3
};

// setup the wireless mode
// infrastructure - connect to AP
// adhoc - connect to another WiFi device
unsigned char wireless_mode = WIRELESS_MODE_INFRA;

unsigned char ssid_len;
unsigned char security_passphrase_len;

const int PHOTORESISTOR_PIN = 0;
const int DHT11_PIN = 8;
const int PIR_PIN = 5;
const int LED_PIN = 6;
const int MOTOR_PIN = 7;

boolean pir_state = false;
unsigned long last_loop_time;
unsigned long delay_time = 15000;

boolean light_state = false;
boolean motor_state = false;

dht DHT11;

boolean Page(char *URL)
{
  if(!strcmp(URL,"/")){
    WiServer.println("CNP");
    
    return true;
  }
  else if(URL[1] == '?'){
    if(URL[2] == 'S'){
      int val = analogRead(PHOTORESISTOR_PIN);
      WiServer.println(val);
      
      DHT11.read11(DHT11_PIN);
      WiServer.println((double)DHT11.temperature,2);
      WiServer.println((double)DHT11.humidity,2);
      
      WiServer.println(pir_state);
      
      WiServer.println(light_state);
      WiServer.println(motor_state);
      
      return true;
    }
    else if(URL[2] == 'L'){
      if(URL[3] == '?'){
        if(URL[4] == 'N'){
          light_state = true;
        
          WiServer.println("Light ON");
          digitalWrite(LED_PIN,HIGH);
          
          return true;
        }
        else if(URL[4] == 'F'){
          light_state = false;
          
          WiServer.println("Light OFF");
          digitalWrite(LED_PIN,LOW);
          
          return true;
        }
      }
    }
    else if(URL[2] == 'M'){
      if(URL[3] == '?'){
        if(URL[4] == 'N'){
          motor_state = true;
          
          WiServer.println("Motor ON");
          digitalWrite(MOTOR_PIN,HIGH);
          
          return true;
        }
        else if(URL[4] == 'F'){
          motor_state = false;
          
          WiServer.println("Motor OFF");
          digitalWrite(MOTOR_PIN,LOW);
          
          return true;
        }
      }
    }
  }
  
  return false;
}

void PIR()
{
  boolean pir = digitalRead(PIR_PIN);
  
  if(((millis() - last_loop_time) > delay_time) && !pir){
    last_loop_time = millis();
    pir_state = true;
  }
  else if((millis() - last_loop_time) > delay_time){
    pir_state = false;
  }
}

void setup()
{
  pinMode(LED_PIN,OUTPUT);
  pinMode(MOTOR_PIN,OUTPUT);
  pinMode(PIR_PIN,INPUT);
  
  digitalWrite(LED_PIN,LOW);
  digitalWrite(MOTOR_PIN,LOW);

  WiServer.init(Page);
  
  last_loop_time = 0;
}

void loop()
{
  PIR();
  WiServer.server_task();
  
  delay(10);
}
