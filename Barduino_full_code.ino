#include <Keypad.h>
#include <Regexp.h>


//first we start by defining the stepper motor control pin
const int stepPin = 10; //A3 pin stepper controls steps
const int dirPin = 13; //A2 pin stepper controls direction

// DC Motor A connections
int enA = 9;
int in1 = 8;
int in2 = 7;
// DC Motor B connections
int enB = 3;
int in3 = 5;
int in4 = 4;


//then here we define the information for the keypad
const byte ROWS = 2;
const byte COLS = 2; //we are using 2 rows and 2 columns

char hexaKeys[ROWS][COLS] = {

  {'4', '5'},
  {'7', '8'},

};

byte rowPins[ROWS] = { 12, 11}; //we are using the digits 4, 5,7 and 8 which are connected to the pins shown here
byte colPins[COLS] = {6, 2};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
MatchState ms;
//ms.Target ("4");


void setup() {
  //here in order to start the keypad we use serial begin:
  Serial.begin(9600);


  //  // Set all the DC motor control pins to outputs
   pinMode(enA, OUTPUT);
   pinMode(enB, OUTPUT);
   pinMode(in1, OUTPUT);
   pinMode(in2, OUTPUT);
   pinMode(in3, OUTPUT);
   pinMode(in4, OUTPUT);
  
   // Turn off DC motors - Initial state
   digitalWrite(in1, LOW);
   digitalWrite(in2, LOW);
   digitalWrite(in3, LOW);
   digitalWrite(in4, LOW);


  // here we set the two stepper motor pins as outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

}


void loop() {
  char customKey = customKeypad.getKey();
  //Here we initiate the keypad when a key is pressed
  //We store the 8 bit key pressed value to custom key  
  customKey = customKeypad.getKey();

 

  // 0: nothing
  // 1: wrong
  // 2: correct
  // we use a switching cases algorithm with 3 commands
  int command = 0;  //this is default command : it is neutral
  switch(customKey){
    case '7':    //we define the 4 different cases (4 possible digits pressed) and associate the command to it
      command = 2;
      break;
    case '4':
      command = 1;
      break;
    case '5':
      command = 1;
      break;
   case '8':
      command = 1;
      break;
    default:
      command = 0;
      break;
  }

  switch (command)
  {
    case 1: //this is for the wrong answer where the user will get water
      Serial.println("Wrong answer"); 
      moveMotor();
      delay(1000);
      directionControl();
      delay(1000); //delays are used to synchronise the movements between the different motors: the functions used here are explained below
      speedControl();
      delay(1000);
      moveMotorR(); // in this case the stepper motor will move to the right, the DC motor pump on the right will pour water in the cup  
      break;        // then the stepper will then move back to the left to get back to the central position
      
    case 2: // this is for the correct answer where the user will get a drink
      Serial.println("Correct answer");
      moveMotorDrink();
      delay(1000);
      directionControlB();
      delay(1000);
      SpeedControlB();
      delay(1000);
      moveMotor();
      break; //here is the opposite direction of case 1
  }


}

void moveMotor() { //this function is for the stepper motor to make it move to the right
  digitalWrite(dirPin, LOW); //Changes the rotations direction
  // Makes 400 pulses for making two full cycle rotation
  for (int x = 0; x < 250; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
  }
  // delay(3000);
}


void moveMotorR() { //this function is for the stepper motor to make it move to the left
  digitalWrite(dirPin, HIGH); //changes the rotations direction
  //makes 600 pulses (200*1.5) to make 1.5 full rotations which equals to 7.5mm
  for (int x = 0; x < 300; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
  }

  //  delay(1000);
}


void moveMotorDrink() { 
  digitalWrite(dirPin, HIGH); //changes the rotations direction
  //makes 600 pulses (200*1.5) to make 1.5 full rotations which equals to 7.5mm
  for (int x = 0; x < 350; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
  }

 
}

void directionControl() { //this function is for the first DC motor 
  // Sets DC motor to maximum speed
  // For PWM maximum possible values are 0 to 255
  Serial.println("Starting DC Motor");
  analogWrite(enA, 255);
  // Turn on DC motor A in one direction
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
     delay(10000);
Serial.println("Stopping DC Motor");
  // Turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);



}

void speedControl() //this function is needed to get the DC motor to work at full speed
{
  // Turn on motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  
  // Accelerate from zero to maximum speed
  for (int i = 0; i < 256; i++)
  {
    analogWrite(enA, i);
    delay(20);
  }
  // Decelerate from maximum speed to zero
  for (int i = 255; i >= 0; --i)
  {
    analogWrite(enA, i);
    delay(20);
  }
  
  // Now turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);

}

void directionControlB() //this function is for the other DC motor
{
  Serial.println("Starting DC Motor");
  analogWrite(enB, 255);
  // Turn on DC motor A in one direction
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
     delay(10000);
Serial.println("Stopping DC Motor");
  // Turn off motors
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

}

void SpeedControlB()
{
   digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  
  // Accelerate from zero to maximum speed
  for (int i = 0; i < 256; i++)
  {
    analogWrite(enB, i);
    delay(20);
  }
  // Decelerate from maximum speed to zero
  for (int i = 255; i >= 0; --i)
  {
    analogWrite(enB, i);
    delay(20);
  }
  
  // Now turn off motors
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
