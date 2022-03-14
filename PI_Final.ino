//-----Definición Variables----------------------------------------------------------------------------------//
float digPin5= 5;    //Cable azul
float digPin6= 6;    //Cable verde
float digPin4 = 4;   //Potenciometro +
float digPin3 = 3;   //Potenciometro -
float voltaje = 0;   //Voltaje que se lee del potenciometro
float volt_ref = 3;  //Voltaje que se lee del potenciometro para la referencia
float error = 0;     //Error
float u=0, upast=0;  //Acción de control y u[n-1]
float errorpast=0;     //e[n-1]
float ki = 0.1;        //Constante integral
float kd = 0;
float kp = 15;       //Ganancia proporcional
float errorcum;       //Acumulacion del error (acción integral)
float umax=5;         //Valor señal de control máxima
float imax=6;         //Valor integral máximo

int valor_pot = 0;  //Variable donde almacenaremos el valor del potenciómetro
int pot_ref = 0;    //Valor del potenciometro que cambia la referencia
int PWM = 0;        //Valor que tomará la PWM entre 0 y 255

const unsigned int Ts = 4;// Periodo de muestreo [us]
unsigned int t,t0; //para darle un entero más a los valores

char estado;
//-----Iniciacion de Puertos y Pines-------------------------------------------------------------------------//
void setup() {
  Serial.begin(115200); // Permite la configuración con el compu

  pinMode(digPin5,OUTPUT);pinMode(digPin6,OUTPUT);pinMode(digPin4,OUTPUT);pinMode(digPin3,OUTPUT);digitalWrite(digPin4,HIGH);digitalWrite(digPin3,LOW);

  t0 = millis();  // función del tiempo
}
//-----Repetición-------------------------------------------------------------------------------------------//
void loop() {

  t = millis(); 
 if (t - t0 >= Ts)
 {
  valor_pot = analogRead(A0); pot_ref = analogRead(A5);//Lee los datos
  
  if (Serial.available()) {String command = Serial.readStringUntil('\n');volt_ref = command.toFloat();}

  voltaje = (valor_pot*5.00/1023.00);
  volt_ref = (pot_ref*5.00/1023.00);
  error = (volt_ref-voltaje);  
  errorcum+=error;

  //Anti WINDUP
  if(errorcum>=imax){errorcum=imax;}; if(errorcum<=-imax){errorcum=-imax;}; 
  //Machete para dejarlo quieto
  if(error<=0.001&error>=-0.001){errorcum=0;}

  //u=(upast+((kp+ki)*error)-(kp*errorpast));
  u=kp*error+ki*Ts*errorcum+kd*(error-errorpast)/Ts;
  upast=u;
  errorpast=error;

  //Anti WINDUP?
  if(u>=umax)  {u=umax;};if(u<=-umax){u=-umax;}
  
  PWM = PWM_signal();
  
  if(error > 0){ //sentido del giro DERECHA
    analogWrite(digPin5, PWM);  analogWrite(digPin6, 0); lecturas(); //estado = 'D';
    }
  
  if (error<=0.001&error>=-0.001) { //Off
    analogWrite(digPin5, 0); analogWrite(digPin6, 0); lecturas(); //estado = 'M';
    }

  if (error<0) { //sentido del giro IZQ
    analogWrite(digPin5, 0);  analogWrite(digPin6, PWM); lecturas(); //estado = 'I';
    } 
   t0 = t;
  }
}
//-----Función Imprimir Datos--------------------------------------------------------------------------------//
void lecturas(){
  //Serial.print(" Ts: ");Serial.print((t-t0));
  Serial.print(" \t V: ");Serial.print(voltaje);
  Serial.print(" \t Ref: ");Serial.println(volt_ref);
  //Serial.print(" Err: ");Serial.print(error);
  //Serial.print(" Errcum: ");Serial.print(errorcum);
  //Serial.print(" PWM: ");Serial.println(PWM);
  //Serial.print(" u: ");Serial.println(u);
  //Serial.print(" estado: ");Serial.println(estado);
}
//-----Funcion Generadora PWM--------------------------------------------------------------------------------//
int PWM_signal(){
  int u_temp;
  u_temp=abs(u)*10;
  PWM=map(u_temp,0,50,0,255);
  if (PWM >= 255){return PWM=255;}
  if(PWM<0){return PWM=0;}
  return PWM;
}
