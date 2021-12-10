

volatile unsigned int counter = 400;
volatile bool newSig = false;
volatile uint8_t DACout = 128;

void setup() {
  EICRB = (0<<ISC51)|(1<<ISC50)|(0<<ISC41)|(1<<ISC40);
  EIMSK = (1<<INT5)|(1<<INT4);
  DDRE = (0<<PE5)|(0<<PE4); // Arduino Pins 2 + 3 Input
  PORTE = (1<<PE5)|(1<<PE4); // Arduino Pins 2 + 3 Pull-Up resistors Enabled
  DDRF = 0xFF;
  pinMode(13, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  if(newSig)
    Serial.println(DACout);
    newSig = false;
}

ISR(INT4_vect)
{
    byte encIn = PINE & 0x30;
    if((encIn == 0x30) || (encIn == 0x00))
      counter += 1;
    else
      counter -= 1;
    DACout = map(counter, 200, 600, 1, 254);
    PORTF = DACout;
    newSig = true;
    digitalWrite(13, 0);
}

ISR(INT5_vect)
{
    byte encIn = PINE & 0x30;
    if((encIn == 0x20) || (encIn == 0x10))
      counter += 1;
    else
      counter -= 1;
    DACout = map(counter, 200, 600, 1, 254);
    PORTF = DACout;
    newSig = true;
    digitalWrite(13, 1);
}
