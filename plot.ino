// Motor 1 - X direction

const int stepPin1 = 6; //6
const int dirPin1 = 7; //7
const int enPin1 = 8; //8

// Motor 2 - Y direction

const int stepPin2 = 3; //3
const int dirPin2 = 4; //4
const int enPin2 = 5; //5

// Steps per revolution for the motors.
const int revTomm = 10;

// Setting the Servo Pins
#include <Servo.h>

Servo servo;

const int servoPin = 9;
const int servoAngle = 0;
const int servoUp = 45;
const int servoDown = 120;

// Getting user input without delay
const byte numChars = 32;
char receivedChars[numChars]; // an array to store the received data
boolean newData = false;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  // Setup the 2 motors and enable them.

  pinMode(enPin1, OUTPUT);
  pinMode(stepPin1, OUTPUT);
  pinMode(dirPin1, OUTPUT);
  digitalWrite(enPin1, LOW);

  pinMode(enPin2, OUTPUT);
  pinMode(stepPin2, OUTPUT);
  pinMode(dirPin2, OUTPUT);
  digitalWrite(enPin2, LOW);

  digitalWrite(dirPin1, HIGH);
  digitalWrite(dirPin2, HIGH);

  // MOVE the 2 motors 5 cm away;

  // Lift the Pen using the servo;
  servo.attach(servoPin);
  servo.write(servoAngle);
  delay(300);
  servo.write(servoUp);

  // All set. بسم الله
  Serial.println("<Arduino is ready>");
}

struct Coordinates
{
  int x;
  int y;
};


void loop() {
  Serial.println("How many coordinates do you want to enter? ");
  while (Serial.available() == 0)
  {}
  delay(100); // Let Arduino breathe
  recvWithEndMarker();
  int numbOfCoords;
  numbOfCoords = getUserInput();

  Serial.println(numbOfCoords); // delete later
  Coordinates arrayOfCords[numbOfCoords];

  // read the coordinates from the user
  for (int i = 0; i < numbOfCoords; ++i)
  {
    Serial.print("Enter the ");
    Serial.print(i + 1);
    Serial.println(" X coordinate");
    while (Serial.available() == 0)
    {}
    delay(100);
    recvWithEndMarker();
    arrayOfCords[i].x = getUserInput();

    Serial.println(arrayOfCords[i].x);

    Serial.print("Enter the ");
    Serial.print(i + 1);
    Serial.println(" Y coordinate");
    while (Serial.available() == 0)
    {}
    delay(100);
    recvWithEndMarker();
    arrayOfCords[i].y = getUserInput();
    Serial.println(arrayOfCords[i].y);

    Serial.println("");
  }


  // *Test*

  Serial.println("Your Coordinates are :");
  for ( int i = 0; i < numbOfCoords; ++i)
  {
    //Serial.print(i + 1);
    Serial.print("(");
    Serial.print( arrayOfCords[i].x );
    Serial.print( "," );
    Serial.print(arrayOfCords[i].y);
    Serial.print( ") " );

    if ( (i + 1) % 5 == 0  )
    {
      Serial.println(""); // Go a new line.
    }
  }

  // *End of Test*
  Serial.println("");
  Serial.println("******");
  Serial.println("It is MAGIC TIME");

  delay(5000);
  //  while ( true)
  ////  {
  ////    // to stop executing the code.
  ////  }


  int previousX = 0;
  int previousY = 0;

  for ( int counter = 0; counter < numbOfCoords; ++counter)
  {

    int xMov = arrayOfCords[counter].x - previousX; // To find the mm we need to move from our current position.
    int absMovX = abs(xMov);
    for (int mmOnX = 0 ; mmOnX < revTomm * absMovX; mmOnX++)
    {
      if (xMov < 0)
      {
        digitalWrite(dirPin1, LOW);
      }
      digitalWrite(stepPin1, HIGH);
      delayMicroseconds(900); // The speed of the stepper motor
      digitalWrite(stepPin1, LOW);
      delayMicroseconds(900);
    }
    digitalWrite(dirPin1, HIGH);
    // Since we already used the current x cord, we state it as the old x.
    previousX = arrayOfCords[counter].x;

    // Wait for 0.4 after moving the X axis
    delay(400);

    
    int yMov = arrayOfCords[counter].y - previousY;
    int absMovY = abs(yMov);
    for (int mmOnY = 0 ; mmOnY < revTomm * absMovY; mmOnY++)
    {
      if (yMov < 0)
      {
        digitalWrite(dirPin2, LOW);
      }
      digitalWrite(stepPin2, HIGH);
      delayMicroseconds(900);
      digitalWrite(stepPin2, LOW);
      delayMicroseconds(900);
    }
    digitalWrite(dirPin2, HIGH);
    previousY = arrayOfCords[counter].y;

    // Wait for 0.4 after moving the Y axis
    delay(400);

    
    // Pen Down.
    servo.write(servoDown);
    delay(400);
    // Pen Up.
    servo.write(servoUp);
    delay(400);
  }

  Serial.println("");
  Serial.println("~][ Returning to the original position ][~");
  delay(3000);

  // To find the mm we need to move from our current position.
  int xMov = previousX;
  for (int mmOnX = 0 ; mmOnX < revTomm * xMov; mmOnX++)
  {

    // Negative direction cuz we are returning back to the origin.
    digitalWrite(dirPin1, LOW);

    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(900);
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(900);
  }
  digitalWrite(dirPin1, HIGH);

  // To find the mm we need to move from our current position.
  int yMov = previousY;
  for (int mmOnY = 0 ; mmOnY < revTomm * yMov; mmOnY++)
  {

    // Negative direction cuz we are returning back to the origin.
    digitalWrite(dirPin2, LOW);

    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(900);
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(900);
  }
  digitalWrite(dirPin2, HIGH);

  Serial.println("");
  Serial.println("Done!");
  Serial.println("");

  Serial.println("**********************************************************");
  Serial.println("Do you wanna plot something again? ( Enter 1 to plot again )");
  while (Serial.available() == 0)
  {
    // wait for input.
  }
  delay(100);
  recvWithEndMarker();
  int choice;
  choice = getUserInput();
  if ( choice != 1 )
  {
    Serial.println("Thank for using our 2 dimensional plotter!");
    Serial.println("\t\tHope to see you again!");
    while (true); // infinite loop to stop the plotting.
  }

  Serial.println("");
  Serial.println("");
}


void recvWithEndMarker() {
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;

  // if (Serial.available() > 0) {
  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();

    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    }
    else {
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}
int getUserInput() {
  if (newData == true) {
    // Serial.print("This just in ... ");
    // Serial.println(receivedChars);
    int x;
    x = atoi(receivedChars);
    newData = false;
    return x;
  }
  else {
    Serial.println("Error: no new data");
    return -1;
  }
}
