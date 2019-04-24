#include <kipr/botball.h>

//PORTS
#define ARM 0
#define CLAW 1

//Sensor PORTS
#define REFLECTANCE 0

//Basic ARM movements
#define ARM_DOWN 980
#define ARM_UP 100

//Basic CLAW movements
#define CLAW_OPEN 100
#define CLAW_CLOSE_POMS 1230
#define CLAW_CLOSE_CUBE 844 //1900
#define CLAW_CLOSE_AMBULANCE 1900 //Change this 

//More specific ARM movements

//More specific CLAW movements
#define CLAW_OPEN_TOP 600 

//Thresholds
#define THRESHOLD 1350


//////////////////////////////////////////////////Basic Functions//////////////////////////////////////////////////////
void setup(){
    set_servo_position(ARM, ARM_UP);
    set_servo_position(CLAW, CLAW_CLOSE_POMS);
    enable_servo(CLAW);
    enable_servo(ARM);
}

void drive(int speed, int time) {
  create_drive_direct(-speed, -speed);
  msleep(time);
  create_drive_direct(0, 0);
}

void turn_right(int speed, int time) {
  create_drive_direct(speed,-speed);						
  msleep(time);
  create_drive_direct(0,0);
}

void turn_left(int speed, int time) {
  create_drive_direct(-speed,speed);						
  msleep(time);
  create_drive_direct(0,0);
}

void stop() {
  create_drive_direct(0, 0);
}

void interpolate(int srv, int pos, int step_size)
{
  int cur_pos = get_servo_position(srv);
  int delta_remain = abs(pos - cur_pos);  
  int direction = pos > cur_pos ? 1 : -1;

  while(delta_remain >= 5)
  {
    //Make the final step fit so that the servo goes to its desired position
    if(delta_remain < step_size)
      step_size = delta_remain;

    set_servo_position(srv, cur_pos + direction * step_size);
    msleep(50);

    //Update the necessary variable
    cur_pos = get_servo_position(srv);
    delta_remain = abs(pos - cur_pos);
  }
}

void grab_poms(int timef, int timeb){
    set_servo_position(CLAW, CLAW_OPEN);
    msleep(200);
    interpolate(ARM, ARM_DOWN, 20);
    msleep(500);
    drive(150, timef);
    interpolate(CLAW, CLAW_CLOSE_POMS, 50);
    msleep(600);
    drive(-150, timeb);
    interpolate(ARM, ARM_UP, 20);
    msleep(700); 
}

void grab_cube(){
    set_servo_position(CLAW, CLAW_OPEN);
    msleep(200);
    interpolate(ARM, ARM_DOWN, 20);
    msleep(500);
    set_servo_position(CLAW, CLAW_CLOSE_CUBE);
    msleep(500);
    interpolate(ARM, ARM_DOWN - 220, 20);
    msleep(400); 
}

////////////////////////////////////////////////Sensor Functions////////////////////////////////////////////////////////
void back_until_bump(){
    while(!get_create_lbump()&&!get_create_rbump()){
        create_drive_direct(250,250);
    }
    stop();
}

int getSign(int n, int tolerance){
    if(n > tolerance){
        return 1;
    }
    else if(n < -tolerance){
        return -1;
    }
    return 0;
}

void driveToLine(int power, int adjustPower){
    int leftSign = getSign(THRESHOLD - get_create_lcliff_amt(),0);
    int rightSign = getSign(THRESHOLD - get_create_rcliff_amt(),0);
    int rpower = rightSign * -power;
    int lpower = leftSign * -power;
    while(!(leftSign == 1 && rightSign == 1)){
        create_drive_direct(rpower, lpower);
        msleep(1);
        leftSign = getSign(THRESHOLD - get_create_lcliff_amt(),0);
        rightSign = getSign(THRESHOLD - get_create_rcliff_amt(),0);
        if(leftSign == 1){
            rpower = rightSign * -adjustPower;
        }
        if(rightSign == 1){
            lpower = leftSign * -adjustPower;
        }
    }
    stop();
}

inline int over_black()
{
  //printf("REFLECTANCE: %d %d\n", analog(REFLECTANCE), analog(REFLECTANCE) > THRESHOLD);
  return analog(REFLECTANCE) > 1000;
}

void line_follow(int time) {
  int t = 0, interval = 10;
  while(t * interval < time){
    if(!over_black()) {
      create_drive_direct(-150, -90);
    }
    else {
      create_drive_direct(-90, -150);
    }
    msleep(interval);
    t++;
  }
  stop();
}

/////////////////////////////////////////////Code Path//////////////////////////////////////////////////////////////////
void grab_cube_sequence(){ 
    turn_left(250, 170);
    msleep(250);
    grab_cube();
    turn_left(250, 400);
}

void middle(){
    //interpolate(ARM, ARM_DOWN - 150, 30);
    driveToLine(-120, -15);
  	create_drive_direct(-400, -100);
   	msleep(1500); //1300 for camera view
    stop();
    msleep(600);
    drive(-200, 870);
    turn_left(120, 600);
    turn_right(120, 600);
}

void cube_dump(){
    line_follow(3100);
    //drive(100, 200);
    turn_left(100, 300); //TURN LEFT 
    msleep(400);
    drive(100,1300); 
    set_servo_position(CLAW, CLAW_OPEN_TOP);
    msleep(300);
    turn_right(50,100); //IF DOESNT WORK CHANGE THIS  
    msleep(20); 
    interpolate(ARM, ARM_UP, 30);
    msleep(100);
    drive(-100,660); 
}

void poms_dump(){
    turn_left(120, 175);
    drive(200, 550);
    turn_left(160, 1150); //TURN THAT WAS JUST CHANGED 
    drive(100, 200);
    set_servo_position(ARM, ARM_UP + 120);
    msleep(200);
    interpolate(CLAW, CLAW_OPEN_TOP, 20);
    msleep(100);
    set_servo_position(ARM, ARM_UP);
}

void grab_ambulance(){
    //TURN 180 to face away from the medical center 
    //msleep for the previous line 
    drive(200, 1500); 
    msleep(300); 
    turn_right(400, 1000); //I'm just assuming all turns are 1000 ms time. 
    msleep(200); 
    set_servo_position(ARM, ARM_DOWN); 
    msleep(150); 
    set_servo_position(CLAW, CLAW_CLOSE_AMBULANCE); 
    msleep(100); 
    drive(-100, 2000); //back up 
    msleep(100); 
    turn_right(500, 1000); 
    msleep(100); 
    drive(100, 2500);
    msleep(100); 
    set_servo_position(CLAW, CLAW_OPEN); 
    msleep(100); 
    drive(200, 500); 
    msleep(100); //little push in to make sure the ambulance is in the zone 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
    setup();
    create_connect();
  	create_full();
    msleep(1000);
    
     
    grab_cube_sequence();
    
    //start sweeping
    drive(200,900); 
    msleep(100); //weeeeeee 
    turn_left(120,2000);
    msleep(100); 
    turn_right(120,1900);
    msleep(100); 
    back_until_bump(); 
    msleep(100); 
    turn_left(120, 100);
    middle();
    cube_dump();
    
    drive(-100, 200);
    turn_right(150, 1500); //CHANGED THIS FOR THE TURN TO GRAB THE BLUE POMS 
    drive(-150, 600); //drive back before poms
    
    grab_poms(450, 400);
    poms_dump();
    
    set_servo_position(CLAW, CLAW_OPEN);
    msleep(200);
    turn_left(120, 1010);
    interpolate(ARM, ARM_DOWN - 140, 30);
    drive(200, 1200);
    
    create_drive_direct(-90, 0);
    msleep(350);
    stop();
    
    msleep(200);
    interpolate(CLAW, CLAW_CLOSE_CUBE, 30);
    msleep(400);
    
    create_drive_direct(90, 0);
    msleep(350);
    stop();
    
    drive(-200, 1800);
  	msleep(100);
    turn_right(120, 920);
    drive(200, 200);
    msleep(1000);
    set_servo_position(CLAW, CLAW_OPEN_TOP); 
    msleep(500);
    drive(-200, 800);
    interpolate(ARM, ARM_UP, 30); 
    msleep(100); 
    turn_left(120, 1600);
    drive(200, 200);
    turn_left(120, 1300);
    interpolate(ARM, ARM_DOWN-400, 30); 
    msleep(100); 
    line_follow(9000); //line following
    msleep(100); 
    turn_left(120, 1200); 
    msleep(100); 
    //drive(50,100); //driving off the line
    //msleep(100); 
    turn_left(120,1200); //making the turn less violent 
    msleep(100); 
    drive(-100, 250); 
    msleep(100); 
    turn_left(120,1200); 
    msleep(100); 
   	set_servo_position(CLAW, CLAW_OPEN); 
    drive(100, 8500); //push 
    msleep(100); 
    turn_left(200, 600); //turn  a little 
    msleep(100); 
 	drive(100, 1000); //push into the medical box 
    msleep(100); 
    
    return 0;
}

//120, 1600 = perfect turn