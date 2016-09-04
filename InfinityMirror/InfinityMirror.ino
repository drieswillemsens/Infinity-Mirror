//RGB strip pins
#define pinRed 9
#define pinGreen 10
#define pinBlue 11
//Microphone pin
#define pinMicro 7
//Rotary encoder pins
#define pinCLK 2
#define pinDT 4
#define pinSW 5
//Program pins
#define pinP1 A2
#define pinP2 A1
#define pinP3 A0

//RGB strip variables
int Red;
int Green;
int Blue;
/*long newRed;
long newGreen;
long newBlue;*/

int LedVal[] = {0,0,0,0,0,1,1,2,2,3,4,5,6,7,8,9,11,12,13,15,17,18,20,22,24,26,28,30,32,35,37,39,42,44,47,49,52,55,58,60,63,66,69,72,75,78,81,85,88,91,94,97,101,104,107,111,114,117,121,124,127,131,134,137,141,144,147,150,154,157,160,163,167,170,173,176,179,182,185,188,191,194,197,200,202,205,208,210,213,215,217,220,222,224,226,229,231,232,234,236,238,239,241,242,244,245,246,248,249,250,251,251,252,253,253,254,254,255,255,255,255,255,255,255,254,254,253,253,252,251,251,250,249,248,246,245,244,242,241,239,238,236,234,232,231,229,226,224,222,220,217,215,213,210,208,205,202,200,197,194,191,188,185,182,179,176,173,170,167,163,160,157,154,150,147,144,141,137,134,131,127,124,121,117,114,111,107,104,101,97,94,91,88,85,81,78,75,72,69,66,63,60,58,55,52,49,47,44,42,39,37,35,32,30,28,26,24,22,20,18,17,15,13,12,11,9,8,7,6,5,4,3,2,2,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//Rotary encoder variable
volatile int virtualPosition = 0;
volatile int lastPosition;

//Program status variable
int Program = 1;

unsigned long dimMillis = 0;
const int dimInterval = 50; //Time the RGB strip needs to be dimmed

//Switch variables
unsigned long Debounce = 0;
const int debounceDelay = 50;
int lastButtonState = HIGH;

const long interval = 30000; //interval between color changes

void setup()
{
  Serial.begin(9600);
  
  pinMode(pinMicro,INPUT_PULLUP); //SIG of the Parallax Sound Impact Sensor connected to Digital Pin 7
  pinMode(pinRed, OUTPUT);
  pinMode(pinGreen, OUTPUT);
  pinMode(pinBlue, OUTPUT);
  pinMode(pinCLK, INPUT_PULLUP);
  pinMode(pinDT, INPUT_PULLUP);
  pinMode(pinSW, INPUT_PULLUP);
  pinMode(pinP1, OUTPUT);
  pinMode(pinP2, OUTPUT);
  pinMode(pinP3, OUTPUT);

  attachInterrupt(0, isr, FALLING);

  digitalWrite(pinCLK, HIGH);
  digitalWrite(pinDT, HIGH);
  digitalWrite(pinSW, HIGH);
  
  //randomSeed(analogRead(0));
  //Red = LedVal[((virtualPosition*10)+120)%360];
  //Green = LedVal[virtualPosition*10];
  //Blue = LedVal[((virtualPosition*10)+240)%360];
  
  /*newRed = Red;
  newGreen = Green;
  newBlue = Blue;*/

  //Bootup sequence (ligth up LED's one by one)
  digitalWrite(pinP1, HIGH);
  delay(1000);
  digitalWrite(pinP1, LOW);
  digitalWrite(pinP2, HIGH);
  delay(1000);
  digitalWrite(pinP2, LOW);
  digitalWrite(pinP3, HIGH);
  delay(1000);
  digitalWrite(pinP3, LOW);

  Serial.println("Start");
}

// Interrupt service routine is executed when a HIGH to LOW transition is detected on CLK of the rotary encoder
void isr ()  {
    if (!digitalRead(pinDT))
        virtualPosition = virtualPosition - 1;
    else
        virtualPosition = virtualPosition + 1;
    } // isr
    
void loop()
{

  //If knob has been turned, change the RGB values for the LED strip
  if(virtualPosition != lastPosition) {
    virtualPosition = 36 + virtualPosition;
    virtualPosition = virtualPosition % 36;
    lastPosition = virtualPosition;
    Serial.print("RGB value: ");
    Serial.println(virtualPosition);

    Red = LedVal[((virtualPosition*10)+120)%360];
    Green = LedVal[virtualPosition*10];
    Blue = LedVal[((virtualPosition*10)+240)%360];

    Serial.print("Red: ");
    Serial.println(Red);
    Serial.print("Green: ");
    Serial.println(Green);
    Serial.print("Blue: ");
    Serial.println(Blue);
  }

  //If knob has been pressed, change the program status
  int ButtonState = digitalRead(pinSW);
  
  if (ButtonState != lastButtonState){
    //Debounce = millis();
    /*}
  if (millis() > Debounce + debounceDelay)*/
    lastButtonState = ButtonState;
    if(lastButtonState == LOW) {
      Program = Program + 1;
      Serial.print("Program: ");
      Serial.println(Program);
    }
  }

  //If sound detected, set dimMillis to current millis
  boolean soundstate = digitalRead(pinMicro);
  if (soundstate == 0) {
    dimMillis = millis();
  }
  
  //Execute program according to Program status
  switch (Program){
    case 1:
      digitalWrite(pinP1, HIGH);
      digitalWrite(pinP2, LOW);
      digitalWrite(pinP3, LOW);
      //As long as the interval is not passed dim the RGB strip
      if (millis() < dimMillis + dimInterval) {
        analogWrite(pinRed, 0);
        analogWrite(pinGreen, 0);
        analogWrite(pinBlue, 0);
      }
      else{
        analogWrite(pinRed, Red);
        analogWrite(pinGreen, Green);
        analogWrite(pinBlue, Blue);
      }
      break;
    case 2:
      digitalWrite(pinP1, LOW);
      digitalWrite(pinP2, HIGH);
      digitalWrite(pinP3, LOW);
      break;
    case 3:
      digitalWrite(pinP1, LOW);
      digitalWrite(pinP2, LOW);
      digitalWrite(pinP3, HIGH);
      break;
    case 4:
      Program = 1;
  }
}
