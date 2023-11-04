  #include <Servo.h>
  #include <LiquidCrystal_I2C.h>
  #include <Wire.h>
  #include <SoftwareSerial.h>
  

  Servo myservo;

  SoftwareSerial SIM900A(10, 11);

  LiquidCrystal_I2C lcd(0x27, 16, 2);

  int motor = 2;
  bool sentMessage = false; // Variable to track if the message has been sent


  void setup()
  {
    SIM900A.begin(9600);

    Serial.begin(9600);

    Serial.println("GSM Ready!");

    delay(1000);

    lcd.init();
    lcd.backlight();

    myservo.attach(9);

    pinMode(motor, OUTPUT);
  }

  void loop()
  {
    if (Serial.available())
    {
      String data = Serial.readStringUntil('\n'); // Read data until a newline character is received
      Serial.println(data);

      int commaIndex = data.indexOf(',');
      String sensorValueData = data.substring(0, commaIndex);
      String recipientData = data.substring(commaIndex + 1);
      int intValue = sensorValueData.toInt();

      Serial.println(intValue);
      Serial.println(recipientData);


      int numRecipients = 5;
      String recipients[numRecipients];

      int currentIndex = 0;
      int prevIndex = 0;
      int recipientCount = 0;

      // Extract recipients from the recipientData string
      for (int i = 0; i < recipientData.length(); i++)
      {
        if (recipientData.charAt(i) == ',')
        {
          recipients[recipientCount] = recipientData.substring(prevIndex, i);
          prevIndex = i + 1;
          recipientCount++;

        }
      }
      // Capture the last recipient
      recipients[recipientCount] = recipientData.substring(prevIndex);

      for (int i = 0; i <= recipientCount; i++)
      {
        Serial.println(recipients[i]);
      }
    
      lcd.setCursor(0, 0);
      lcd.print("Value :");
      lcd.print(intValue);
      lcd.print("   ");
      lcd.setCursor(0, 1);
      lcd.print("Level :");

      if (intValue == 0)
      {
        myservo.write(0);
        lcd.print("Low        ");
        digitalWrite(motor, HIGH);
        sentMessage = false; 
      }

      else if (intValue > 1 && intValue < 389)
      {
        myservo.write(0);
        lcd.print("Low        ");
        digitalWrite(motor, HIGH);
        sentMessage = false; 
      }

      else if (intValue > 390 && intValue < 510)
      {
        myservo.write(80);
        lcd.print("Medium        ");
        digitalWrite(motor, HIGH);
        sentMessage = false; 
      }

      else if (intValue > 511 && !sentMessage)
      {
        myservo.write(100);
        lcd.print("High        ");
        digitalWrite(motor, LOW);
        delay(3000);
        for (int i = 0; i < recipientCount; i++)
        {
          Serial.print("Sending message to: ");
          Serial.println(recipients[i]);
          SendMessage(recipients[i]);
          delay(500); // Adding a delay to ensure proper message transmission between recipients
        }
        delay(5000);
        sentMessage = true; // Set the flag to true to indicate the message has been sent
      }

      else
      {
        myservo.write(0);
        lcd.print("           ");
        digitalWrite(motor, HIGH);
        sentMessage = false; // Reset the flag if the condition is not met
      }
    }
  }

  void SendMessage(String recipient)
  {
    if (SIM900A.available())
    {
      Serial.write(SIM900A.read());
    }

    //Serial.println("Sending Message");
     SIM900A.println("AT+CMGF=1"); // Sets the GSM Module in Text Mode
    delay(1000);

    SIM900A.print("AT+CMGS=\"");
    SIM900A.print(recipient);
    SIM900A.write('"');
    SIM900A.write('\r');
    delay(1000);

    SIM900A.print("Water is High!"); // Message content
    SIM900A.write('\r');

    SIM900A.write((char)26); // ASCII code of CTRL+Z
    delay(1000);
  }
