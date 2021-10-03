// Button pins 2 - 6
const int b1pin = 2;
const int b2pin = 3;
const int b3pin = 4;
const int b4pin = 5;
const int b5pin = 6;

// Cmaj pentatonic Using Just-tuning ratios (just for fun)
const float CNOTE = 261.63; // 1:1
const float DNOTE = 294.33; // 9:8
const float ENOTE = 327.04; // 5:4
const float GNOTE = 392.44; // 3:2
const float ANOTE = 463.05; // 5:3

// Amin pentatonic using just-tuning ratios
const float AmNOTE = 220;   // 1:1
const float BmNOTE = 247.5; // 9:8
const float CmNOTE = 264;   // 6:5
const float EmNOTE = 330;   // 3:2
const float FmNOTE = 352;   // 8:5

// floating point arrays of the two potential scales
float CScale[5] = {CNOTE, DNOTE, ENOTE, GNOTE, ANOTE};
float AmScale[5] = {AmNOTE, BmNOTE, CmNOTE, EmNOTE, FmNOTE};

// float pointer to switch between the two scales
float* curScale = CScale;

int sampleMel[50] = {0,2,1,3,4,2,0,-1}; // a sample melody for testing 
int curBank[50];

// keySwitchPin goes on pin 12
const int keySwitchPin = 12;

// buzzer goes on pin 13
const int buzzPin = 13;

// play button and go on pins 9 and 10
const int playButtonPin = 10;
const int playLEDpin = 9;

const int recButtonPin = 11;

// read the state of each button
int b1State;
int b2State;
int b3State;
int b4State;
int b5State;

// read the sate of the key switch pin
int keySwitchState;

// read the state of the play button
int playState;

// read the state of the rec button
int recState;

bool rec = true; // set to false to only read one val per button press
int recPos = 0;  // records the position in dymArray to write to

void setup() {
  // button pins are for input
  pinMode(b1pin, INPUT);
  pinMode(b2pin, INPUT);
  pinMode(b3pin, INPUT);
  pinMode(b4pin, INPUT);
  pinMode(b5pin, INPUT);
  pinMode(playButtonPin, INPUT);
  pinMode(recButtonPin, INPUT);
  pinMode(playLEDpin, OUTPUT);

  // keySwitchPin is for input
  pinMode(keySwitchPin, INPUT);

  // temporary test to make sure buzzer is working
  tone(buzzPin, 440);
  delay(2000);
  noTone(buzzPin);

  // initialize dynBank to all -1
  for(int i = 0; i < 50; i++){
    curBank[i] = -1;
  }

  play(sampleMel);

}

// play the selected melpdy
void play(int* mel){
  digitalWrite(playLEDpin, HIGH);
  int pos = 0;
  while(mel[pos] != -1 && pos < 50){
    tone(buzzPin, curScale[mel[pos]]);
    delay(1000);  
    pos++;
  }
  digitalWrite(playLEDpin, LOW);
}
 
void loop() {
  // grab the state of the buttons
  b1State = digitalRead(b1pin);
  b2State = digitalRead(b2pin);
  b3State = digitalRead(b3pin);
  b4State = digitalRead(b4pin);
  b5State = digitalRead(b5pin);
  keySwitchState = digitalRead(keySwitchPin);
  recState = digitalRead(recButtonPin);
  playState = digitalRead(playButtonPin);
  

  if(keySwitchState == HIGH){
      curScale = CScale;  
  }
  else{
      curScale = AmScale;  
  }

  if(!recState){
    rec = false;   
  }

  if(playState){
    play(curBank);  
  }

  // if any of the buttons are pressed play corresponding tone
  if(b1State == HIGH){
      tone(buzzPin, curScale[0]);
      if(rec == true && recPos < 50){
        curBank[recPos] = 0;
        rec = false;
        recPos++;  
      }
  }
  else if(b2State == HIGH){
      tone(buzzPin, curScale[1]);
      if(rec == true && recPos < 50){
        curBank[recPos] = 1;
        rec = false;
        recPos++;  
      }      
  }
  else if(b3State == HIGH){
      tone(buzzPin, curScale[2]);
      if(rec == true && recPos < 50){
        curBank[recPos] = 2;
        rec = false;
        recPos++;  
      }      
  }
  else if(b4State == HIGH){
      tone(buzzPin, curScale[3]);  
      if(rec == true && recPos < 50){
        curBank[recPos] = 3;
        rec = false;
        recPos++;  
      }      
  }
  else if(b5State == HIGH){
      tone(buzzPin, curScale[4]);  
      if(rec == true && recPos < 50){
        curBank[recPos] = 4;
        rec = false;
        recPos++;  
      }      
  }
  // if no button is pressed, play nothing (for stanity)
  else{
      noTone(buzzPin);
      rec = true;
  }
  delay(5);
  

}
