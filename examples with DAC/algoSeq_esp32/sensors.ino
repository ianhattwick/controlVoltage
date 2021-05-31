const byte SENSOR_DEBUG = 0;

struct defPot{
  byte pin;
  int val;
  int prev;
};

defPot pot[3] = {
  {33,0},
  {32,0},
  {39,0}
};

//enum buttStates {UP, PUSHED, DOWN, RELEASED};

struct defSw{
  byte pin;
  uint32_t timer;
  buttStates state;
};

defSw sw[4] = {
  {23,0},
  {19,0},
  {18,0},
  {2,0}
};

void sensorSetup(){
  for(int i=0;i<3;i++) {
    pinMode( pot[i].pin, INPUT_PULLUP);
    pot[i].val = analogRead( pot[i].pin );
  }
  for(int i=0;i<4;i++) pinMode( sw[i].pin, INPUT_PULLUP);

  //Serial.println("Sensors initialized");
}

void sensorLoop(){
  static uint32_t timer = 0;
  int interval = 5;

  if (millis()-timer > interval){
    timer = millis();
    
    //check pots
    for( int i=0;i<3;i++){
      pot[i].prev = (pot[i].prev * 3 + analogRead( pot[i].pin )/4 )/4;
      
      if( abs(pot[i].prev - pot[i].val) > 3 ) {
        pot[i].val = pot[i].prev;
        if( SENSOR_DEBUG ) Serial.println("pot: " + String(i) + "val " + String(pot[i].val));
      }
    }//pot

    //check buttons
    for( int i=0;i<4;i++){
      switch( sw[i].state){
        case UP: //OFF
        case RELEASED:
        if( digitalRead( sw[i].pin) == 0){
          sw[i].state = PUSHED;
          sw[i].timer = millis();
          if( SENSOR_DEBUG ) Serial.println("sw: " + String(i) + "pushed ");
        }
        break;

        case PUSHED: //button is pushed down, looking for release state
        case DOWN:
        if( millis()- sw[i].timer > 25){
          if( digitalRead( sw[i].pin) == 1) sw[i].state = RELEASED;
        }
      }
    }//sw
  }
}

int getPot(byte num){
  return pot[num].val;
}

buttStates getSw(byte num){
  buttStates cur = sw[num].state;
  
  switch(sw[num].state){
    case PUSHED: sw[num].state = DOWN; break;
    case RELEASED: sw[num].state = UP; break;
  }
  
  return cur;
}
