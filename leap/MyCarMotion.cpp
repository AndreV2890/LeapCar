#include "MyCarMotion.h"
#include "function_leapmotion.h"

#include <pthread.h>
#include <cmath>

using namespace Leap;

//Constructor of class MyCarMotion
MyCarMotion::MyCarMotion(DiViClientSeriale* c){
	x= 0;
	z = 0;

	linear_motion = true;
	right_hand = false;

	new_vel = 0;
	new_angle = 0;
	old_vel = 0;
	old_angle = 0;

	palm_normal = 0;
	old_palm_normal = 0;

	count_frame = 0;

	client = c;

	init();
}

//To read hand information
void MyCarMotion::read_frame(){
	Frame f = c.frame();
	HandList hands;
	hands = f.hands();

	for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl){
		Hand hand = *hl;
		if(hand.isRight()){
			right_hand = true;
			x = (int)hand.palmPosition().x;
			z = (int)hand.palmPosition().z;
			old_palm_normal = palm_normal;
			if(hand.palmNormal().y > 0)
				palm_normal = 1;
			else
				palm_normal = 0;
		}
	}
	if(hands.isEmpty()) right_hand = false;
}

//To reduce the number of generated packets
int MyCarMotion::filter_before_send(){
	if(new_vel == old_vel && new_angle == old_angle)
		count_frame++;
	else{
		old_angle = new_angle;
		old_vel = new_vel;
		count_frame = 0;
	}
	return count_frame;
}

//Return car velocity
int MyCarMotion::velocity(){
	return -new_vel;
}

//Return car rotation angle
int MyCarMotion::rotation_angle(){
	return -new_angle;
}

//Return if car is moving with a linear motion
//or with a curviliner motion
bool MyCarMotion::isLinear(){
	return linear_motion;
}

//Return true if the right hand is detected
bool MyCarMotion::hand_founded(){
	return right_hand;
}

//To send information about velocity, angle and motion
//to the DiViClientSeriale in order to create the packets
void MyCarMotion::send(){
	client->send_command(velocity(), rotation_angle(), isLinear());
	if(!hand_founded()) std::cout << "Mano non trovata\t ";
	std::cout << "Velocity: " << velocity();
	std::cout << "\tLinear_motion: " << isLinear();
	if(!isLinear()){
		std::cout << "\tRotation Angle: " << rotation_angle();
		std::cout << "\tX: " << x;
	}
	std::cout << std::endl;

}

//Init function of the state machine
void MyCarMotion::init(){
	myState = NO_HAND;
}

//Tran function of the state machine
void MyCarMotion::tran(State s){
	myState = s;
}

//To check the signal for the state machine
Signal MyCarMotion::check_signal(){
	if(!right_hand) return hand_absent;
	if(old_palm_normal == 1 && palm_normal == 0) return gesture;
	return hand_present;
}

//Dispatch function of the state machine
void MyCarMotion::dispatch(){
	read_frame();
	Signal s = check_signal();
	switch(myState){
		//CASE NO_HAND: nothing to do
		case NO_HAND:{
			switch(s){
				case hand_present:{
					tran(FREE_RUNNING);
					break;
				}
				default:{

				}
			}
			break;
		}
		//CASE FREE_RUNNING: the car will be able to move in all 
		//the direction
		case FREE_RUNNING:{
			switch(s){
				case hand_absent:{
					tran(NO_HAND);
					new_vel = 0;
					new_angle = 0;
					linear_motion = true;
					send();
					break;
				}
				case gesture:{
					tran(ROTATION);
					new_vel = 0;
					new_angle = 0;
					linear_motion = true;
					send();
					break;
				}
				default:{
					linear_motion = true;
					if(abs(z) <= Z_MM_MIN) new_vel = 0;
					if(abs(z) > Z_MM_MAX) new_vel = MAX_ABS_VEL;
					if(abs(z) > Z_MM_MIN && abs(z) <= Z_MM_MAX) new_vel = abs(z)-(abs(z)%5) - Z_MM_MIN;

					if(abs(x) <= X_MM_MIN){
						new_angle = 0;
					}
					if(abs(x) >= X_MM_MAX-3){
						new_angle = 1;
						linear_motion = false;
					}
					if(abs(x) > X_MM_MIN && abs(x) < X_MM_MAX){
						linear_motion = false;
						new_angle = MAX_ABS_ANG - (abs(x)-X_MM_MIN)/10;
					}

					if(z < 0) new_vel *=-1;
					if(x < 0) new_angle *=-1;
					if(filter_before_send() == N){
						send();
					}
				}
			}
			break;
		}
		//CASE ROTATION: the car will be able to rotate only 
		//around a whell
		case ROTATION:{
			switch(s){
				case hand_absent:{
					tran(NO_HAND);
					new_vel = 0;
					new_angle = 0;
					linear_motion = true;
					send();
					break;
				}
				case gesture:{
					tran(FREE_RUNNING);
					new_vel = 0;
					new_angle = 0;
					linear_motion = true;
					send();
					break;
				}
				default:{
					linear_motion = false;
					new_angle = 0;

					if(abs(x) <= X_MM_MIN) new_vel = 0;
					if(abs(x) > X_MM_MAX) new_vel = MAX_ABS_VEL;
					if(abs(x) > X_MM_MIN && abs(x) <= X_MM_MAX) new_vel = abs(x)-(abs(x)%5) - Z_MM_MIN;

					if(x < 0){
						new_vel *=-1;
					}
					if(filter_before_send() == N){
						send();
					}
				}
			}
			break;
		}
	}
}
