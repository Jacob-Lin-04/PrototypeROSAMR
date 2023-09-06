#include <ros.h>
#include <geometry_msgs/Twist.h>

#define IN1A 3  // Left motor forward
#define IN1B 5  // Left motor reverse
#define IN2A 6  // Right motor forward
#define IN2B 9  // Right motor reverse

ros::NodeHandle nh;

void setMotorSpeeds(int left_speed, int right_speed) {
  // Motor A (Left motor)
  if (left_speed >= 0) {
    analogWrite(IN1A, left_speed);
    analogWrite(IN1B, 0);
  } else {
    analogWrite(IN1A, 0);
    analogWrite(IN1B, -left_speed);
  }
  
  // Motor B (Right motor)
  if (right_speed >= 0) {
    analogWrite(IN2A, right_speed);
    analogWrite(IN2B, 0);
  } else {
    analogWrite(IN2A, 0);
    analogWrite(IN2B, -right_speed);
  }
}

void driveCallback(const geometry_msgs::Twist& twist_msg) {
  float linear_speed = twist_msg.linear.x;
  float angular_speed = twist_msg.angular.z;

  int left_motor_speed = constrain((linear_speed - angular_speed) * 255, -100, 100);
  int right_motor_speed = constrain((linear_speed + angular_speed) * 255, -100, 100);

  // Set the motor speeds
  setMotorSpeeds(left_motor_speed, right_motor_speed);
}

ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", driveCallback);

void setup() {
  pinMode(IN1A, OUTPUT);
  pinMode(IN1B, OUTPUT);
  pinMode(IN2A, OUTPUT);
  pinMode(IN2B, OUTPUT);

  nh.initNode();
  nh.subscribe(sub);
}

void loop() {
  nh.spinOnce();
  delay(10);
}
