void rhySetup(){
  for(int i=0;i<NUM_STEPS;i++) rhy.set(i, i*100);
}

void rhyLoop(){

  static byte density = 0;
  if( getPot(1)/100 != density ) {
    density = getPot(1)/100;

    int acc = 0;

    for(int i=0;i<NUM_STEPS;i++){
      acc += getPot(1)*5/4;
      acc = acc > 1000 ? acc%1000 : acc;
      
      rhy.set(i, acc);
    }

  }

}
