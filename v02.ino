#include <Servo.h>  


const int speed_mult = 200; // 200

const int J1_dir_pin = 23;
const int J1_step_pin = 22;

const int J2_dir_pin = 25;
const int J2_step_pin = 24;

const int J3_dir_pin = 27;
const int J3_step_pin = 26;

const int J4_dir_pin = 29;
const int J4_step_pin = 28;

const int J5_dir_pin = 31;
const int J5_step_pin = 30;

const int J6_dir_pin = 33;
const int J6_step_pin = 32;

const bool J1_start_dir = LOW;
const bool J2_start_dir = LOW;
const bool J3_start_dir = LOW;
const bool J4_start_dir = LOW;
const bool J5_start_dir = HIGH;
const bool J6_start_dir = LOW;

const int switch1_pin = 41;
const int switch2_pin = 44;
const int switch3_pin = 42;
const int switch4_pin = 43;
const int switch5_pin = 40;
const int switch6_pin = 45;


const int relay_pin_1 = 34;
const int relay_pin_2 = 35;
const int relay_pin_3 = 36;
const int relay_pin_4 = 37;
const int relay_pin_5 = 38;
const int relay_pin_6 = 39;


const long int J1_range = 171881; //21274 //  20220
const long int J2_range = 29014;
const long int J3_range = 31529; //26326
const long int J4_range = 15676;
const long int J5_range = 3217;
const long int J6_range = 44003;


bool motor_calibrate = false;
bool position_first = false;

int motor_n_c = 6;
int motor_n_p = 6;


int count = 0;

class Motor {
  private:
    int dir_pin;
    int step_pin;
    int switch_pin;
    int start_dir;
    bool is_calibrate = false;
    bool position_ok = false;
    long long curent_position;
    int SPEED_MULT;
    

  public:

    Motor(int dir_, int step_, int switch_, int start_, int speed_m) {
      dir_pin = dir_;
      step_pin = step_;
      switch_pin  = switch_;
      start_dir = start_;
      SPEED_MULT = speed_m;
    }


  void calibrate() {
    if(!is_calibrate){

      if(digitalRead(switch_pin) == LOW) {
        digitalWrite(dir_pin, start_dir);
        digitalWrite(step_pin,HIGH); 
        delayMicroseconds(SPEED_MULT/motor_n_p);
        digitalWrite(step_pin,LOW); 
        count++;
      }

       else {
          is_calibrate = true;
          curent_position = 0;
          motor_n_c -= 1;
       }
    }


    if(digitalRead(switch_pin) == HIGH) {
          digitalWrite(dir_pin, !start_dir);
          digitalWrite(step_pin,HIGH); 
          delayMicroseconds(SPEED_MULT/motor_n_p);
          digitalWrite(step_pin,LOW); 
    } 
  }  
   
  void set_position(long x) {
    if(is_calibrate && !position_ok) {

      if(curent_position == x)
      {
        position_ok = true;
        motor_n_p -= 1;
        return;
      }
        
      
      if(x > curent_position)
      {
        digitalWrite(dir_pin, !start_dir);
        digitalWrite(step_pin,HIGH);
        delayMicroseconds(SPEED_MULT/motor_n_p);
        digitalWrite(step_pin,LOW);
        curent_position += 1;
      }
      
      else if(x < curent_position )
      {
        digitalWrite(dir_pin, start_dir);
        digitalWrite(step_pin,HIGH); 
        delayMicroseconds(SPEED_MULT/motor_n_p);
        digitalWrite(step_pin,LOW);
        curent_position -=1;
      }
      
      else
      {
        curent_position = x;
        position_ok = true;
        motor_n_p -= 1;
      }
      
    }
  }

  void setup_pin(){
    pinMode(step_pin, OUTPUT);
    pinMode(dir_pin, OUTPUT);
    pinMode(switch_pin, INPUT); 
  }
  
  bool is_cal() {
    return is_calibrate;
  }
  
  bool is_ok() {
    return position_ok;
  }
  
  void set_pos_ok() {
    position_ok = false;
  }
};


Motor J1(J1_dir_pin, J1_step_pin, switch1_pin, J1_start_dir, speed_mult/10);
Motor J2(J2_dir_pin, J2_step_pin, switch2_pin, J2_start_dir, speed_mult);
Motor J3(J3_dir_pin, J3_step_pin, switch3_pin, J3_start_dir, speed_mult);
Motor J4(J4_dir_pin, J4_step_pin, switch4_pin, J4_start_dir, speed_mult);
Motor J5(J5_dir_pin, J5_step_pin, switch5_pin, J5_start_dir, speed_mult);
Motor J6(J6_dir_pin, J6_step_pin, switch6_pin, J6_start_dir, speed_mult/5);

bool position1 = false;
bool position2 = false;
bool position3 = false;

void reset()
{
  J1.set_pos_ok();
  J2.set_pos_ok();
  J3.set_pos_ok();
  J4.set_pos_ok();
  J5.set_pos_ok();
  J6.set_pos_ok();
  motor_n_p = 6;
  position1 = false;
  position2 = false;
  position3 = false;
  delay(1000);
}

Servo myservo;

void setup() {

  J1.setup_pin();
  J2.setup_pin();
  J3.setup_pin();
  J4.setup_pin();
  J5.setup_pin();
  J6.setup_pin();

//  pinMode(relay_pin_1, OUTPUT);
//  pinMode(relay_pin_2, OUTPUT);
//  pinMode(relay_pin_3, OUTPUT);
//  pinMode(relay_pin_4, OUTPUT);
//  pinMode(relay_pin_5, OUTPUT);
//  pinMode(relay_pin_6, OUTPUT);
//  delay(100);
//  digitalWrite(relay_pin_1, LOW);
//  digitalWrite(relay_pin_2, LOW);
//  digitalWrite(relay_pin_3, LOW);
//  digitalWrite(relay_pin_4, LOW);
//  digitalWrite(relay_pin_5, LOW);
//  digitalWrite(relay_pin_6, LOW);

  Serial.begin(9600);
  myservo.attach(34);


  delay(1000);
}




void loop() {

  // if(!J5.is_cal()) {
  //   J5.calibrate();
  // }
  // else
  // {
  //   Serial.println(count);
  // }

  
  
  while(!motor_calibrate) {
    J1.calibrate();
    J2.calibrate();
    J3.calibrate();
    J4.calibrate();
    J5.calibrate();
    J6.calibrate();
    
    motor_calibrate = J1.is_cal() && J2.is_cal() && J3.is_cal() && J4.is_cal() && J5.is_cal() && J6.is_cal();
  }
  delay(1000);

     while (!position1) {
       J1.set_position(J1_range/2);
       J2.set_position(J2_range/4);
       J3.set_position(J3_range/5);
       J4.set_position(J4_range/3);
       J5.set_position(J5_range/2); 
       J6.set_position(J6_range/3);
       position1 = J1.is_ok() && J2.is_ok() && J3.is_ok() && J4.is_ok() && J5.is_ok() && J6.is_ok();
     }
     delay(1000);
     myservo.write(110);
     delay(1000);
     reset();

     
    
     while (!position2) {
       J1.set_position(J1_range/3);
       J2.set_position(J2_range/5);
       J3.set_position(J3_range/3);
       J4.set_position(J4_range/2);
       J5.set_position(J5_range/100); 
       J6.set_position(J6_range/6);
       position2 = J1.is_ok() && J2.is_ok() && J3.is_ok() && J4.is_ok() && J5.is_ok() && J6.is_ok();
     }
     reset();

     delay(1000);
     myservo.write(70);
     delay(1000);

  //   while (!position3) {
  //     J1.set_position(21274/3*2);
  //     J2.set_position(29014/3);
  //     J3.set_position(26326/8);
  //     J4.set_position(0);
  //     J5.set_position(10);
  //     J6.set_position(44003/6);
    
  //     position3 = J1.is_ok() && J2.is_ok() && J3.is_ok() && J4.is_ok() && J5.is_ok() && J6.is_ok();
  //   }
  //   reset();


  // }
}


 


// black green  red  blue
