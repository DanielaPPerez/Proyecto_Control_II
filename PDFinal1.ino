//-----Definición Variables----------------------------------------------------------------------------------//
float digPin5= 5;   //Cable azul
float digPin6= 6;   //Cable verde
float digPin4 = 4;   //Potenciometro +
float digPin3 = 3;   //Potenciometro -
float voltaje = 0;   //Voltaje que se lee del potenciometro
float volt_ref = 3;  //Voltaje que se lee del potenciometro para la referencia
float error = 0;     //Error
float u, upast=0;    //Acción de control y u[n-1]
float errorpast;     //e[n-1]
float kd = 0.086;
float kp = 40.8;        //Ganancia proporcional

int valor_pot = 0;  //Variable donde almacenaremos el valor del potenciómetro
int pot_ref = 0;    //Valor del potenciometro que cambia la referencia
int PWM = 0;        //Valor que tomará la PWM entre 0 y 255

const unsigned int Ts = 4;// Periodo de muestreo [ms]
unsigned int t,t0; //para darle un entero más a los valores

char estado;
//-----Iniciacion de Puertos y Pines-------------------------------------------------------------------------//
void setup() {
  Serial.begin(115200); // permite la configuración con el compu

  pinMode(digPin5,OUTPUT);pinMode(digPin6,OUTPUT);pinMode(digPin4,OUTPUT);pinMode(digPin3,OUTPUT);digitalWrite(digPin4,HIGH);
  digitalWrite(digPin3,LOW);

  t0 = millis();  // función del tiempo
}
//-----Repetición-------------------------------------------------------------------------------------------//
void loop() {

  t = millis();  // put your main code here, to run repeatedly:
 if (t - t0 >= Ts)
 {
  
  valor_pot = analogRead(A0);
  pot_ref = analogRead(A5);
  
  voltaje = (valor_pot*5.00/1023.00);
  volt_ref = (pot_ref*5.00/1023.00);

  if (Serial.available()) { 
  String command = Serial.readStringUntil('\n');
  volt_ref = command.toFloat();
  }
  
  error = (volt_ref-voltaje);

  u=((kp+(kd/Ts))*error)-(kd*errorpast);
  errorpast=error;
  
  PWM = PWM_signal(u);
  
  if(error > 0){
    //estado = 'D';
    analogWrite(digPin5, PWM);  //sentido del giro DERECHA
    analogWrite(digPin6, 0);
    lecturas();
  }
  
  if (error==0) {
    //estado = 'M';
    analogWrite(digPin5, 0);  //off
    analogWrite(digPin6, 0);
    lecturas();
  }

  if (error<0) {
    //estado = 'I';
    analogWrite(digPin5, 0);  //sentido del giro IZQ
    analogWrite(digPin6, PWM);
    lecturas();
  } 
   t0 = t;
  }
}
//-----Función Imprimir Datos--------------------------------------------------------------------------------//
void lecturas(){
  
  //Serial.print(" Ts: ");Serial.print((t-t0));
  Serial.print(" V: ");Serial.print(voltaje);
  Serial.print(" Ref: ");Serial.print(volt_ref);
  Serial.print(" Er: ");Serial.println(error);
  //Serial.print(" PWM: ");Serial.print(PWM);
  //Serial.print(" Estado: ");Serial.print(estado);
}
//-----Funcion Generadora PWM--------------------------------------------------------------------------------//
int PWM_signal(float U){
  int u_temp;
  u_temp=abs(U)*10;
  PWM=map(u_temp,0,50,0,255);
  if (PWM >= 255){return PWM=255;}
  if(PWM<0){return PWM=0;}
  else{return PWM;}
}
