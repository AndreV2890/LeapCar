#include "Leap.h"
#include "LeapMath.h"
#include "../serial/DiViClientSeriale.h"

#define X_MM_MIN 20
#define Z_MM_MIN 20
#define X_MM_MAX 120
#define Z_MM_MAX 90

#define MAX_ABS_VEL 70
#define MAX_ABS_ANG 10

enum State{NO_HAND, FREE_RUNNING, ROTATION};
enum Signal{hand_present, hand_absent, gesture};

using namespace Leap;

class MyCarMotion{
	private:
		//Coordinates x and z read from LeapMotion
		int x;
		int z;
		
		//values of velocity and rotation angle
		//new_* -> current value
		//old_* -> last value
		int new_vel;
		int new_angle;
		int old_vel;
		int old_angle;
		
		//number of equal frame 
		//(to filter before sending to DiViClientSeriale)
		int count_frame;
		
		//boolean to check is the right hand is founded
		bool right_hand;
		
		//Boolean to check is linear or not.
		//In order to have a linear motion the z coordinate of the hand 
		//must be z < |Z_MM_MIN|
		bool linear_motion;
		
		//direction of y component of palm normal vector (useful for the
		//check_signal function)
		//0 -> negative direction
		//1 -> positive direction
		int palm_normal;
		int old_palm_normal;
		
		//State Machine (implemented by nested switch)
		State myState;
		
		//Pointer to DiViClientSeriale object
		DiViClientSeriale* client;
		
		//Controller of the Leap Motion
		Controller c;

		//Private functions of MyCarMotion 
		//(see MyCarMotion.cpp for further informations)	
		void read_frame();
		int filter_before_send();
		int rotation_angle();
		bool hand_founded();
		bool isLinear();
		void send();
		Signal check_signal();

	public:
		MyCarMotion(DiViClientSeriale*);
		~MyCarMotion();

		void init();
		void dispatch();
		void tran(State);

		int velocity();

};
