#include <kipr/botball.h>

//PORTS
#define ARM 0
#define CLAW 1

//Basic ARM movements
#define ARM_DOWN 980
#define ARM_UP 100

//Basic CLAW movements
#define CLAW_OPEN 1320
#define CLAW_CLOSE_POMS 2047
#define CLAW_CLOSE_CUBE 1900
#define CLAW_CLOSE_AMBULANCE 1900 //Change this 

//More specific ARM movements

//More specific CLAW movements
#define CLAW_OPEN_TOP 1650

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

void grab_poms(){
    set_servo_position(ARM, ARM_DOWN);
    msleep(3000);
    set_servo_position(CLAW, CLAW_OPEN);
    msleep(3000);
    drive(200, 700);
    set_servo_position(CLAW, CLAW_CLOSE_POMS);
    msleep(2000);
    set_servo_position(ARM, ARM_UP);
    turn_right(500,1000); //Estimates 
    msleep(1000); 
    drive(200, 5000);
    msleep(1000); 
    //ADD LINE HERE THAT LOWERS ARM 
    //MSLEEP OF PREVIOUS LINE 
    set_servo_position(CLAW, CLAW_OPEN); 
    msleep(3000); 
}

void grab_cube(){
    //TURN 180 to move away from medical center 
    //sleep for previous line 
    set_servo_position(ARM, ARM_DOWN);
    msleep(3000);
    set_servo_position(CLAW, CLAW_OPEN);
    msleep(3000);
    drive(200, 2000);
    turn_right(500,1000); 
    msleep(100); 
    drive(100, 500); 
    msleep(100); 
    set_servo_position(CLAW, CLAW_CLOSE_CUBE);
    msleep(2000);
    set_servo_position(ARM, ARM_UP);
    msleep(400); 
    drive(-200, 2000); //back up more and then turning  
    msleep(500); 
    turn_right(200, 1000); 
    msleep(500); 
    drive(400, 3500); 
    msleep(500); 
    set_servo_position(ARM, ARM_DOWN); 
    msleep(100); 
    set_servo_position(CLAW, CLAW_OPEN); 
    msleep(100); //Drops the cube into the burning zone 
    drive(100, 1000); //Push in a little bit to make sure the cube is in the zone... CHANGE LATER, SHOULD BE ON TOP 
    msleep(100);
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

int main() {
    create_connect();
  	create_full();
    enable_servo(CLAW);
    enable_servo(ARM);
    stop();
    return 0;
}