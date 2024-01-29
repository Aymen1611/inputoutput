const int trigPin = 12;
const int echoPin = 8;
const int ledPin = 2;
const int ledPin2 = 5;

struct DetectieStatus {
  int teller;
  boolean lichtenAan;
  unsigned long previousMillis;
};

DetectieStatus detectie;

const long interval = 5000;  // Interval van 5 seconden
const long lichtenAanInterval = 1000;  // Interval van 1 seconde voor het uitschakelen van de lichten
const float detectieAfstand = 30.0;  // Afstand in centimeters om de LED's aan te zetten

void setup() {
    Serial.begin(115200);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(ledPin, OUTPUT);
    pinMode(ledPin2, OUTPUT);
    digitalWrite(ledPin, LOW);
    digitalWrite(ledPin2, LOW);

    // Initialisatie van de detectie status
    detectie.teller = 0;
    detectie.lichtenAan = false;
}

void triggerUltrasoneSensor() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
}

float meetAfstand() {
    float duration = pulseIn(echoPin, HIGH);
    return (duration * 0.0343) / 2;  // Deel door 2 omdat we de enkele afstand willen
}

void updateLichten(boolean status) {
    digitalWrite(ledPin, status ? HIGH : LOW);
    digitalWrite(ledPin2, status ? HIGH : LOW);
}

void loop() {
    unsigned long currentMillis = millis();

    triggerUltrasoneSensor();
    float afstand = meetAfstand();

    Serial.print("Afstand: ");
    Serial.println(afstand);

    if (afstand < detectieAfstand) {
        detectie.teller++;
        Serial.println(detectie.teller);

        if (detectie.teller >= 5 && !detectie.lichtenAan) {
            updateLichten(true);
            Serial.println("Lichten aan!");

            detectie.lichtenAan = true;
            detectie.previousMillis = currentMillis;
        }
    }

    // Controleer of het interval voor het uitschakelen van de lichten is verstreken
    if (detectie.lichtenAan && currentMillis - detectie.previousMillis >= lichtenAanInterval) {
        updateLichten(false);
        Serial.println("Lichten uit!");

        // Reset de teller en update de vorige tijd
        detectie.teller = 0;
        detectie.lichtenAan = false;
        detectie.previousMillis = currentMillis; // Reset de timer voor het uitschakelen van de lichten
    }

    // Controleer of het interval is verstreken
    if (detectie.lichtenAan && currentMillis - detectie.previousMillis >= interval) {
        updateLichten(false);
        Serial.println("Lichten uit!");

        // Reset de teller en update de vorige tijd
        detectie.teller = 0;
        detectie.lichtenAan = false;
    }

    // Reduceer de vertraging om de detectiesnelheid te verhogen
    delay(40);
}
