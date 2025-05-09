#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>
#include <Servo.h>

Adafruit_8x16matrix matrix = Adafruit_8x16matrix();
Servo myServo;

const int trigPin = 12;
const int echoPin = 13;

void setup() {
  Serial.begin(9600);

  pinMode(2, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  myServo.attach(A3);
  myServo.write(90);

  matrix.begin(0x70);
  matrix.clear();
  matrix.writeDisplay();
}

void loop() {
  int front = getDistanceCM();

  if (front > 20) {
    avanzar();
    mostrarFlecha("->");
  } else {
    detener();
    mostrarTexto("STOP");
    delay(500);

    myServo.write(160);
    delay(400);
    int izquierda = getDistanceCM();

    myServo.write(20);
    delay(400);
    int derecha = getDistanceCM();

    myServo.write(90);
    delay(200);

    if (izquierda > 20) {
      girarIzquierda();
      mostrarFlecha("<-");
      delay(600);
    } else if (derecha > 20) {
      girarDerecha();
      mostrarFlecha("->");
      delay(600);
    } else {
      retroceder();
      mostrarTexto("BACK");
      delay(800);
    }
  }

  delay(100);
}

void avanzar() {
  digitalWrite(2, HIGH);
  analogWrite(5, 150);
  digitalWrite(4, HIGH);
  analogWrite(6, 150);
}

void detener() {
  digitalWrite(2, LOW);
  analogWrite(5, 0);
  digitalWrite(4, LOW);
  analogWrite(6, 0);
}

void girarIzquierda() {
  digitalWrite(2, LOW);
  analogWrite(5, 0);
  digitalWrite(4, HIGH);
  analogWrite(6, 150);
}

void girarDerecha() {
  digitalWrite(2, HIGH);
  analogWrite(5, 150);
  digitalWrite(4, LOW);
  analogWrite(6, 0);
}

void retroceder() {
  digitalWrite(2, LOW);
  analogWrite(5, 150);
  digitalWrite(4, LOW);
  analogWrite(6, 150);
}

void mostrarFlecha(String direccion) {
  matrix.clear();
  matrix.setCursor(0, 0);
  matrix.print(direccion);
  matrix.writeDisplay();
}

void mostrarTexto(String texto) {
  matrix.clear();
  matrix.setCursor(0, 0);
  matrix.print(texto);
  matrix.writeDisplay();
}

int getDistanceCM() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;
  return distance;
}
