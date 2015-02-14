#include "GA.h"
#include <math.h>

const int led_pin = 3;
const int photo_pin = A0;
//double input = 0,output = 0,setpoint = 120;

#define PMAX 9
unsigned char parameter[PMAX];
boolean flag = false;
boolean ga_run = false;
boolean pid_run = false;
unsigned int count = 0;

void CheckSum(){
  parameter[PMAX-1] = 0;
  for(int i = 0;i < PMAX-1;++i){
    parameter[PMAX-1] ^= parameter[i];
  }
}

void Paser()
{
  while(Serial.available()){
    flag = true;
    parameter[count++] = Serial.read();
  }
  if(flag && (count == PMAX)){
    flag = false;
    count = 0;
    switch (parameter[0])
    {
      case 'r':
      {
        unsigned char chk = parameter[PMAX-1];
        CheckSum();
        if(chk == parameter[PMAX-1]){
          unsigned int temp = 0;
          temp += (parameter[1] * 255);
          temp += (parameter[2]);
          ITERA = temp;
          MAX = parameter[3];
          CROSSOVER_RATE = parameter[4] * 0.001f;
          MUTATION_RATE = parameter[5] * 0.001f;
          temp = 0;
          temp += (parameter[6] * 255);
          temp += (parameter[7]);
          setpoint = temp/4;
          ga_run = true;
          //pid_run = true;
        }
        else{
          Serial.println("CheckSum Error");
        }
        break;
      }
      case 0x0:
      {
        unsigned char chk = parameter[PMAX-1];
        CheckSum();
        if(chk == parameter[PMAX-1]){
          for(int i = 1;i < PMAX-1;++i){
            if(parameter[i] == i){
              //Serial.print(parameter[i]);
              //Serial.print(" ");
              ga_run ^= 0x1;
            }
          }
        }        
        break;
      }
      default:
      {
        Serial.println("Command Not Find");
        break;
      }
    }
  }
}

void setup()
{
  pinMode(led_pin,OUTPUT);
  pinMode(4,OUTPUT);
  digitalWrite(4,LOW);
  Serial.begin(9600);
}
int c = 0;
void loop()
{
  Paser();
  while(ga_run){
    Paser();
    int i = 1;
    InitGene();
    FitnessFunc();
    Serial.print(i);
    Serial.print(" Iterate ");
    Serial.print(best_gene.DNA & 0x01F);
    Serial.print(" ");
    Serial.print((best_gene.DNA >> 5) & 0x01F);
    Serial.print(" ");
    Serial.println((best_gene.DNA >> 10) & 0x01F);
    for(i = 2;(i <= ITERA) && ga_run;++i){
      Paser();
      Crossover();
      Mutation();
      FitnessFunc();
      Serial.print(i);
      Serial.print(" Iterate ");
      Serial.print(best_gene.DNA & 0x01F);
      Serial.print(" ");
      Serial.print((best_gene.DNA >> 5) & 0x01F);
      Serial.print(" ");
      Serial.println((best_gene.DNA >> 10) & 0x01F);
    }
    ga_run = false;
  }
  /*
  input = kalman.Update(analogRead(photo_pin) / 4);
  pid.Computing();
  analogWrite(led_pin,output);

  //Serial.print(output);
  //Serial.print(" ");
  if(c != pid.count){
  Serial.println(input);
      c = pid.count;
  }
  */
}
