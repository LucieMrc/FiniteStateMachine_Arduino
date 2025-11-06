# FiniteStateMachine_Arduino

Un principe d'un jeu de base : 

!['schema'](./images/FiniteStateMachine.png)

Le jeu qu'on va faire ici : 

!['schema'](./images/FiniteStateMachine2.png)

Nos 3 modes :

Attract mode : les leds clignotent
on passe de l'attract mode au mode jeu avec une action spécifique

Mode jeu : on attend l'action des joueurs
on passe du mode jeu au mode perdu/gagné avec les actions de jeu

Mode perdu/gagné qui affiche le gagnant, le score
on passe du mode perdu/gagné à l'attract mode avec un timing en général, ou une action


## Mise en place

### Le montage

!['schema'](./images/montage.png)

Les leds sont branchés sur les pins 6 et 7, et les boutons sont sur les pins 8 et 9.

<!-- photo du montage -->

### Le début du code

D'abord je déclare les pins des leds et boutons au dessus du setup() :
```
int pinLed1 = 6;
int pinLed2 = 7;
int pinBouton1 = 8;
int pinBouton2 = 9;
```

On va également créer une variable `gameState` qui nous permet de naviguer entre les modes. Le mode 0 est l'AttractMode, le mode 1 est le mode de jeu, et le mode 2 est le mode perdu/gagné.

On déclare donc :
```
int gameState = 0; //le mode par défaut est l'Attract Mode
```

Et dans le setup() je déclare le mode des pins :

```
void setup() {
  pinMode(pinLed1, OUTPUT);
  pinMode(pinLed2, OUTPUT);
  pinMode(pinBouton1, INPUT);
  pinMode(pinBouton2, INPUT);
}
```

## Attract Mode

Dans l'Attract Mode, je veux que les deux leds clignotent en décalé toutes les secondes, et qu'on quitte l'Attract Mode en appuyant sur les deux boutons en même temps.

### Créer l'animation

Pour faire clignoter une led, on pourrait écrire dans le loop() : 

````
void loop() {

digitalWrite(pinLed1, HIGH); //la led s'allume
delay(1000); //on attend 1 seconde
digitalWrite(pinLed1, LOW); //la led s'éteinte
delay(1000); //on attend 1 seconde
}
````
Mais dans notre cas, on ne veux pas utiliser la fonction ``delay()``car elle bloque le code, et on ne recevrait donc pas l'information des boutons qui nous ferait quitter l'Attract Mode.

On va donc plutôt utiliser la fonction `millis()`, qui donne (en millisecondes) le temps depuis lequel la carte Arduino est allumée.

On va donc plutôt créer un système avec une variable `timing`. On va écrire une condition qui définit que quand `millis()` (= "l'heure" qu'il est) est plus grand que `timing` + 1 seconde, alors on change l'état de la led (on l'allume ou on l'éteint) et on redéfinit `timing`.

On définit donc la variable `timing` au dessus du setup(), avec la ligne :
````
long timing = 0;
````
Et on crée notre condition dans le loop() :

````
void loop() {

  if (millis() > timing + 1000) {
    // on change l'état de la led
    timing = millis();
  }
}
````

Pour changer l'état de la led, on crée un booléen `ledState1` pour savoir si la led était éteinte ou allumée précédemment, et donc si on doit l'éteindre ou l'allumer.

On définit donc la variable `ledState1` au dessus du setup(), avec la ligne :
````
bool ledState1 = LOW;
````
Et on met à jour notre condition dans le loop() :
````
void loop() {

  if (millis() > timing + 1000) {
    ledState1 =! ledState1; //ledState1 devient le contraire de ledState1
    digitalWrite(pinLed1, ledState1); //on donne l'état ledState1 à la led
    timing = millis();
  }
}
````

Notre led1 clignote, et pour faire en sorte que la led2 clignote en décalé, on prend le même principe mais on définit la variable `ledState2` en HIGH par défaut :
````
bool ledState2 = HIGH;
````
 Dans le loop() :

```
void loop() {

  if (millis() > timing + 1000) {
    ledState1 =! ledState1; //ledState1 devient le contraire de ledState1
    ledState2 =! ledState2; //ledState2 devient le contraire de ledState2
    digitalWrite(pinLed1, ledState1); //on donne l'état ledState1 à la led1
    digitalWrite(pinLed2, ledState2); //on donne l'état ledState2 à la led2
    timing = millis();
  }
}
```

### Limiter l'animation à l'Attract Mode

Pour que le clignotement des leds ne soit que dans l'Attract Mode, on va créer une condition prenant en compte l'état de la variable `gameState`.

```
void loop() {

  if (gameState == 0) {  //si on est dans l'Attract Mode
    if (millis() > timing + 1000) {
      ledState1 = !ledState1;            //ledState1 devient le contraire de ledState1
      ledState2 = !ledState2;            //ledState2 devient le contraire de ledState2
      digitalWrite(pinLed1, ledState1);  //on donne l'état ledState1 à la led1
      digitalWrite(pinLed2, ledState2);  //on donne l'état ledState2 à la led2
      timing = millis();
    }
  }
}
```

À la fin de notre condition du `gameState`, on va créer une condition qui définit que si on appuie sur les deux boutons en même temps, alors on passe du `gameState` 0 au `gameState1`

On veux donc créer la condition suivante :

```
    if (digitalRead(pinBouton1) == 1 && digitalRead(pinBouton2) == 1) {
      gameState = 1;
    }
```

On peux simplifier en créant des variables int `buttonState1` et `buttonState2`, et en écrivant dans le loop() :
```
buttonState1 = digitalRead(pinBouton1);
buttonState2 = digitalRead(pinBouton2);
```

Afin de pouvoir écrire 
```
    if (buttonState1 == 1 && buttonState2 == 1) {
      gameState = 1;
    }
```

On a donc l'Attract Mode avec le clignotement des leds, qu'on quitte en appuyant sur les deux boutons en même temps.

<details>

<summary> Code complet à cette étape</summary>

```
int pinLed1 = 6;
int pinLed2 = 7;
int pinBouton1 = 8;
int pinBouton2 = 9;

int gameState = 0;  //le mode par défaut est l'Attract Mode

buttonState1 = digitalRead(pinBouton1);
buttonState2 = digitalRead(pinBouton2);

long timing = 0;
bool ledState1 = LOW;
bool ledState2 = HIGH;

void setup() {
  Serial.begin(9600);
  pinMode(pinLed1, OUTPUT);
  pinMode(pinLed2, OUTPUT);
  pinMode(pinBouton1, INPUT);
  pinMode(pinBouton2, INPUT);
}

void loop() {

  if (gameState == 0) {  //si on est dans l'Attract Mode
    if (millis() > timing + 1000) {
      ledState1 = !ledState1;            //ledState1 devient le contraire de ledState1
      ledState2 = !ledState2;            //ledState2 devient le contraire de ledState2
      digitalWrite(pinLed1, ledState1);  //on donne l'état ledState1 à la led1
      digitalWrite(pinLed2, ledState2);  //on donne l'état ledState2 à la led2
      timing = millis();
    }

    if (buttonState1 == 1 && buttonState2 == 1) {
      gameState = 1;
    }
  }
}

```

</details>


## Mode de jeu

## Perdu/gagné
