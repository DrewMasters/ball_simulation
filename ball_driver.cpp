#include <iostream>
#include "ball.h"

using namespace std;

int main(){
	//test program from ball, vec, and ball_simulation classes
	ball_simulation test(10,10);
	test.add_ball(2, 1, 2, 0, 0, 0);
	test.update();
	test.add_ball(2,1,0,0,1,0);
	for (int i=0; i<10; i++){
		test.update();
	}
}
