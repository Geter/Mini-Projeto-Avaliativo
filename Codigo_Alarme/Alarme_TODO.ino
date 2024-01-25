// CARREGANDO AS BIBLIOTECAS
#include <Adafruit_LiquidCrystal.h> //Carrega a biblioteca com as funções para o sensor ultrassônico funcionar
#include <Servo.h> //Carrega a biblioteca com as funções para o servo motor  funcionar

//CRIANDO OS OBJETOS
Adafruit_LiquidCrystal lcd_1(0); // Cria uma "variável" do tipo lcd
Servo myservo; // Cria uma "variável" do tipo Servo
 
//DEFININDO NOMES ÀS PORTAS 
const int botLigar = 5;  //define a porta 5 como o botão de ligar
const int botTestar = 3; //define a porta 3 como botão de teste e desligar
const int ledLigado = 6; //define a porta 6 como LED vermelho q indica que o sistema está ligado
const int ledAlarme = 2; //define a porta 2 como LED azul que indica que o alarme disparou  
const int piezo = 8; // define a porta 8 como o piezo

//DEFININDO AS VAROÁVEIS
int pos = 0; //cria uma variável para controlar a posição do servomotor
int cm = 0; // cria uma variável para receber a distância em centímetros
int ligar = 0; // cria uma variável para receber a informação se o botão de ligar foi pressionado
int testar =0; //cria uma variável para receber a informação se o botão de testar/desligar foi pressionado
int distanciaSegura = 50; // cria a variável que define que a distância segura é de 50 centímetros
bool sistemaLigado = false; // cria uma variável do tipo boolean para receber a informação se o sistema está ligado ou desligado

//CRIANDO AS FUNÇÕES

//A função readUltrasonicDistance faz a leitura da distância e devolve um valor que será
// interpretado pela função escanear
long readUltrasonicDistance(int triggerPin, int echoPin){
  pinMode(triggerPin, OUTPUT);//Responder 1 : O que este trecho faz?  
  digitalWrite(triggerPin, LOW);//Responder 2: O que este trecho faz?
  delayMicroseconds(2);//Responder 3: O que este trecho faz?
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}

// A função escanear recebe um valor da função readUltrasonicDistance 
//e a transforma em centímetros, além de verificar a distância permitida e disparar o alarme
void escanear(){
  // measure the ping time in cm
  cm = 0.01723 * readUltrasonicDistance(7, 7);
  
  Serial.print(cm);//Responder 4: O que este trecho faz?
  Serial.println("cm");
  delay(100);//Responder 5: O que este trecho faz?
  if(cm <= distanciaSegura){//Responder 6: O que este trecho faz?
    disparaAlarme();
    delay(500);
  }
}

//Liga o Alarme 
void ligaAlarme(){
  sistemaLigado = true; 
  abrirTrava(1);//Responder 7: O que este trecho faz?
  limpaTela();
  
  Serial.println("Alarme ligado");
  escreveTela(0, "Ligado");
  escreveTela(1, "Escaneando...");
	
  	digitalWrite(ledLigado, HIGH);
}
  
//Desliga o alarme e desativa o disparo  
void desligaAlarme(){
  sistemaLigado = false;
  abrirTrava(0);  
  limpaTela();                     
  escreveTela(0, "Sist Desligado");
  escreveTela(1, "Ligar     Testar");
  digitalWrite(ledLigado, LOW);
  digitalWrite(ledAlarme, LOW);
  Serial.println("Sistema Desligado");
}

//Função responsável por dispara o alarme
void disparaAlarme(){
  limpaTela();
  digitalWrite(ledAlarme, HIGH);
  escreveTela(0, "...PERIGO...");
  escreveTela(1, "............");
  do{ // início de laço de repetição do while (faça... enquanto)
    
    for(int i = 0; i< 1000; i++){ // laço de repetição for (para valor inicial, valor final, incremento, faça)
      digitalWrite(piezo, HIGH);
      delayMicroseconds(500);
      digitalWrite(piezo, LOW);
      delayMicroseconds(500);
    }
    testar = digitalRead(botTestar);
    if(testar == HIGH){
      desligaAlarme();
    }
  }while(sistemaLigado == true);// Fim do laço do While
}

//Função para testar o alarme
void testaAlarme(){
    limpaTela();
    escreveTela(0, "Em teste........");
    Serial.println("Sistema em modo de teste");
    digitalWrite(ledAlarme, HIGH);
    for(int i = 0; i< 1000; i++){//Responder 8: O que este trecho faz?
      digitalWrite(piezo, HIGH);
      delayMicroseconds(500);
      digitalWrite(piezo, LOW);
      delayMicroseconds(500);
    }
    abrirTrava(1);
    abrirTrava(0);//Responder 9: O que este trecho faz?
    digitalWrite(ledAlarme, LOW); 
    escreveTela(1, "Teste concluido!"); 
    delay(1000);
    desligaAlarme();
}

// Função responsável por abrir ou fechar a trava utilizando a posição do servo motor
int abrirTrava(int abrir)
{
  if(abrir == 0){
    for (pos = 0; pos <= 180; pos += 1) { 
      myservo.write(pos)//Responder 10: O que este trecho faz?;              
      delay(15);  
    }
  }
  if(abrir ==1){
    for (pos = 180; pos >= 0; pos -= 1) { 
      myservo.write(pos);              
      delay(15);  
    }
  }
}

//Função responsável por escrever alguma mensagem na tela lcd 
void escreveTela(int linha, char mensagem[16]){  
  if(linha == 0){
    lcd_1.begin(16, 2);
    lcd_1.print(mensagem);
  }
  if(linha == 1){
    lcd_1.setCursor(0, 1);
    lcd_1.print(mensagem);
    delay(500); 
    lcd_1.setBacklight(1);
    delay(500);
  }
}

// Função responsável por apagar oquê estiver escrito na tela
void limpaTela(){
  //Pesquisar no site Arduino.cc uma função que limpe a tela lcd
}

// função setup...
void setup() {

  Serial.begin(9600);
  escreveTela(0,"Bem vindo!");
  escreveTela(1, "Iniciando...");
  myservo.attach(9);
  pinMode(botLigar,//Insereir valor );
  pinMode(botTestar,//Insereir valor );
  pinMode(ledLigado,//Insereir valor );
  pinMode(ledAlarme,//Insereir valor );
  desligaAlarme(); 
}

//Função loop...
void loop() {
  
  ligar = digitalRead(//Informar botão responsável por ligar);
  testar = digitalRead(//Informar botão responsável por testar ou desligar);
  
  
  if (//Programar: Se sistema for igua a false){
    if(//Programar: Se a variavel que recebe a informação do botão de ligar for nível lógico alto) {
      //Programar:chamar a função que liga o sistema    
    }
    if(//Programar: Se a variavel que recebe a informação do botão de testar for nível lógico alto){
      //Programar: Chamar a função que testa o sistema -> testaAlarme();
    }
  }
  else if(sistemaLigado == true){
    do{
      //Programar: Chamar a função que escaneia
      //Programar: Ler o botão testar ou desligar
      if(sistemaLigado == true && testar == HIGH){
        //Programar: Chamar a função que desliga o alarme -> desligaAlarme();
        //Programar: O sistema deve aguardar 3 segundos
      }
    }while(sistemaLigado == true);
  }
}