#include <Stepper.h> 

const int stepPin_b = 2; //X.STEP
const int dirPin_b = 5; // X.DIR
const int stepPin_c = 3; //X.STEP
const int dirPin_c = 6; // X.DIR
const int stepPin_z = 4; //X.STEP
const int dirPin_z = 7; // X.DIR
const int stepPin_a = 12; //X.STEP
const int dirPin_a = 13; // X.DIR
//const byte enablePin = 8;
const int SW = 9;
int vrx = A1; 
int vry = A0; 
int vrx_data = 0; 
int vry_data = 0;
static int pos_cur_x = 0; 
static int pos_cur_y = 0;
int time_delay = 1000; //2000 > Very Slow, 1000 > Slow, 500 > Fast
long move_delay = 1000;

 
void setup() {
 // Sets the two pins as Outputs
   pinMode(stepPin_b,OUTPUT); 
   pinMode(dirPin_b,OUTPUT);
   pinMode(stepPin_c,OUTPUT); 
   pinMode(dirPin_c,OUTPUT);
   pinMode(stepPin_z,OUTPUT); 
   pinMode(dirPin_z,OUTPUT);
   pinMode(stepPin_a,OUTPUT); 
   pinMode(dirPin_a,OUTPUT);
   //digitalWrite(enablePin, LOW);
   pinMode(vrx , INPUT); 
   pinMode(vry, INPUT);  
   Serial.begin(9600);
   //draw_square(20);
   //draw_octagon(20);
   draw_star(200, 5);
   //draw_polygon(100, 8);
}
 
void loop() {
   joystick();
}

void move_z(int steps){
  if(steps < 0){
    digitalWrite(dirPin_z,LOW);
    digitalWrite(dirPin_a,LOW);
  }
  if(steps > 0){
    digitalWrite(dirPin_z,HIGH);
    digitalWrite(dirPin_a,HIGH);
  }
  
  for(int i = 0; i < abs(steps); i++){
    digitalWrite(stepPin_z,HIGH); 
    digitalWrite(stepPin_a,HIGH); 
    
    delayMicroseconds(time_delay); 
    
    digitalWrite(stepPin_z,LOW);
    digitalWrite(stepPin_a,LOW); 
    
    delayMicroseconds(time_delay); 
  }
  
  digitalWrite(dirPin_z,LOW);
  digitalWrite(dirPin_a,LOW);
}

void move_xy(int pos_des_x, int pos_des_y){
  int diff_x = pos_des_x; //- pos_cur_x;
  int diff_y = pos_des_y; //- pos_cur_y;
  int steps_b = diff_y - diff_x;
  int steps_c = diff_y + diff_x;
  int step_b_dir = 0;
  int step_c_dir = 0;
  float b_move = 0.0;
  float c_move = 0.0;
  
  if(steps_b < 0){
    digitalWrite(dirPin_b,LOW);
    step_b_dir = 1;
  }
  if(steps_b > 0){
    digitalWrite(dirPin_b,HIGH);
    step_b_dir = -1;
  }
  if(steps_c < 0){
    digitalWrite(dirPin_c,LOW);
    step_c_dir = -1;
  }
  if(steps_c > 0){
    digitalWrite(dirPin_c,HIGH);
    step_c_dir = 1;
  }
  
  for(int i = 0; i < max(abs(steps_b), abs(steps_c)); i++){
    if(abs(steps_b) > 0){
      b_move += abs(steps_b)/max(abs(steps_b), abs(steps_c));
    }
    if(abs(steps_c) > 0){
      c_move += abs(steps_c)/max(abs(steps_b), abs(steps_c));
    }
    if(b_move >= 1){
      digitalWrite(stepPin_b,HIGH); 
      pos_cur_x += step_b_dir;
      b_move -= 1;
    }
    if(c_move >= 1){
      digitalWrite(stepPin_c,HIGH); 
      pos_cur_y += step_c_dir;
      c_move -= 1;
    }
    delayMicroseconds(time_delay); 
  
    if(abs(steps_b) > 0){
      digitalWrite(stepPin_b,LOW);
    }
    if(abs(steps_c) > 0){
      digitalWrite(stepPin_c,LOW);
    }
    delayMicroseconds(time_delay); 
  }
  
  digitalWrite(dirPin_b,LOW);
  digitalWrite(dirPin_c,LOW);
}

void joystick(){
  vrx_data = analogRead(vrx);
  vry_data = analogRead(vry);
  int pos_des_x = 0;
  int pos_des_y = 0;
  if (vrx_data > 900){
    pos_des_x += 1;
  }
  if (vrx_data < 100){
    pos_des_x -= 1;
  }
  if (vry_data > 900){
    pos_des_y += 1;
  }
  if (vry_data < 100){
    pos_des_y -= 1;
  }
  move_xy(pos_des_x, pos_des_y);
  //delay(time_delay/100);
}

void draw_polygon(float scale, int sides){
  int angle = 0;
  move_xy(0, -cos((360-(360/sides))*(PI/180))*scale);
  delay(move_delay);
  for(int i = 0; i < sides; i++){
    move_xy(scale*sin(angle*(PI/180)), -scale*cos(angle*(PI/180)));
    angle += (360/sides);
    delay(move_delay);
  }
  move_xy(0, cos((360-(360/sides))*(PI/180))*scale);
}

void draw_star(float scale, int points){
  int angle = 0; //270+(90/points);
  move_xy(0, -scale);
  delay(move_delay);
  for(int i = 0; i < points; i++){
    Serial.print("angle: ");
    Serial.print(angle);
    Serial.print(", x: ");
    Serial.print(-scale*cos(angle*(PI/180)));
    Serial.print(", y: ");
    Serial.println(scale*sin(angle*(PI/180)));
    move_xy(scale*sin(angle*(PI/180)), -scale*cos(angle*(PI/180)));
    angle += 2*(360/points);
    //angle -= ((int)(angle/360))*360;
    delay(move_delay);
  }
  move_xy(0, scale);
}
