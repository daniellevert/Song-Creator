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

// the dynamic bank used for storing the melody
int curBank[50];

// the (two for now) banks for storing the melody
int bank1[50];
int bank2[50];
int bank3[50];
int bank4[50];
int bank5[50];

// keySwitchPin goes on pin 12
const int keySwitchPin = 12;

// buzzer goes on pin 13
const int buzzPin = 13;

// play button and go on pins 9 and 10
const int playButtonPin = 10;
const int playLEDpin = 9;
const int recButtonPin = 11;
const int potPin = 0;

const int inputLedPin = 8;

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
int recording = true;
int recPos = 0;  // records the position in dymArray to write to

int tempoRead;
int tempo;

void setup() {
  Serial.begin(9600);
  // button pins are for input
  pinMode(b1pin, INPUT);
  pinMode(b2pin, INPUT);
  pinMode(b3pin, INPUT);
  pinMode(b4pin, INPUT);
  pinMode(b5pin, INPUT);
  pinMode(playButtonPin, INPUT);
  pinMode(recButtonPin, INPUT);
  pinMode(playLEDpin, OUTPUT);
  pinMode(inputLedPin,  OUTPUT);
//  pinMode()

  // keySwitchPin is for input
  pinMode(keySwitchPin, INPUT);

  // initialize dynBank to all -1
  for(int i = 0; i < 50; i++){
    curBank[i] = -1;
  }
  play(sampleMel, 250);
}

// play
//
// play the selected melpdy at the tempo reading given by the potentiometer
void play(int* mel, int tempo){ 
  // turn the green led on
  digitalWrite(playLEDpin, HIGH);
  // play corespoinding chord tone until we read a -1
  int pos = 0;
  while(mel[pos] != -1 && pos < 50){
    tone(buzzPin, curScale[mel[pos]]);
    delay(7 * (tempo / 8));
    noTone(tempo / 8);  
    pos++;
  }
  // turn the green led off
  digitalWrite(playLEDpin, LOW);
}

// clearBank
//
// reset the bank to an empty song bank and reset recPos to 0
void clearBank(int* bank){
  for(int i = 0; i < 50; i++){
      bank[i] = -1;
    }
  recPos = 0;
}

// uploadBank
//
// Store the melody in cur bank into one of the five store banks
void uploadBank(int* bank){
  clearBank(bank);
  int pos = 0;
  while(curBank[pos] != -1 && pos < 50){
    bank[pos] = curBank[pos];
    pos++;
  }
  bank[pos] = -1;
}

// downloadBank
//
// Clear the melody in 
void downloadBank(int* bank){
  clearBank(curBank);
  int pos = 0;
  while(bank[pos] != -1 && pos < 50){
    curBank[pos] = bank[pos]; 
    pos++; 
  }
  curBank[pos] = -1;    
}


// blinkInputLed
//
// blinks the input led n times with a delay of d
void blinkInputLed(int n, int d){
     for(int i = 0; i < n; ++i){
      digitalWrite(inputLedPin, HIGH);
      delay(d);
      digitalWrite(inputLedPin, LOW);
      delay(d);
    } 
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
  tempoRead = analogRead(potPin);

  // read serial input if it is available
  if(Serial.available()){
    char cmd = (char)Serial.read(); // Need to cast?
    char bank = (char)Serial.read();

  if(cmd == 'd'){
     blinkInputLed(2, 500); // blink twice slow
     // blink n times fast
     if(bank == '5'){
        blinkInputLed(5, 250);
        downloadBank(bank5);
     }
     else if(bank == '4'){
       blinkInputLed(4,250); 
       downloadBank(bank4);
     }
     else if(bank == '3'){
       blinkInputLed(3,250);
       downloadBank(bank3); 
     }
     else if(bank == '2'){
       blinkInputLed(2,250); 
       downloadBank(bank2);
     }
     else if(bank == '1'){
       blinkInputLed(1,250);
       downloadBank(bank1);
     }
     else{
      blinkInputLed(10,25); // out of bounds
     } 
  } // end cmd == d 
  else if(cmd == 'u'){
    blinkInputLed(3, 500); // blink 3 times slow
    // blink n times fast
     if(bank == '5'){
        blinkInputLed(5, 250);
        uploadBank(bank5);
     }
     else if(bank == '4'){
       blinkInputLed(4,250);
       uploadBank(bank4);
     }
     else if(bank == '3'){
       blinkInputLed(3,250);
       uploadBank(bank3); 
     }
     else if(bank == '2'){
       blinkInputLed(2,250);
       uploadBank(bank2); 
     }
     else if(bank == '1'){
       blinkInputLed(1,250);
       uploadBank(bank1);
     }
     else{
      blinkInputLed(10,25); // out of bounds
     } 
  } // end cmd == u
  Serial.flush();
  } // end if Serial.available()

  // map tempo to correct range
  tempo = map(tempoRead, 0, 1023, 125, 500);

  if(keySwitchState == HIGH){
      curScale = CScale;    
  }
  else{
      curScale = AmScale;  
  }

  // record button is not presses, do not record presses
  if(!recState){
    rec = false;
    recording = false;
  }
  // record button is pressed, record button presses
  else{
    // if it is the first time we are reading the signal clear the song bank
    if(!recording){
      clearBank(curBank);
    }
    recording = true;  
  }

  // if play button is pressed play the melody at the tempo read by the potentiometer
  if(playState){
    play(curBank, tempo);  
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
      if(rec == true && recPos < 49){
        curBank[recPos] = 1;
        rec = false;
        recPos++;  
      }      
  }
  else if(b3State == HIGH){
      tone(buzzPin, curScale[2]);
      if(rec == true && recPos < 49){
        curBank[recPos] = 2;
        rec = false;
        recPos++;  
      }      
  }
  else if(b4State == HIGH){
      tone(buzzPin, curScale[3]);  
      if(rec == true && recPos < 49){
        curBank[recPos] = 3;
        rec = false;
        recPos++;  
      }      
  }
  else if(b5State == HIGH){
      tone(buzzPin, curScale[4]);  
      if(rec == true && recPos < 49){
        curBank[recPos] = 4;
        rec = false;
        recPos++;  
      }      
  }
  // if no button is pressed, play nothing (for sanity)
  else{
      noTone(buzzPin);
      rec = true;
  }
  delay(5);
}
