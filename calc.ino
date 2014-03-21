/* Criado por Jair Guilherme Certório - 2014 */ 
#include <SPI.h>
#include <LiquidCrystal.h>

struct Pad{

  int pino[8];
  unsigned short atual,anterior , estado;
  
  void             Begin(int conex[8]){
    /*
        Ao indicar os pinos conectados ao teclado coloque-os separados por linhas e
        colunas, isto é, não misture ou inverta a ordem das conexões, exemplo:
          Conectores
           ||||IIII   Em uma matriz 4x4 metade dos pinos será conectado em linha
           ||||IIII   e os outros nas colunas, para evitar problemas conecte-os em ordem
           ||||IIII   seguindo sempre da esquerda->direita e mantendo as posições relativas
           ||||IIII   na conexões no Arduino.
           ||||IIII <-- Provável coluna
            ^-- Provável linhas 
    */
      estado=0;
      anterior=0;
      atual=0;
      for(int i=0;i<8;i++){
        pino[i]=conex[i];
        pinMode(pino[i],INPUT_PULLUP);
      }
  };   
  
  unsigned short   rawLer(){
    /*
        Esta leitura retorna os botões que estão apertados no momento e atualiza
        as variáveis com os botões pressionados anteriormente e no 'estado'( ainda antes do anterior),
        USE Ler() PARA OBTER A SAIDA TRATADA (DEBOUNCED).
        Cada tecla é guardada em 1 bit de Leitura, mapeie de acordo o simbolo ou função
        associada a cada uma
        Sendo ele todo ocupado, se necessário troque para unsigned long.
    */
      unsigned short Leitura = 0;
      for( int i=0 ; i<4 ; i++ ){
          pinMode(pino[i],OUTPUT);
          digitalWrite(pino[i],LOW);
          for( int j=0 ; j<4 ; j++ ){
              int valor = digitalRead(pino[4+j]);
              if (valor == LOW){
                  Leitura|=(0x01<<(4*j+i));    
              }
          }
          pinMode(pino[i],INPUT_PULLUP);
      }
      estado   = anterior;
      anterior = atual;
      atual    = Leitura;
      return     Leitura;
  }
 
 unsigned short    Ler(){
   /*
        Esta leitura retorna os botões que foram apertados de forma tratada, isto é,
        retorna somente 1 vez a cada pressionar de tecla, sendo bom para digitação e
        troca de estados no uC.
        USE rawLer() PARA OBTER DE SAIDA AS TECLAS ATUALMENTE PRESSIONADAS.
        Cada tecla é guardada em 1 bit de Leitura, mapeie de acordo o simbolo ou função
        associada a cada uma
        Sendo ele todo ocupado, se necessário troque para unsigned long.
    */
     rawLer();
     return (atual&anterior&~estado);
 } 


};

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
Pad teclado;

void setup() {
  
  int entradas[] = {13,10,9,8,7,6,1,0};
  teclado.Begin(entradas);
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.

}

int op = 0;
int valor = 0;
int valor2 = 0;
int cursorPosition = 0;
void loop() {
  
  lcd.setCursor(cursorPosition, 0);
  unsigned short numero = teclado.Ler();
  
  if (numero != 0) {
  
    if (numero == 15 || numero == 87) numero = 1;
    else if(numero == 240) numero = 2;
    else if(numero == 3840) numero = 3;
    else if(numero == 2) numero = 4;
    else if(numero == 32) numero = 5;
    else if(numero == 512) numero = 6;
    else if(numero == 4) numero = 7;
    else if(numero == 64) numero = 8;
    else if(numero == 1024) numero = 9;
    else if(numero == 128) numero = 0;
    else if(numero == 4096) { //soma
      cursorPosition = 0;
      op = 1;
      lcd.clear();
      return;
    }
    else if(numero == 8) { //* multiplicaçao
      cursorPosition = 0;
      op = 2;
      lcd.clear();
      return;
    }
    else if(numero == 2048){// # divisao
      cursorPosition = 0;
      op = 3;
      lcd.clear();
      return;    
    }
    else if(numero == 8192){
      cursorPosition = 0;
      op = 4;
      lcd.clear();
      return;    
    }
     else if(numero == 16384){
      cursorPosition = 0;
      op = 5;
      lcd.clear();
      return;    
    }
    else if(numero == 32768) { // =
      lcd.clear();
      cursorPosition = 0;
      if(op == 1) lcd.print(valor+valor2);
      else if (op == 2) lcd.print(valor*valor2);
      else if (op == 3) lcd.print(valor/valor2);
      else if (op == 4) lcd.print(valor-valor2);
      else if (op == 5) lcd.print("Vai C Fude D7");
      while(true);
    }
    if(op == 0) {
      valor *= 10;
      valor += numero;
    }else{
      valor2 *= 10;
      valor2 += numero;
    }
    lcd.print(numero);
    cursorPosition += 1;
  }
}
