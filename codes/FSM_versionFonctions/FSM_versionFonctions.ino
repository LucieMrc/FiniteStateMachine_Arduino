int pinLed1 = 6;
int pinLed2 = 7;
int pinBouton1 = 8;
int pinBouton2 = 9;

int buttonState1;
int buttonState2;

int gameState = 0;  //le mode par défaut est l'Attract Mode

long timing = 0;
bool ledState1 = LOW;
bool ledState2 = HIGH;

long startLed;
long startGame;

int whichLed;
int winner;

int count;

void setup() {
  Serial.begin(9600);
  pinMode(pinLed1, OUTPUT);
  pinMode(pinLed2, OUTPUT);
  pinMode(pinBouton1, INPUT);
  pinMode(pinBouton2, INPUT);

  randomSeed(analogRead(0));
}

void loop() {

  buttonState1 = digitalRead(pinBouton1);
  buttonState2 = digitalRead(pinBouton2);
  if (gameState == 0) {  //si on est dans l'Attract Mode
    attractMode();
  } else if (gameState == 1) {  //si on est dans le mode jeu
    jeuMode();
  } else if (gameState == 2) {  //si on est dans le mode perdu/gagné
    gagnantMode();
  }
}

void attractMode() {
  if (millis() > timing + 1000) {
    ledState1 = !ledState1;            //ledState1 devient le contraire de ledState1
    ledState2 = !ledState2;            //ledState2 devient le contraire de ledState2
    digitalWrite(pinLed1, ledState1);  //on donne l'état ledState1 à la led1
    digitalWrite(pinLed2, ledState2);  //on donne l'état ledState2 à la led2
    timing = millis();
  }
  if (buttonState1 == 1 && buttonState2 == 1) {
    startGame = millis();
    startLed = random(5000, 15000);  //le laps de temps random est entre 5 et 15secondes
    whichLed = random(1, 3);
    ledState1 = LOW;
    ledState2 = LOW;
    digitalWrite(pinLed1, ledState1);
    digitalWrite(pinLed2, ledState2);
    gameState = 1;
  }
}

void jeuMode() {
  if (millis() > startGame + startLed) {  //si le laps de temps random est écoulé
    if (whichLed == 1) {
      ledState1 = HIGH;
      digitalWrite(pinLed1, ledState1);
    } else if (whichLed == 2) {
      ledState2 = HIGH;
      digitalWrite(pinLed2, ledState2);
    }

    if (buttonState1 == HIGH) {  //si on appuie sur le bouton1
      gameState = 2;
      ledState1 = LOW;
      ledState2 = LOW;
      digitalWrite(pinLed1, ledState1);
      digitalWrite(pinLed2, ledState2);
      winner = 1;
      timing = millis();
    } else if (buttonState2 == HIGH) {  //si on appuie sur le bouton2
      gameState = 2;
      ledState1 = LOW;
      ledState2 = LOW;
      digitalWrite(pinLed1, ledState1);
      digitalWrite(pinLed2, ledState2);
      winner = 2;
      timing = millis();
    }
  }
}

void gagnantMode() {
  if (millis() > timing + 200) {
    if (winner = 1) {
      ledState1 = !ledState1;            //ledState1 devient le contraire de ledState1
      digitalWrite(pinLed1, ledState1);  //on donne l'état ledState1 à la led1
    } else if (winner == 2) {
      ledState2 = !ledState2;            //ledState2 devient le contraire de ledState2
      digitalWrite(pinLed2, ledState2);  //on donne l'état ledState2 à la led2
    }
    count = count + 1;
    timing = millis();
    if (count == 25) {
      gameState = 0;
      count = 0;
      winner = 0;
      ledState1 = LOW;
      ledState2 = HIGH;  //pour que les leds alternent dans l'attract mode
    }
  }
}
