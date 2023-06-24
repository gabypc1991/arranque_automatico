
#define contacto 4
#define arranque 5
#define presion 6
#define tension_generador 7
#define tension_red 8
#define boton_manual 9
#define selectora 10
#define boton_reset 11

int tiempo_arranque = 2000;
int tiempo_pausa = 5000;
int tiempo_apagado = 120000;
int espera_error = 60000;
int intentos_arranque = 4;
int intentos_ciclo = 4;

bool presion_gen = false;
bool tension_gen = false;
bool tensionRed = false;
bool error = false;
bool error_ciclo = false;
bool marchaOk = false;

int intentos = 1;
int ciclos = 1;

void setup() {
  pinMode(contacto, OUTPUT);
  pinMode(arranque, OUTPUT);
  pinMode(selectora, OUTPUT);
  pinMode(presion, INPUT_PULLUP);
  pinMode(tension_generador, INPUT_PULLUP);
  pinMode(tension_red, INPUT_PULLUP);
  pinMode(boton_manual, INPUT_PULLUP);
  pinMode(boton_reset, INPUT_PULLUP);

  digitalWrite(contacto, LOW);
  digitalWrite(arranque, LOW);
  digitalWrite(selectora, LOW);

  Serial.begin(9600);

}

void loop() {
  estado_generador();
  if(tensionRed == false && marchaOk == false && error_ciclo == false){
    arranque_generador();
    }
  //imprimir_estado();
}

void imprimir_estado(){
  Serial.print("Presion Aceite: "); Serial.println(presion_gen);
  Serial.print("Tension Generador: "); Serial.println(tension_gen);
  Serial.print("Tension Red: "); Serial.println(tensionRed);
  Serial.print("Error Ciclo "); Serial.println(error_ciclo);
  }

void estado_generador(){
  presion_gen = !digitalRead(presion);
  tension_gen = !digitalRead(tension_generador);
  tensionRed = !digitalRead(tension_red);
  }
  
void parada_emergencia(){
  digitalWrite(contacto, LOW);
  digitalWrite(arranque, LOW);
  digitalWrite(selectora, LOW);
  }
  
void arranque_generador(){  
  if(error == true){delay(espera_error); error = false;}
  estado_generador();
  while(presion_gen == false || tension_gen == false || tensionRed == false || intentos <= intentos_arranque){
    estado_generador();
    Serial.print("Intentando arranque N°: "); Serial.println(intentos);
    Serial.print("Intentando ciclo N°: "); Serial.println(ciclos);
    digitalWrite(contacto, HIGH);
    digitalWrite(arranque, LOW);
    delay(1000);
    digitalWrite(contacto, HIGH);
    digitalWrite(arranque, HIGH);
    delay(tiempo_arranque);
    digitalWrite(contacto, HIGH);
    digitalWrite(arranque, LOW);
    estado_generador();
    if(presion_gen == true || tension_gen == true){marchaOk = true; break;}else{marchaOk = false; intentos += 1;}
    if(intentos > intentos_arranque){ciclos += 1; error = true; break;}
    if(ciclos > intentos_ciclo){error_ciclo = true; parada_emergencia(); break;}            
    }
  }
