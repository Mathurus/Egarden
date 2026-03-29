#include <LiquidCrystal.h>
#include <Servo.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int bouton1 = 8;
const int motorpin = 9;
const int mesure = A5;
const int capteur_humidite = A0;
const int capteur_luminosite = A1;
const int capteur_gaz = A2;
const int capteur_humidite_air = A4; 
const int motorpin2 = 6;

int etat = 0;                  
bool ancienEtatBouton = HIGH;  
Servo monservo_1;
Servo monservo_2;

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  monservo_1.attach(9);
  monservo_2.attach(7);
  pinMode(bouton1, INPUT_PULLUP); 
  pinMode(10, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(13, OUTPUT);
}

void loop() {
  humidifi();
  ouverture();
  refroidir();
  ajouter_lumunosite(); 
  rechauffer();
  limiter_humi_air();
  bool etatBouton = digitalRead(bouton1);

  if (etatBouton == LOW && ancienEtatBouton == HIGH) {
    etat++;
    if (etat > 4) etat = 0;   
    lcd.clear();
  }
  
  int sensor3 = analogRead(capteur_gaz);
  long sensor50 = (long)sensor3 * 100 / 380;
  if (sensor50 >= 55) {
    afficher_gaz_secour();
  }


  else {
    ancienEtatBouton = etatBouton;

    switch (etat) {
      case 0:
        afficher_temperature();
        break;

      case 1:
        afficher_humidite();
        break;

      case 2:
        afficher_gaz();
        break;

      case 3:
        afficher_luminosite();
        break;

      case 4:
        afficher_humidite_air();
        break;  
    }
  }
}

void afficher_temperature() {
  int reading = analogRead(mesure);
  float voltage = reading * 5.0 / 1023.0;
  int temperatureC = (voltage - 0.5) * 100;

  lcd.setCursor(0, 0);
  lcd.print("Temperature:");
  lcd.setCursor(0, 1);
  lcd.print(temperatureC);
  lcd.print(" C   ");
}

void afficher_humidite() {

  delay(10); 
  int value_sol = analogRead(capteur_humidite);
  long humidite_sol = (long)value_sol * 100 / 876;
  lcd.setCursor(0, 0);
  lcd.print("Humidite sol:");
  lcd.setCursor(0, 1);
  lcd.print(humidite_sol);
  lcd.print("     ");
}


void afficher_luminosite() {
  delay(50);
  int valeur = analogRead(capteur_luminosite);
  long pourcentge_valeur = (long)valeur * 100 / 471;

  lcd.setCursor(0, 0);
  lcd.print("Luminosite:");
  lcd.setCursor(0, 1);
  lcd.print(pourcentge_valeur);
  lcd.print("     ");
}

void afficher_gaz() {
  delay(50);
  int sensor = analogRead(capteur_gaz);
  long sensor49 = (long)sensor * 100 / 380;
  lcd.setCursor(0, 0);
  if (sensor49 >= 70) {
    lcd.print("Gaz ELEVE !");
  } else {
    lcd.print("Gaz:");
  }

  lcd.setCursor(0, 1);
  lcd.print(sensor49);
  lcd.print("     ");
}

void afficher_gaz_secour() {
  int sensor2 = analogRead(capteur_gaz);
  long sensor48 = (long)sensor2 * 100 / 380;
  lcd.setCursor(0, 0);
  lcd.print("Gaz ELEVE :");
  lcd.setCursor(0,1);
  lcd.print(sensor48);
  delay(50);
  lcd.clear();
}

void afficher_humidite_air() {
  delay(10);
  int valeur4 = analogRead(capteur_humidite_air);
  long humidite = (long)valeur4 * 100 / 876;
  lcd.setCursor(0,0);
  lcd.print("Humidite air:");
  lcd.setCursor(0,1);
  lcd.print("Raw: ");
  lcd.print(humidite);
  lcd.print("    ");  
}

void allumer_moteur() {
  int reading = analogRead(mesure);
  float voltage = reading * 5.0 / 1023.0;
  int temperatureC = (voltage - 0.5) * 100;

  if (temperatureC >= 22) {
    digitalWrite(motorpin, HIGH);
  } 
  else {
    digitalWrite(motorpin, LOW);
  }
}

void humidifi() {
  delay(10); 
  int value_sol = analogRead(capteur_humidite);
  long humidite_sol = (long)value_sol * 100 / 876;
  long degre = monservo_1.read();
  if (humidite_sol <= 45)  {
  	monservo_1.write(180);
  }
  if (humidite_sol > 45) {
  monservo_1.write(90);
  }
}

void ouverture() {
  delay(10);
  int sensor2 = analogRead(capteur_gaz);
  long sensor48 = (long)sensor2 * 100 / 380;
  if (sensor48 >= 55 ) {
    monservo_2.write(180);
  }
  else {
    monservo_2.write(0);
  }
}

void refroidir() {
  int reading = analogRead(mesure);
  float voltage = reading * 5.0 / 1023.0;
  int temperatureC = (voltage - 0.5) * 100;
  
  if (temperatureC >= 22) {
    digitalWrite(10, HIGH);
  }
  else {
    digitalWrite(10, LOW);
  }
}

void ajouter_lumunosite() {
  delay(10);
  int valeur48 = analogRead(capteur_luminosite);
  long valeur_pourcentage = (long)valeur48 * 100 / 471;
  
  if (valeur_pourcentage <= 20) {
    digitalWrite(6, HIGH);
  } 
  else {
    digitalWrite(6, LOW);
  }
}

void rechauffer() {
  delay(10);
  int reading = analogRead(mesure);
  float voltage = reading * 5.0 / 1023.0;
  int temperatureC = (voltage - 0.5) * 100;
  
  if (temperatureC <= 18) {
    digitalWrite(13, HIGH);
  }
  else {
    digitalWrite(13, LOW);
  }
}

void limiter_humi_air() {
  delay(10);
  int valeur4 = analogRead(capteur_humidite_air);
  long humidite = (long)valeur4 * 100 / 876;
  
  if (humidite >= 45) {
    digitalWrite(10, HIGH);
  }
  else {
    digitalWrite(10, LOW);
  }
}
