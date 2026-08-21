//Hey Yall! Put some comments even if you all ready know the coding launguage
#include <LiquidCrystal.h>

// RS, E, D4, D5, D6, D7 LCD Pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Hardware pins For The piezo + LED
const int buzzerPin = 13;
const int ledPin = 10;

// Frequencies for a C Minor Arpeggio (C4, Eb4, G4, C5)
const int cMinorChord[] = {262, 311, 392, 523}; 

//Starting Message
String currentMsg = "PITCH BLACK";
unsigned long lastScrollTime = 0;
int scrollPos = 0;
const int scrollDelay = 350; 

//Set Up. Loop
void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  displayMessage(currentMsg);
}

//Main loop
void loop() {
  if (Serial.available() > 0) {
    String incoming = Serial.readStringUntil('\n');
    incoming.trim();
    if (incoming.length() > 0) {
      currentMsg = incoming;
      scrollPos = 0; 
      displayMessage(currentMsg);

      if (currentMsg.indexOf("CRASH!") >= 0) {
        triggerCliffDeathSequence();
      }
    }
  }

  if (currentMsg.length() > 16) {
    if (millis() - lastScrollTime > scrollDelay) {
      lastScrollTime = millis();
      scrollMessage();
    }
  }
}

// Triggers a 2-second rapid C Minor arpeggio chord simulation + red strobe
void triggerCliffDeathSequence() {
  unsigned long startTime = millis();
  int chordIndex = 0;

  // Run loops continuously until 2000 milliseconds (2 seconds) have elapsed
  while (millis() - startTime < 2000) {
    // Alternate LED state on every note swap to keep it blinking red
    digitalWrite(ledPin, (chordIndex % 2 == 0) ? HIGH : LOW);
    
    // Play the individual harmonic steps of the C Minor chord rapidly
    tone(buzzerPin, cMinorChord[chordIndex]);
    delay(30); // 30ms per tone creates a cohesive polyphonic illusion
    
    chordIndex = (chordIndex + 1) % 4; // Cycle through C, Eb, G, C
  }
  
  noTone(buzzerPin);          // Silence the buzzer after 2 seconds
  digitalWrite(ledPin, LOW);   // Darken the LED
}

void displayMessage(String msg) {
  lcd.clear();
  if (msg.length() <= 16) {
    lcd.setCursor(0, 0);
    lcd.print(msg);
  } else {
    lcd.setCursor(0, 0);
    lcd.print(msg.substring(0, 16));
  }
}

void scrollMessage() {
  lcd.clear();
  lcd.setCursor(0, 0);
  
  String paddedMsg = currentMsg + "   "; 
  int msgLen = paddedMsg.length();
  
  String displayWindow = "";
  for (int i = 0; i < 16; i++) {
    int index = (scrollPos + i) % msgLen;
    displayWindow += paddedMsg[index];
  }
  
  lcd.print(displayWindow);
  
  scrollPos++;
  if (scrollPos >= msgLen) {
    scrollPos = 0;
  }
}
