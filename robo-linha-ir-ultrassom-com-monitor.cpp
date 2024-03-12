#include <Arduino.h>

// Definição dos pinos
const int sensorEsquerdo = 8;
const int sensorDireito = 9;
const int trigPin = 10;
const int echoPin = 11;
const int transistorEsquerdo = 6;
const int transistorDireito = 5;
const int botaoPin = 12;

// Variáveis
bool estadoBotao = false;
bool estadoBotaoAnterior = false;
long duracao, distancia;
float velocidadeEsquerda, velocidadeDireita;

// Função para configurar os pinos
extern "C" void setup() {
  pinMode(sensorEsquerdo, INPUT);
  pinMode(sensorDireito, INPUT);
  pinMode(transistorEsquerdo, OUTPUT);
  pinMode(transistorDireito, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(botaoPin, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("Inicialização completa. Aguardando botão de controle...");
}

// Função principal
extern "C" void loop() {
  // Leitura do estado do botão
  bool botaoPressionado = digitalRead(botaoPin) == LOW;
  if (botaoPressionado && !estadoBotaoAnterior) {
    estadoBotao = !estadoBotao;
    if (estadoBotao) {
      Serial.println("Botão de controle pressionado. Iniciando movimento do robô...");
    } else {
      Serial.println("Botão de controle liberado. Parando movimento do robô...");
    }
  }
  estadoBotaoAnterior = botaoPressionado;

  // Se o botão estiver desligado, pare o robô
  if (!estadoBotao) {
    digitalWrite(transistorEsquerdo, LOW);
    digitalWrite(transistorDireito, LOW);
    return;
  }

  // Leitura dos sensores
  int leituraEsquerda = digitalRead(sensorEsquerdo);
  int leituraDireita = digitalRead(sensorDireito);

  // Leitura do sensor ultrassônico
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duracao = pulseIn(echoPin, HIGH);
  distancia = (duracao / 2) / 29.1; // Conversão para cm
  Serial.print("Distância medida pelo sensor ultrassônico: ");
  Serial.print(distancia);
  Serial.println(" cm");

  // Controle de velocidade com feedback
  velocidadeEsquerda = 100;
  velocidadeDireita = 100; // Velocidade inicial (50% da capacidade)
  if (distancia < 10) { // Se um objeto estiver próximo, freie o robô
    velocidadeEsquerda = velocidadeDireita = 0;
    Serial.println("Objeto detectado próximo. Freando o robô...");
  } else if (leituraEsquerda == HIGH && leituraDireita == LOW) {
    // Obstáculo à esquerda, virar para a direita
    velocidadeEsquerda = 0;
    velocidadeDireita = 100;
    Serial.println("Obstáculo detectado à esquerda. Virando para a direita...");
  } else if (leituraEsquerda == LOW && leituraDireita == HIGH) {
    // Obstáculo à direita, virar para a esquerda
    velocidadeEsquerda = 100;
    velocidadeDireita = 0;
    Serial.println("Obstáculo detectado à direita. Virando para a esquerda...");
  }

  // Aceleração gradativa e suave dos motores
  for (int i = 0; i < 10; i++) {
    analogWrite(transistorEsquerdo, velocidadeEsquerda * i / 10);
    analogWrite(transistorDireito, velocidadeDireita * i / 10);
    delay(10);
  }
}
