const byte ALGO_DEBUG = 0;



int16_t goalSeq[4][NUM_STEPS];

int16_t curSequence[NUM_STEPS];

void algoSetup(){
  for(int i=0;i<4;i++){
    for(int k=0;k<NUM_STEPS;k++) goalSeq[i][k] = random(0,4096);
  }
}

void algoLoop(){
  genNewSequence();

  if(ALGO_DEBUG){
    int16_t temp[NUM_STEPS];
    seq.getArray(temp,NUM_STEPS);
    Serial.print("goal ");
    printArray(goalSeq[curGoal], NUM_STEPS);
    Serial.print("gen  ");
    printArray(temp, NUM_STEPS);
    Serial.println();
  }
}

void genNewSequence(){
  float scalar = 2*(float)getPot(0)/1023;
  //Serial.println("scalar" + (String)scalar);
  
  for( int i=0;i<NUM_STEPS;i++){
    int diff = abs(goalSeq[curGoal][i] - seq.get(i) );
    if(diff < 3) diff = 3;
    
    curSequence[i] = seq.get(i) + scalar*(random(diff) - (diff/2));

    int fitness = calcFitness(i);
    if(fitness>0) seq.set(i,curSequence[i]);
  } 
}

int calcFitness(int num){
  int curFitness = 0;
  int newFitness = 0;

    newFitness = abs(goalSeq[curGoal][num] - curSequence[num] );
    curFitness = abs(goalSeq[curGoal][num] - seq.get(num) );

  return (curFitness - newFitness);
}
