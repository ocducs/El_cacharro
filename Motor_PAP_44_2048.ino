bool Run=false;
unsigned long tActual=0;

unsigned long t0=0;
unsigned long tf=0;

bool btnAnterior=LOW;
bool btnActual;
enum State{OFF, ON};
State estado=OFF;

 //////   

unsigned long tiempo=0;
int ix=0;int jx=0;
unsigned long tAnterior=0;
int mS=14; //milisegundos por vuelta del motor. valor max 10 sin calentarse(fabricante), soporta hasta 3 - 5 ms con pasos intermedios en Ctrl posicion 
int pin[]={8,9,10,11};
int paso [4][4] = //Recomendada por el fabricante (par máximo)
{
{1, 1, 0, 0},
{0, 1, 1, 0},
{0, 0, 1, 1},
{1, 0, 0, 1}
};

void setup() {
  // put your setup code here, to run once:
  set(pin);
  Serial.begin(115200);
  pinMode(12,INPUT_PULLUP);
  
}

void loop() {

  tActual=millis();
  
  if(Flanco()){
    //Serial.print(msg);
    Serial.print(" t: "+String(tf-t0));
    
    switch(estado){
    case OFF:
      estado=(tf-t0>1000)?ON:OFF;break;
      
    case ON:
      apagar();
      break;
    }
    if(tf-t0<400 && tf-t0>50){tiempo=tiempo+5000;}
  }
  
    Serial.print("Estado: ");
  if(estado){
    Serial.print(" ON  ");
  }
  else{
    Serial.print(" OFF ");
  }
    Serial.print(" tiempo: ");
    Serial.println(tiempo);
  
  if(estado){
    girar();
  }
    
  
}
void set(int param[]){
  int n=sizeof(param)/sizeof(*param);
  for(int i =0;i<n;i++){
    pinMode(param[i],OUTPUT);
  }  
}

bool Flanco(){
  btnActual=digitalRead(12);
  
  if(btnAnterior==HIGH && btnActual==LOW){
    t0=tActual; //tiempo inicial
    btnAnterior=btnActual;
    
  }
  if(btnAnterior==LOW && btnActual==HIGH){
    tf=tActual; //tiempo final
    btnAnterior=btnActual;
    return true;
  }
  btnAnterior=btnActual;
  return false;
}
void girar(){
   tActual=millis();
   if(tActual-tAnterior>mS){
      tAnterior=tActual;
      if(tiempo>0){
        
        digitalWrite(pin[0],paso[ix][0]);
        digitalWrite(pin[1],paso[ix][1]);
        digitalWrite(pin[2],paso[ix][2]);
        digitalWrite(pin[3],paso[ix][3]);
        ix++;
        ix=ix%4;
        
        if(tiempo<mS){
          apagar();
        }
        else{
        tiempo=tiempo-mS;
        }
      }
   }
}

void apagar(){
  if(tf-t0>1000){
    estado=OFF;
    tiempo=0;
    digitalWrite(pin[0],LOW);
    digitalWrite(pin[1],LOW);
    digitalWrite(pin[2],LOW);
    digitalWrite(pin[3],LOW);
  }
}
