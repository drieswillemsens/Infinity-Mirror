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

//unsigned long previousMillis = 0;
//const long interval = 30000; //interval tussen kleurveranderingen

void setup()
{
  Serial.begin(9600);
  
  pinMode(pinMicro,INPUT_PULLUP); //SIG of the Parallax Sound Impact Sensor connected to Digital Pin 7
  pinMode(pinRed, OUTPUT);
  pinMode(pinGreen, OUTPUT);
  pinMode(pinBlue, OUTPUT);
  pinMode(pinCLK, INPUT);
  pinMode(pinDT, INPUT);
  pinMode(pinSW, INPUT_PULLUP);

  attachInterrupt(0, isr, FALLING);
  
  //randomSeed(analogRead(0));
  Red = LedVal[((virtualPosition*10)+120)%360];
  Green = LedVal[virtualPosition*10];
  Blue = LedVal[((virtualPosition*10)+240)%360];
  
  /*newRed = Red;
  newGreen = Green;
  newBlue = Blue;*/

  Serial.println("Start");
}

// Interrupt service routine is executed when a HIGH to LOW transition is detected on CLK
void isr ()  {
    if (!digitalRead(pinDT))
        virtualPosition = virtualPosition + 1;
    else
        virtualPosition = virtualPosition - 1;
    } // isr
    
//this function will make the LED dim once the Parallax Sound Impact Sensor sends a 1 signal, and then return to it’s original brightness.
void loop()
{
  //unsigned long currentMillis = millis();

  if(virtualPosition != lastPosition) {
    Serial.println(virtualPosition);
    virtualPosition = 36 + virtualPosition;
    virtualPosition = virtualPosition % 36;
    lastPosition = virtualPosition;
    Serial.println(virtualPosition);

    Red = LedVal[((virtualPosition*10)+120)%360];
    Green = LedVal[virtualPosition*10];
    Blue = LedVal[((virtualPosition*10)+240)%360];    
  }
  /*//Every interval calculate a new color
  if(currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;

    newRed = random(255);
    newGreen = random(255);
    newBlue = random(255);
  }

  //Fade to the new color
  //Red
  if(newRed > Red)
  {
    Red += 1;
  }
  else if(newRed < Red)
  {
    Red -= 1;
  }
  //Green
  if(newGreen > Green)
  {
    Green += 1;
  }
  else if(newGreen < Green)
  {
    Green -= 1;
  }
  //Blue
  if(newBlue > Blue)
  {
    Blue += 1;
  }
  else if(newBlue < Blue)
  {
    Blue -= 1;
  }*/

  //If sound detected, dim the led strip
  boolean soundstate = digitalRead(pinMicro);
  if (soundstate == 0) {
         analogWrite(pinRed, 0);
         analogWrite(pinGreen, 0);
         analogWrite(pinBlue, 0);
         delay(50);
  }
  else{
  
    analogWrite(pinRed, Red);
    analogWrite(pinGreen, Green);
    analogWrite(pinBlue, Blue);
  }
}
