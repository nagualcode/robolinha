// Projeto de Bloco Robótica - INFNET
// Robo Seguidor de Linha com IR e Ultrassom
// Aluno: Frederico Flores
// Março/2024

const int sensorEsquerdo = 8; // Pino do sensor IR esquerdo
const int sensorDireito = 9; // Pino do sensor IR direito
const int trigPin = 10; // Pino do trigger do sensor de ultrassom
const int echoPin = 11; // Pino do echo do sensor de ultrassom

const int transistorEsquerdo = 6; // Pino do transistor do motor esquerdo
const int transistorDireito = 5; // Pino do transistor do motor direito

const int botaoPin = 12; // Pino do push button

// Variável para armazenar o estado do botão (ligado/desligado)
bool estadoBotao = false;

// Variável para armazenar o estado anterior do botão
bool estadoBotaoAnterior = false;

// Variável para controlar se os sensores devem ser exibidos no Serial
bool exibirSensores = false;

void setup() {
  pinMode(sensorEsquerdo, INPUT); 
  pinMode(sensorDireito, INPUT); 
  pinMode(transistorEsquerdo, OUTPUT); 
  pinMode(transistorDireito, OUTPUT); 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
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
      if (exibirSensores) {
    Serial.println("Aperte o botão para ligar");
      }
    digitalWrite(transistorEsquerdo, HIGH); 
    digitalWrite(transistorDireito, HIGH); 
    delay(100); // Pequeno atraso para evitar flutuações no botão
    return; 
  }

  
  int leituraEsquerda = digitalRead(sensorEsquerdo);
  int leituraDireita = digitalRead(sensorDireito);

  // Lê a distância do sensor ultrassônico
  long duracao, distancia;
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
  duracao = pulseIn(echoPin, HIGH);
  distancia = (duracao / 2) / 29.1; // Convertendo para centímetros
  
  if (exibirSensores) {
    Serial.print("Distancia do objeto: ");
    Serial.print(distancia);
    Serial.println(" cm");
  }

  // Limita a velocidade a 0 se um objeto estiver a 5 cm ou menos
  if (distancia <= 10) {
    digitalWrite(transistorEsquerdo, HIGH); 
    digitalWrite(transistorDireito, HIGH);
    Serial.println("Objeto muito próximo, parando o robô");
    delay(100); // Pequeno atraso antes de verificar novamente
    return;
  }

  // Controla os transistores dos motores com base na leitura dos sensores
  if (leituraEsquerda == HIGH && leituraDireita == LOW) {
    Serial.println("obstáculo à esquerda, vire para a direita ->");
    digitalWrite(transistorEsquerdo, LOW); 
    digitalWrite(transistorDireito, HIGH); 
  } else if (leituraEsquerda == LOW && leituraDireita == HIGH) {
    Serial.println("obstáculo à direita, vire para a esquerda <-");
    digitalWrite(transistorEsquerdo, HIGH); 
    digitalWrite(transistorDireito, LOW); 
  } else if (leituraEsquerda == HIGH && leituraDireita == HIGH) {
    Serial.println("Obstáculos em ambos os lados, pare <>");
    digitalWrite(transistorEsquerdo, HIGH); 
    digitalWrite(transistorDireito, HIGH); 
  } else {
    Serial.println("siga reto --");
    digitalWrite(transistorEsquerdo, LOW); 
    digitalWrite(transistorDireito, LOW); 
  }
  if (exibirSensores) {
    delay(100); // Pequeno atraso entre as iterações do loop para ler no monitor
  }
}
