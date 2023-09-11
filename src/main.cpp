#include <Arduino.h>
#include <Servo.h>

#define SERVO_ROLL_PIN D0
#define SERVO_PITCH_PIN D2

// Define the servos
Servo servoRoll;
Servo servoPitch;

// Input buffers
String input_msg_r = "";
String input_msg_p = "";
char input_char;

int current_roll = 90;
int current_pitch = 90;
int roll_position = 0;
int pitch_position = 0;

unsigned long current_time = 0;
unsigned long time_taken = 0;


void setup()
{
  Serial.begin(115200);
  servoRoll.attach(SERVO_ROLL_PIN, 500, 2400);

  servoPitch.attach(SERVO_PITCH_PIN, 500, 2400);

  // Set servos to 90
  servoRoll.write(current_roll);
  servoPitch.write(current_pitch);
}

void crop_angle(int &angle)
{
  if (angle > 180)
  {
    angle = 180;
  }
  else if (angle < 0)
  {
    angle = 0;
  }
}

void loop()
{
  input_msg_r = "";
  input_msg_p= "";

  // Read the incoming angle which is 3 bytes
  // Mesure current time
  current_time = millis();

  if (Serial.available() > 0)
  {
    // The first character should be 'a'
    input_char = Serial.read();

    if (input_char == 'a')
    {
      // Read servo 1
      while (input_msg_r.length() < 3)
      {
        // Read 3 characters
        if (Serial.available() > 0)
        {
          input_char = Serial.read();
          input_msg_r += input_char;
        }
      }

      // Read servo 2
      while (input_msg_p.length() < 3)
      {
        // Read 3 characters
        if (Serial.available() > 0)
        {
          input_char = Serial.read();
          input_msg_p += input_char;
        }
      }

      // Convert the string to an integer
      current_roll = input_msg_r.toInt();
      current_pitch = input_msg_p.toInt();

      // Crop the angle
      crop_angle(current_roll);
      crop_angle(current_pitch);

      // Write the angle to the servo
      servoRoll.write(current_roll);
      servoPitch.write(current_pitch);

      // Get servo position
      roll_position = servoRoll.read();
      pitch_position = servoPitch.read();

      Serial.print("r");
      Serial.print(roll_position);
      Serial.print("p");
      Serial.print(pitch_position);
      Serial.print("t");
      // Get time difference
      time_taken = millis() - current_time;
      Serial.print(time_taken);
      Serial.print("e");
    }
  }
}
