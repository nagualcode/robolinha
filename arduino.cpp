// Projeto de Bloco Robótica - INFNET
// Robo Seguidor de Linha com IR e Ultrassom
// Aluno: Frederico Flores
// Março/2024

const int sensorEsquerdo = 8; // Pino do sensor IR esquerdo
const int sensorDireito = 9; // Pino do sensor IR direito

const int transistorEsquerdo = 6; // Pino do transistor do motor esquerdo
const int transistorDireito = 5; // Pino do transistor do motor direito

const int botaoPin = 12; // Pino do push button

// Variável para armazenar o estado do botão (ligado/desligado)
bool estadoBotao = false;

// Variável para armazenar o estado anterior do botão
bool estadoBotaoAnterior = false;

// Variável para controlar se os sensores devem ser exibidos no Serial
bool exibirSensores = true;

void setup() {
  pinMode(sensorEsquerdo, INPUT); 
  pinMode(sensorDireito, INPUT); 
  pinMode(transistorEsquerdo, OUTPUT); 
  pinMode(transistorDireito, OUTPUT); 

  pinMode(botaoPin, INPUT_PULLUP); 
  
  Serial.begin(9600); 
}

void loop() {
  // Lê o estado atual do botão e inverte quando pressionado
  bool botaoPressionado = digitalRead(botaoPin) == LOW; 

  
  if (botaoPressionado && !estadoBotaoAnterior) {
    
    estadoBotao = !estadoBotao;
  }
  

  estadoBotaoAnterior = botaoPressionado;

  
  if (!estadoBotao) {
    Serial.println("Aperte o botão para ligar");
    digitalWrite(transistorEsquerdo, LOW); 
    digitalWrite(transistorDireito, LOW); 
    delay(100); // Pequeno atraso para evitar flutuações no botão
    return; 
  }

  
  int leituraEsquerda = digitalRead(sensorEsquerdo);
  int leituraDireita = digitalRead(sensorDireito);

 
  if (exibirSensores) {
    Serial.print("Sensor Esquerdo: ");
    Serial.println(leituraEsquerda);
    Serial.print("Sensor Direito: ");
    Serial.println(leituraDireita);
  }

  // Controla os transistores dos motores com base na leitura dos sensores
  if (leituraEsquerda == HIGH && leituraDireita == LOW) {
    Serial.println("obstáculo à esquerda, vire para a direita ->");
    digitalWrite(transistorEsquerdo, HIGH); 
    digitalWrite(transistorDireito, LOW); 
  } else if (leituraEsquerda == LOW && leituraDireita == HIGH) {
    Serial.println("obstáculo à direita, vire para a esquerda <-");
    digitalWrite(transistorEsquerdo, LOW); 
    digitalWrite(transistorDireito, HIGH); 
  } else if (leituraEsquerda == HIGH && leituraDireita == HIGH) {
    Serial.println("Obstáculos em ambos os lados, pare <>");
    digitalWrite(transistorEsquerdo, LOW); 
    digitalWrite(transistorDireito, LOW); 
  } else {
    Serial.println("--");
    digitalWrite(transistorEsquerdo, HIGH); 
    digitalWrite(transistorDireito, HIGH); 
  }
  if (exibirSensores) {
    delay(100); // Pequeno atraso entre as iterações do loop para ler no monitor
  }
}
