void setup() {
  Serial.begin(9600);

  // put your setup code here, to run once:
  pinMode(2, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  digitalWrite(2, LOW);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);

  pinMode(3, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);

  delay(5000);
  for (int i = 0; i < 100; i++) { 
    Keyboard.press(KEY_RIGHT_ARROW);
    Keyboard.release(KEY_RIGHT_ARROW);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("1: ");
  Serial.print(digitalRead(3));
  Serial.print(" 2: ");
  Serial.print(digitalRead(4));
  Serial.print(" 3: ");
  Serial.println(digitalRead(5));

  delay(20);
}
