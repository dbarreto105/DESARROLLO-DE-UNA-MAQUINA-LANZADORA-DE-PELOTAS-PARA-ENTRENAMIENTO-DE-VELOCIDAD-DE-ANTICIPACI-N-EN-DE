#include<Servo.h>
Servo ESC; //Crear un objeto de clase servo
Servo ESC2;

char valor = 1;

int DIRH = 4;
int PULH = 5;
int DIRV = 6;
int PULV = 7;
int PMAS = 8;
int PMEN = 9;
int RD = 10;
int RI = 11;

int pasoH = 1;
int pasoHA = 12;
int pasoV = 1;
int pasoVA = 12;
int Ppiston = 0;

int IH = 20;
int IV = 21;
int IP = 18;
int Ip1 = 3;
int Ip2 = 2;

int Tlanzamiento = 0;
int Vrueda = 0;
int contadorlanzador = 0;
int contadorrueda = 0;
int contadorlanzador2 = 0;
int contadorrueda2 = 0;


int Tpaso = 15000;
unsigned long Contador = 0;
unsigned long Contadormilis = 0;
// Posicion
int posicion;

void setup() {
  Serial.begin(115200);
  pinMode(DIRH, OUTPUT);//definir pin como salida
  pinMode(PULH, OUTPUT);//definir pin como salida
  pinMode(DIRV, OUTPUT);//definir pin como salida
  pinMode(PULV, OUTPUT);//definir pin como salida



  attachInterrupt(digitalPinToInterrupt(IH), FIH, RISING);//definir pin de interrupcion (final de carrera 1 motor horizontal)
  attachInterrupt(digitalPinToInterrupt(IV), FIV, RISING);//definir pin de interrupcion (final de carrera 1 motor horizontal)
  attachInterrupt(digitalPinToInterrupt(IP), FIP, RISING); //definir pin de interrupcion (final de carrera 1 motor horizontal)
  attachInterrupt(digitalPinToInterrupt(Ip1), FP1, FALLING);
  attachInterrupt(digitalPinToInterrupt(Ip2), FP2, FALLING);


  //MOTOR HORIZONTAL EN POSCION INICIAL

  boolean prueva = digitalRead (IH);// lectura por si el motor horizontal ya esta en posicion inicial
  if (prueva == false) {
    pasoHA = 1; // definir posicion inicial
  }
  while (pasoHA != 1) { // girar el motor hasta que este en pocicion inicial

    digitalWrite(DIRH, HIGH);
    digitalWrite(PULH,  HIGH);
    Esperar(Tpaso);
    digitalWrite(DIRH, HIGH);
    digitalWrite(PULH, LOW);
    Esperar(Tpaso);

  }
  //desactivar piston
  digitalWrite(PMAS, LOW);
  digitalWrite(PMEN, LOW);

  //MOTOR VERTICAL EN POSCION INICIAL

  boolean pruevaV = digitalRead (IV);// lectura por si el motor horizontal ya esta en posicion inicial
  if (pruevaV == false) {
    pasoVA = 1; // definir posicion inicial
  }


  while (pasoVA != 1) { // girar el motor hasta que este en pocicion inicial

    digitalWrite(DIRV, LOW);
    digitalWrite(PULV,  HIGH);
    Esperar(Tpaso);
    digitalWrite(DIRV, LOW);
    digitalWrite(PULV, LOW);
    Esperar(Tpaso);
  }


  //INICIALIZAR ESCS
  ESC.attach(RD);
  ESC2.attach(RI);
  ESC.writeMicroseconds(1500); // Activar el ESC
  ESC2.writeMicroseconds(1500);
  delay(5000); //Esperar 5 segundos para hacer la activacion

  ESC.writeMicroseconds(1555);
  ESC2.writeMicroseconds(1555);
  // inicializar tiempos y tomar cemilla de aleatoreidad
  Tpaso = 10000;
  randomSeed(analogRead(A0));
  Tlanzamiento = random(1000, 10000);
  
  randomSeed(analogRead(A0));
 

  ///////////////////
  contadorlanzador2 = millis();
}
void loop() {
  // recivir posicion
  if (Serial.available() > 0)
  {
    valor = Serial.read();
    calcularDistancia(valor);

    // fin recivir posicion
  }
  // HUVICAR MOTOR VERTICAL EN POSICION RECIVIDA
  if (pasoH > pasoHA) {

    digitalWrite(DIRH, LOW);
    digitalWrite(PULH,  HIGH);
    Esperar(Tpaso);
    digitalWrite(DIRH, LOW);
    digitalWrite(PULH, LOW);
    Esperar(Tpaso);
    pasoHA = pasoHA + 1;
  }
  if (pasoH < pasoHA) {
    digitalWrite(DIRH, HIGH);
    digitalWrite(PULH,  HIGH);
    Esperar(Tpaso);
    digitalWrite(DIRH, HIGH);
    digitalWrite(PULH, LOW);
    Esperar(Tpaso);
    pasoHA = pasoHA - 1;
  }


  if (pasoV > pasoVA) {

    digitalWrite(DIRV, HIGH);
    digitalWrite(PULV,  HIGH);
    Esperar(Tpaso);
    digitalWrite(DIRV, HIGH);
    digitalWrite(PULV, LOW);
    Esperar(Tpaso);
    pasoVA = pasoVA + 1;
  }
  if (pasoV < pasoVA) {
    digitalWrite(DIRV, LOW);
    digitalWrite(PULV,  HIGH);
    Esperar(Tpaso);
    digitalWrite(DIRV, LOW);
    digitalWrite(PULV, LOW);
    Esperar(Tpaso);
    pasoVA = pasoVA - 1;
  }

  /*if (pasoH==pasoHA && pasoV == pasoVA){
    //desactivar piston
    digitalWrite(PMAS, LOW);
  digitalWrite(PMEN, HIGH);
  delay(500);
  digitalWrite(PMAS, HIGH);
  digitalWrite(PMEN, LOW);
  delay(500);
  digitalWrite(PMAS, LOW);
  digitalWrite(PMEN, LOW);
  delay(1500);
  }*/

  /////////
  contadorlanzador = millis();
  if ( (contadorlanzador - contadorlanzador2) > Tlanzamiento) {
   
    // meter piston
    digitalWrite(PMAS, LOW);
    digitalWrite(PMEN, HIGH);
    Esperarmilis(500);
    // sacar piston
    digitalWrite(PMAS, HIGH);
    digitalWrite(PMEN, LOW);
    Esperarmilis(500);
    //descativar piston
    digitalWrite(PMAS, LOW);
    digitalWrite(PMEN, LOW);
    // reiniciar tiempo
    contadorlanzador2 = millis();
    randomSeed(analogRead(A0));
    Tlanzamiento = random(1000, 10000); //nueva semilla de tiempo aleatorio
    
    //velocidad
    randomSeed(analogRead(A0));
    Vrueda = random(1550, 1590);
  
  ESC.writeMicroseconds(Vrueda);
  ESC2.writeMicroseconds(Vrueda);
    contadorlanzador2 = millis();
  }


}

// SECUENCIA INTERRUPCION MOTOR HORIZONTAL
void FIH () {
  pasoHA = 1;


}
// SECUENCIA INTERRUPCION MOTOR VERTICAL
void FIV () {
  pasoVA = 1;

}
// SECUENCIA INTERRUPCION PISTON
void FIP () {
  Ppiston = 1;

}

// SECUENCIA INTERRUPCION RECONTEO DE PASOS
void FP1 () {

  pasoHA = 33;
}
void FP2 () {

  pasoHA = 57;
}

// FUNCION CONTAR TIEMPO ENTRE PASO DE MOTOR
void Esperar (int valor) {
  Contador = micros() + valor;
  while (Contador >= micros()) {

  }
}

void Esperarmilis (int valor) {
  Contadormilis = millis() + valor;
  while (Contadormilis>= millis()) {

  }
}

// FUNCION ELEGIR PASO   ( caracteres ASCII )
void calcularDistancia (int paso) {
  switch (paso) {
    //83 pasos verticales (42 el central )
    case '!':
      pasoH = 7;
      break;
    case '"':
      pasoH = 8;
      break;
    case '#':
      pasoH = 9;
      break;
    case '$':
      pasoH = 10;
      break;
    case '%':
      pasoH = 11;
      break;
    case '&':
      pasoH = 12;
      break;
    case '(':
      pasoH = 13;
      break;
    case ')':
      pasoH = 14;
      break;
    case '*':
      pasoH = 15;
      break;
    case '+':
      pasoH = 16;
      break;
    case ',':
      pasoH = 17;
      break;
    case '-':
      pasoH = 18;
      break;
    case '.':
      pasoH = 19;
      break;
    case '/':
      pasoH = 20;
      break;
    case '0':
      pasoH = 21;
      break;
    case '1':
      pasoH = 22;
      break;
    case '2':
      pasoH = 23;
      break;
    case '3':
      pasoH = 24;
      break;
    case '4':
      pasoH = 25;
      break;
    case '5':
      pasoH = 26;
      break;
    case '6':
      pasoH = 27;
      break;
    case '7':
      pasoH = 28;
      break;
    case '8':
      pasoH = 29;
      break;
    case '9':
      pasoH = 30;
      break;
    case ':':
      pasoH = 31;
      break;
    case ';':
      pasoH = 32;
      break;
    case '<':
      pasoH =33;
      break;
    case '=':
      pasoH = 34;
      break;
    case '>':
      pasoH = 35;
      break;
    case '?':
      pasoH = 36;    ///////
      break;
    case '@':
      pasoH = 37;
      break;
    case 'A':
      pasoH = 38;
      break;
    case 'B':
      pasoH = 39;
      break;
    case 'C':
      pasoH = 40;
      break;
    case 'D':
      pasoH = 41;
      break;
    case 'E':
      pasoH = 42;
      break;
    case 'F':
      pasoH = 43;
      break;
    case 'G':
      pasoH = 44;
      break;
    case 'H':
      pasoH = 45;
      break;
    case 'I':
      pasoH = 46;
      break;
    case 'J':
      pasoH = 47;
      break;
    case 'K':
      pasoH = 48;
      break;
    case 'L':
      pasoH = 49;
      break;
    case 'M':
      pasoH = 50;
      break;
    case 'N':
      pasoH = 51;
      break;
    case 'O':
      pasoH = 52;
      break;
    case 'P':
      pasoH = 53;
      break;
    case 'Q':
      pasoH = 54;
      break;
    case 'R':
      pasoH = 55;
      break;
    case 'S':
      pasoH = 56;
      break;
    case 'T':
      pasoH = 57;
      break;
    case 'U':
      pasoH = 58;
      break;
    case 'V':
      pasoH = 59;
      break;
    case 'W':
      pasoH = 60;
      break;
    case 'X':
      pasoH = 61;
      break;
    case 'Y':
      pasoH = 62;
      break;
    case 'Z':
      pasoH = 63;
      break;
    case '[':
      pasoH = 64;
      break;
    case ']':
      pasoH = 65;
      break;
    case '^':
      pasoH = 66;
      break;

    // 64 pasos verticales (central 70, conteo desde el paso 38)
    case '_':
      pasoV = 25;
      break;
    case '`':
      pasoV = 26;
      break;
    case 'a':
      pasoV = 27;
      break;
    case 'b':
      pasoV = 28;
      break;
    case 'c':
      pasoV = 29;
      break;
    case 'd':
      pasoV = 30;
      break;
    case 'e':
      pasoV = 31;
      break;
    case 'f':
      pasoV = 32;
      break;
    case 'g':
      pasoV = 33;
      break;
    case 'h':
      pasoV = 34;
      break;
    case 'i':
      pasoV = 35;
      break;
    case 'j':
      pasoV = 36;
      break;
    case 'k':
      pasoV = 37;
      break;
    case 'l':
      pasoV = 38;
      break;
    case 'm':
      pasoV = 39;
      break;
    case 'n':
      pasoV = 40;
      break;
    case 'o':
      pasoV = 41;
      break;
    case 'p':
      pasoV = 42;  ////
      break;
    case 'q':
      pasoV = 43;
      break;
    case 'r':
      pasoV = 44;
      break;
    case 's':
      pasoV = 45;
      break;
    case 't':
      pasoV = 46;
      break;
    case 'u':
      pasoV = 47;
      break;
    case 'v':
      pasoV = 48;
      break;
    case 'w':
      pasoV = 49;
      break;
    case 'x':
      pasoV = 50;
      break;
    case 'y':
      pasoV = 51;
      break;
    case 'z':
      pasoV = 52;
      break;
    case '{':
      pasoV = 53;
      break;
    case '|':
      pasoV = 54;
      break;
    case '}':
      pasoV = 55;
      break;
    case '~':
      pasoV = 56;
      break;
    case ' ':
      pasoV = 57;
      break;


  }
}
