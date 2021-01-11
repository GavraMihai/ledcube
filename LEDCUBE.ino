#include <LedCube.h>
#include <newfix.h>                                                                          // Import the LedCube library
#include <IRremote.h>
#include <ir_Lego_PF_BitStreamEncoder.h>

#define SIZE 5               // Define the size of the CUBE
#define COLS (SIZE*SIZE)     // Define the columns number

int receiver = 13;
IRrecv irrecv(receiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'

byte levelPins[SIZE] = {A5,A4,A3,A2,A1};                                                   // Level pins
byte colPins[COLS] = {12,11,10,9,8,7,6,5,4,3,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36};// Column pins

LedCube cube(SIZE, levelPins, colPins);  

int selectAnimation = 0;
int animationSpeed = 1;

void setup() {
  irrecv.enableIRIn(); // Start the receiver
}

void loop() { 
  if (irrecv.decode(&results)) // have we received an IR signal?
  {
    translateIR(); 
    irrecv.resume(); // receive the next value
  } 
  switch(selectAnimation)
  {
  case 1: CubeFlick(); break;
  case 2: turnOnRowByRow();    break;
  case 3: turnOnColByCol();    break;
  case 4: turnOnRandomRowOrCol();   break;
  case 5: lightRandomLed(); break;
  case 6: ledWave2();    break;
  case 7: ledWave();    break;
  case 8: rainLeds();    break;
  case 9: waterDrop();    break;
  case 10: cubeInCubeOut();    break; 
  case 11: selectRandomAnimation(); break;
  } 
}

void translateIR() // takes action based on IR code received
{
  switch(results.value)
  {
  case 0xFFA25D: selectAnimation = 1;   break; //1
  case 0xFF629D: selectAnimation = 2;   break; //2
  case 0xFFE21D: selectAnimation = 3;   break; //3
  case 0xFF22DD: selectAnimation = 4;   break; //4
  case 0xFF02FD: selectAnimation = 5;   break; //5
  case 0xFFC23D: selectAnimation = 6;   break; //6
  case 0xFFE01F: selectAnimation = 7;   break; //7
  case 0xFFA857: selectAnimation = 8;   break; //8
  case 0xFF906F: selectAnimation = 9;   break; //9
  case 0xFF9867: selectAnimation = 10;  break; //0
  case 0xFF6897: selectAnimation = 0; turnEveryLedOn();    break; //*
  case 0xFFB04F: selectAnimation = 0; turnEveryLedOff();    break; //#
  case 0xFF18E7: if(animationSpeed < 2) animationSpeed++;    break; //up
  case 0xFF4AB5: if(animationSpeed > 0) animationSpeed--;    break; //down
  case 0xFF10EF: if(selectAnimation >1) selectAnimation--;    break;  //left
  case 0xFF5AA5: if(selectAnimation <11) selectAnimation++;    break;  //right
  case 0xFF38C7: selectAnimation = 11;    break;  //ok
  }
  delay(500); // Do not get immediate repeat
}


void selectRandomAnimation(){
  switch(random(10))
  {
  case 0: for(int i=0; i<=4; i++) CubeFlick(); break;
  case 1: for(int i=0; i<=5; i++) turnOnRowByRow();    break;
  case 2: for(int i=0; i<=6; i++) turnOnColByCol();    break;
  case 3: for(int i=0; i<=10; i++) turnOnRandomRowOrCol();   break;
  case 4: for(int i=0; i<=20; i++) lightRandomLed(); break;
  case 5: for(int i=0; i<=5; i++) ledWave2();    break;
  case 6: for(int i=0; i<=5; i++) ledWave();    break;
  case 7: for(int i=0; i<=10; i++) rainLeds();    break;
  case 8: for(int i=0; i<=5; i++) waterDrop();    break;
  case 9: for(int i=0; i<=10; i++) cubeInCubeOut();    break; 
  }
}

void turnEveryLedOn(){
  for(int i=0;i<25;i++)
  {
    for(int j=0;j<5;j++)
    {
      cube.light(j,i,HIGH);
    }
  }
}

void turnEveryLedOff(){
  for(int i=0;i<25;i++)
    {
      for(int j=0;j<5;j++)
      {
        cube.light(j,i,LOW);
      }
    }
}

void CubeFlick(){
  turnEveryLedOn();
  delay(200 + animationSpeed*500);
  turnEveryLedOff();
  delay(200 + animationSpeed*500);
}

void turnOnRowByRow(){
  for(int i=0; i<25; i++)
    {
      for(int j=0; j<5; j++)
      {
        cube.light(j, i, HIGH); 
        delay(5 + 5*animationSpeed);
      }
    }

  turnEveryLedOff();

    for(int i=24; i>=0; i--)
  {
    for(int j=0; j<5; j++)
    {
      cube.light(j, i, HIGH);
      delay(5 + 5*animationSpeed);
    }
  }

  turnEveryLedOff();
}

void turnOnColByCol(){
  for(int j=0; j<5; j++)
    {
      for(int i=0; i<25; i++)
      {
        cube.light(j, i, HIGH); 
      }
      delay(50 + 50*animationSpeed);
    }

  turnEveryLedOff();

    for(int j=4; j>=0; j--)
  {
    for(int i=0; i<25; i++)
    {
      cube.light(j, i, HIGH);
      
    }
    delay(50 + 50*animationSpeed);
  }

  turnEveryLedOff();
  
}

void turnOnRandomRowOrCol(){
  if(random(2) == 1){
    int randomCol = random(5);
    for(int i=0; i<25; i++)
      cube.light(randomCol, i, HIGH);
  }
  else{
    int randomRow = random(25);
    for(int j=0; j<5; j++)
      cube.light(j, randomRow, HIGH);
  }
  delay(100 + animationSpeed*100);
  turnEveryLedOff();
}

void lightRandomLed(){
  int randomAnod = random(25);
  int randomCatod = random(5);
  
  cube.light(randomCatod, randomAnod, HIGH);
  delay(50 + animationSpeed*50);
  cube.light(randomCatod, randomAnod, LOW);
}

void ledWave2(){ 
  byte a1 [] = {2,0, 2,1, 2,2, 2,3, 2,4, 2,5, 2,9, 2,10, 2,14, 2,15, 2,19, 2,20, 2,21, 2,22, 2,23, 2,24}; 
  byte a2 [] = {2,6, 2,7, 2,8, 2,11, 2,13, 2,16, 2,17, 2,18};
  byte a3 [] = {3,6, 3,7, 3,8, 3,11, 3,13, 3,16, 3,17, 3,18};
  byte a4 [] = {1,6, 1,7, 1,8, 1,11, 1,13, 1,16, 1,17, 1,18};

  for(int i=0; i<animationSpeed+1; i++){
    cube.lightSequence(a1, sizeof(a1), 1);
    cube.lightSequence(a2, sizeof(a2), 1);
    cube.light(2, 12, HIGH);
    delay(5);
    cube.light(2, 12, LOW);
  }

  for(int i=0; i<animationSpeed+1; i++){
    cube.lightSequence(a1, sizeof(a1), 1);
    cube.lightSequence(a2, sizeof(a2), 1);
    cube.light(3, 12, HIGH);
    delay(5);
    cube.light(3, 12, LOW);
  }

  for(int i=0; i<animationSpeed+1; i++){
    cube.lightSequence(a1, sizeof(a1), 1);
    cube.lightSequence(a3, sizeof(a3), 1);
    cube.light(3, 12, HIGH);
    delay(5);
    cube.light(3, 12, LOW);
  }

  for(int i=0; i<animationSpeed+1; i++){
    cube.lightSequence(a1, sizeof(a1), 1);
    cube.lightSequence(a3, sizeof(a3), 1);
    cube.light(4, 12, HIGH);
    delay(5);
    cube.light(4, 12, LOW);
  }

  for(int i=0; i<animationSpeed+1; i++){
    cube.lightSequence(a1, sizeof(a1), 1);
    cube.lightSequence(a3, sizeof(a3), 1);
    cube.light(3, 12, HIGH);
    delay(5);
    cube.light(3, 12, LOW);
  }

  for(int i=0; i<animationSpeed+1; i++){
    cube.lightSequence(a1, sizeof(a1), 1);
    cube.lightSequence(a2, sizeof(a2), 1);
    cube.light(3, 12, HIGH);
    delay(5);
    cube.light(3, 12, LOW);
  }

  for(int i=0; i<animationSpeed+1; i++){
    cube.lightSequence(a1, sizeof(a1), 1);
    cube.lightSequence(a2, sizeof(a2), 1);
    cube.light(2, 12, HIGH);
    delay(5);
    cube.light(2, 12, LOW);
  }

  for(int i=0; i<animationSpeed+1; i++){
    cube.lightSequence(a1, sizeof(a1), 1);
    cube.lightSequence(a2, sizeof(a2), 1);
    cube.light(1, 12, HIGH);
    delay(5);
    cube.light(1, 12, LOW);
  }

  for(int i=0; i<animationSpeed+1; i++){
    cube.lightSequence(a1, sizeof(a1), 1);
    cube.lightSequence(a4, sizeof(a3), 1);
    cube.light(1, 12, HIGH);
    delay(5);
    cube.light(1, 12, LOW);
  }

  for(int i=0; i<animationSpeed+1; i++){
    cube.lightSequence(a1, sizeof(a1), 1);
    cube.lightSequence(a4, sizeof(a3), 1);
    cube.light(0, 12, HIGH);
    delay(5);
    cube.light(0, 12, LOW);
  }

  for(int i=0; i<animationSpeed+1; i++){
    cube.lightSequence(a1, sizeof(a1), 1);
    cube.lightSequence(a4, sizeof(a3), 1);
    cube.light(1, 12, HIGH);
    delay(5);
    cube.light(1, 12, LOW);
  }

  for(int i=0; i<animationSpeed+1; i++){
    cube.lightSequence(a1, sizeof(a1), 1);
    cube.lightSequence(a2, sizeof(a2), 1);
    cube.light(1, 12, HIGH);
    delay(5);
    cube.light(1, 12, LOW);
  }
}

void ledWave(){
  byte a1 [] = {0,0, 0,1, 0,2, 0,3, 0,4}; 
  byte a2 [] = {1,5, 1,6, 1,7, 1,8, 1,9}; 
  byte a3 [] = {2,10, 2,11, 2,12, 2,13, 2,14}; 
  byte a4 [] = {3,15, 3,16, 3,17, 3,18, 3,19};
  byte a5 [] = {4,20, 4,21, 4,22, 4,23, 4,24}; 

  byte b1 [] = {1,0, 1,1, 1,2, 1,3, 1,4}; 
  byte b5 [] = {3,20, 3,21, 3,22, 3,23, 3,24}; 

  byte c2 [] = {2,5, 2,6, 2,7, 2,8, 2,9}; 
  byte c4 [] = {2,15, 2,16, 2,17, 2,18, 2,19};

  byte d1 [] = {2,0, 2,1, 2,2, 2,3, 2,4}; 
  byte d5 [] = {2,20, 2,21, 2,22, 2,23, 2,24}; 

  byte f1 [] = {3,0, 3,1, 3,2, 3,3, 3,4}; 
  byte f5 [] = {1,20, 1,21, 1,22, 1,23, 1,24}; 

  byte g2 [] = {3,5, 3,6, 3,7, 3,8, 3,9}; 
  byte g4 [] = {1,15, 1,16, 1,17, 1,18, 1,19};

  byte h1 [] = {4,0, 4,1, 4,2, 4,3, 4,4}; 
  byte h5 [] = {0,20, 0,21, 0,22, 0,23, 0,24};
  

  for(int i=0; i< animationSpeed*2+1; i++){
    cube.lightSequence(a1, sizeof(a1), 1);
    cube.lightSequence(a2, sizeof(a1), 1);
    cube.lightSequence(a3, sizeof(a1), 1);
    cube.lightSequence(a4, sizeof(a1), 1);
    cube.lightSequence(a5, sizeof(a1), 1);  
  }

  for(int i=0; i<animationSpeed*2+1; i++){
    cube.lightSequence(b1, sizeof(a1), 1);
    cube.lightSequence(a2, sizeof(a1), 1);
    cube.lightSequence(a3, sizeof(a1), 1);
    cube.lightSequence(a4, sizeof(a1), 1);
    cube.lightSequence(b5, sizeof(a1), 1);  
  }

  for(int i=0; i<animationSpeed*2+1; i++){
    cube.lightSequence(b1, sizeof(a1), 1);
    cube.lightSequence(c2, sizeof(a1), 1);
    cube.lightSequence(a3, sizeof(a1), 1);
    cube.lightSequence(c4, sizeof(a1), 1);
    cube.lightSequence(b5, sizeof(a1), 1);  
  }

  for(int i=0; i<animationSpeed*2+1; i++){
    cube.lightSequence(d1, sizeof(a1), 1);
    cube.lightSequence(c2, sizeof(a1), 1);
    cube.lightSequence(a3, sizeof(a1), 1);
    cube.lightSequence(c4, sizeof(a1), 1);
    cube.lightSequence(d5, sizeof(a1), 1);  
  }

  for(int i=0; i<animationSpeed*2+1; i++){
    cube.lightSequence(f1, sizeof(a1), 1);
    cube.lightSequence(c2, sizeof(a1), 1);
    cube.lightSequence(a3, sizeof(a1), 1);
    cube.lightSequence(c4, sizeof(a1), 1);
    cube.lightSequence(f5, sizeof(a1), 1);  
  }

  for(int i=0; i<animationSpeed*2+1; i++){
    cube.lightSequence(f1, sizeof(a1), 1);
    cube.lightSequence(g2, sizeof(a1), 1);
    cube.lightSequence(a3, sizeof(a1), 1);
    cube.lightSequence(g4, sizeof(a1), 1);
    cube.lightSequence(f5, sizeof(a1), 1);  
  }

  for(int i=0; i<animationSpeed*2+1; i++){
    cube.lightSequence(h1, sizeof(a1), 1);
    cube.lightSequence(g2, sizeof(a1), 1);
    cube.lightSequence(a3, sizeof(a1), 1);
    cube.lightSequence(g4, sizeof(a1), 1);
    cube.lightSequence(h5, sizeof(a1), 1);  
  } 

  for(int i=0; i<animationSpeed*2+1; i++){
    cube.lightSequence(f1, sizeof(a1), 1);
    cube.lightSequence(g2, sizeof(a1), 1);
    cube.lightSequence(a3, sizeof(a1), 1);
    cube.lightSequence(g4, sizeof(a1), 1);
    cube.lightSequence(f5, sizeof(a1), 1);  
  }

  for(int i=0; i<animationSpeed*2+1; i++){
    cube.lightSequence(f1, sizeof(a1), 1);
    cube.lightSequence(c2, sizeof(a1), 1);
    cube.lightSequence(a3, sizeof(a1), 1);
    cube.lightSequence(c4, sizeof(a1), 1);
    cube.lightSequence(f5, sizeof(a1), 1);  
  }
  
  for(int i=0; i<animationSpeed*2+1; i++){
    cube.lightSequence(d1, sizeof(a1), 1);
    cube.lightSequence(c2, sizeof(a1), 1);
    cube.lightSequence(a3, sizeof(a1), 1);
    cube.lightSequence(c4, sizeof(a1), 1);
    cube.lightSequence(d5, sizeof(a1), 1);  
  }

  for(int i=0; i<animationSpeed*2+1; i++){
    cube.lightSequence(b1, sizeof(a1), 1);
    cube.lightSequence(c2, sizeof(a1), 1);
    cube.lightSequence(a3, sizeof(a1), 1);
    cube.lightSequence(c4, sizeof(a1), 1);
    cube.lightSequence(b5, sizeof(a1), 1);  
  }

  for(int i=0; i<animationSpeed*2+1; i++){
    cube.lightSequence(b1, sizeof(a1), 1);
    cube.lightSequence(a2, sizeof(a1), 1);
    cube.lightSequence(a3, sizeof(a1), 1);
    cube.lightSequence(a4, sizeof(a1), 1);
    cube.lightSequence(b5, sizeof(a1), 1);  
  } 
}

void rainLeds (){
int r1 = random(25);
int r2 = random(25);
int r3 = random(25);

for(int i = 6; i>=0; i--)
  {
  cube.light(i, r1, HIGH);
  cube.light(i-1, r2, HIGH);
  cube.light(i-2, r3, HIGH);
  delay(25 + 25*animationSpeed);
  cube.light(i, r1, LOW);
  cube.light(i-1, r2, LOW);
  cube.light(i-2, r3, LOW);
  }
}

void waterDrop (){
  byte a1 [] = {0,6, 0,7, 0,8, 0,11, 0,13, 0,16, 0,17, 0,18}; //first water circle;
  byte c2 [] = {0,0, 0,1, 0,2, 0,3, 0,4, 0,5, 0,9, 0,10, 0,14, 0,15, 0,19, 0,20, 0,21, 0,22, 0,23, 0,24}; //second water circle;
  cube.light(4, 12, HIGH);
  delay(50+50*animationSpeed);
  cube.light(4, 12, LOW); 
  cube.light(3, 12, HIGH);
  delay(50+50*animationSpeed);
  cube.light(3, 12, LOW);
  cube.light(2, 12, HIGH);
  delay(50+50*animationSpeed);
  cube.light(2, 12, LOW);
  cube.light(1, 12, HIGH);
  delay(50+50*animationSpeed);
  cube.light(1, 12, LOW);
  cube.light(0, 12, HIGH);
  delay(40+40*animationSpeed);
  cube.light(0, 12, LOW);
  cube.lightSequence(a1, sizeof(a1), 15);
  cube.lightSequence(c2, sizeof(c2), 10);
  delay(100+100*animationSpeed);
}
 
void cubeInCubeOut () {
  byte a1 [] = {0,0, 0,1, 0,2, 0,3, 0,4, 0,5, 0,9, 0,10, 0,14, 0,15, 0,19, 0,20, 0,21, 0,22, 0,23, 0,24, 
  4,0, 4,1, 4,2, 4,3, 4,4, 4,5, 4,9, 4,10, 4,14, 4,15, 4,19, 4,20, 4,21, 4,22, 4,23, 4,24, 
  1,0, 1,4, 2,0, 2,4, 3,0, 3,4, 1,20, 1,24, 2,20, 2,24, 3,20, 3,24}; 
  byte a2 [] = {1,6, 1,7, 1,8, 1,11, 1,13, 1,16, 1,17, 1,18,
  3,6, 3,7, 3,8, 3,11, 3,13, 3,16, 3,17, 3,18, 2,6, 2,8, 2,16, 2,18};
  
  cube.lightSequence(a1, sizeof(a1), 2+animationSpeed);
  cube.lightSequence(a2, sizeof(a2), 5+animationSpeed);
  cube.light(2,12,HIGH);
  delay(50+50*animationSpeed);
  cube.light(2,12,LOW); 
  cube.lightSequence(a2, sizeof(a2), 5+animationSpeed);
  
}
