#define audio A0 
#define trimPot A1
#define modeButton 10 // This is the button that is built into the BMBB
#define modeLED 5
#define mouth1 7 //open (Mouth 1 and Mouth 2 might need to be swapped depending on which way the motor was installed in the motor driver. (The polarity was not labeled on my motors))
#define mouth2 6 //close
#define head 9 // The pins for head and tail might also need to be swapped for the same reason as the mouth
#define tail 8 
bool state = true; // true is speech mode, the light is off in speech mode
int threshold = 2040; // speech mode is on by default so the threshold is initialized at the speech threshold
int adjust = 0; // this value is added to the threshold value to allow for fine tuning of the threshold on the fly
int level = 0;
int onDelay = 80; //on delay is the delay for how long the motor should remain on (initialized at speach mode value)
int loopDelay = 10; // Loop Delay is the delay between loops of code (Initialized at the speech mode value)


void setup() {
//  pinMode(LED_BUILTIN, OUTPUT); 
//  digitalWrite(LED_BUILTIN, HIGH); // This line and the one above illuminates the built in LED (used for testing purposes)
  pinMode(audio, INPUT); // all pins are initialized according to their function
  pinMode(trimPot, INPUT);
  pinMode(modeButton, INPUT_PULLUP);
  pinMode(modeLED, OUTPUT);
  pinMode(mouth1, OUTPUT);
  pinMode(mouth2, OUTPUT);
  pinMode(head, OUTPUT);
  pinMode(tail, OUTPUT);
  analogReadResolution(12); // changes the resolution to the 12 bits for the Pico's 12 bit ADC
//  Serial.begin(115200); // initializes serial (for testing)
}

void loop() {
  if (digitalRead(modeButton)==LOW) { // checks to see if the button is pressed
    state =! state; // when pressed, the state boolean will swap
    delay(200); // debounce for the button
  }
  adjust = map(analogRead(trimPot), 0, 4095, -50, 200); // reads the trim pot to fine tune the threshold
  if (state == true){ // Checks for speech mode
    digitalWrite(modeLED, LOW); //LED is off for speech mode
    threshold = (2040 + adjust); // sets the threshold to the speech value plus the adjustment
    onDelay = 90; //sets on delay to the value for speech mode
    loopDelay = 10; //sets loop delay to speech mode value
  }
  if (state == false) { // checks for music mode
    analogWrite(modeLED, 125); // LED is on in music mode. PWM is used here because i wanted the LED to be dimmer
    threshold = (2100 + adjust); // sets threshold to default music value plus the adjustment
    onDelay = 65; // sets on delay to the value for music mode
    loopDelay = 2; // sets loop delay to the music mode value
  }
  level = analogRead(audio); // reads the audio level to the level variable
  
  if (level > threshold){ //checks if the audio level is higher than the threshold
    digitalWrite(mouth1, HIGH); // this opens the mouth by bringing one of the control pins on the motor driver high
    delay(onDelay); // delay is used to give the motor adequate time to respond depending on the mode it is in
  } else {
    digitalWrite(mouth1, LOW); // this turns off the mouth motor when the audio level falls below the threshold, closing the mouth
  }
  delay(loopDelay); // loop delay slows down the code to prevent the fish from reacting poorly
}
