#include <Event.h>
#include <Timer.h>

//Define integer variables for input and output pins.

int ir_diag_out = 7;
int ir_side_out = 4;
int ir_front_out = 8;

int front_r_in = A7;
int front_l_in = A6;
int side_r_in = A3;
int side_l_in = A2;
int diag_r_in = A1;
int diag_l_in = A0;

//Order is front, diagonal, side
int irArray[3][3]={
  {ir_front_out, front_r_in, front_l_in},
  {ir_diag_out,diag_r_in,diag_l_in},
  {ir_side_out, side_r_in, side_l_in}
};

//callibratedArray[i][0] => right sensor
//callibratedArray[i][0] => left sensor
int callibratedArray[3][2];

void setup()
{
  //Set intput and output modes for defined pins
    Serial.begin(9600);  //Begin serial communcation
    pinMode( ir_diag_out, OUTPUT );
    pinMode( ir_side_out, OUTPUT );
    pinMode( ir_front_out, OUTPUT );
 
    pinMode( front_r_in, INPUT );
    pinMode( front_l_in, INPUT );
    pinMode( side_r_in, INPUT );
    pinMode( side_l_in, INPUT );
    pinMode( diag_r_in, INPUT );
    pinMode( diag_l_in, INPUT );
    
    //Populate the table of sensor averages
    callibrateSensors();
}

//Given an input of an LED pin and a receiver pin, get the average receiver value over 10 trials.
int getAverage(int ledPin, int recPin){
  int runningTally = 0;
  for (int x=0; x<=10; x++){
    runningTally += readSensor(ledPin, recPin);
  }
  int avgTally = runningTally/10;
  return avgTally;
}

void callibrateSensors(){
  for (int i = 0; i<3; i++){
    callibratedArray[3][0]=getAverage(irArray[i][0],irArray[i][1]);
    callibratedArray[3][1]=getAverage(irArray[i][0],irArray[i][2]);
  }
}

int readSensor(int ledPin, int recPin)
{
  digitalWrite(ledPin, HIGH);
  delayMicroseconds(80);
  int returnVal = analogRead(recPin);
  digitalWrite(ledPin,LOW);
  delayMicroseconds(80);
  
  return returnVal;
}

void loop()
{
  for (int i=0; i<3; i++){
    

    int sensorRight = readSensor(irArray[i][0],irArray[i][1]);
    int sensorLeft = readSensor(irArray[i][0],irArray[i][2]);
    
    if((sensorRight>callibratedArray[i][0]+10)||(sensorRight<callibratedArray[i][0]-10)){
      Serial.println(sensorRight);
    }
    
    if((sensorLeft>callibratedArray[i][1]+10)||(sensorLeft<callibratedArray[i][1]-10)){
      Serial.println(sensorLeft);
    }
    
    //int sensorRight = readSensor(ir_side_out, side_r_in);
    //Serial.println(sensorRight);
    
  }
}  

