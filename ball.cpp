#include <iostream>
#include <vector>

using namespace std;

ball::ball(int ball_id, double m, double r, double vx, double vy, double x_pos, double y_pos){
	id = id;
	mass = m;
	radius = r;
	v_x = vx;
	v_y = vy;
	x = x_pos;
	y = y_pos;
}

double ball::get_mass(){
	return mass;
}

double ball::get_vx(){
	return v_x;
}

double ball::get_vy(){
	return v_y;
}

double ball::get_r(){
	return radius;
}

double ball:get_x(){
	return x;
}

double ball::get_y(){
	return y;
}

void ball::update_vx(double vx){
	v_x = vx;
}

void ball::update_vy(double vy){
	v_y = vy;
}

void ball::update_x(double new_x){
	x = new_x;
}

void ball::update_y(double new_y){
	y = new_y;
}

void print(){
	cout << "Ball " << id << endl;
	cout << "	mass: " << mass << endl;
	cout << "	radius: " << radius << endl;
	cout << "	x velocity: " << v_x << endl;
	cout << "	y velocity: " << v_y << endl;
	cout << "	x position: " << x << endl;
	cout << "	y position: " << y << endl;
}

ball_simulation::ball_simulation(int x_limit, int y_limit){
	x_wall = x_limit;
	y_wall = y_limit;
	ball_count = 0;
}

void ball_simulation::add_ball(double m, double r, double vx, double vy, double x, double y){
	balls.push_back(ball(ball_count, m, r, vx, vy, x, y));
	ball_count++;
}

void ball_simulation::update(){
	int i, j;
	double r, dist;

	for (i=0; i < balls.size(); i++){
		for (j=0; j < balls.size(); j++){
			if (i!=j){
				r = balls[i].get_r()+balls[j].get_r();
				r = r*r;
				dist = distance(balls[i].get_x(),balls[j].get_x(), balls[i].get_y(),balls[j].get_y());
				dist = dist*dist;
				if (dist <= r){
					//collision has occurred
					resolve_collision(balls[i],balls[j]);
				}
			}
		}
	}

	for (i=0; i < balls.size(); i++){
		
	}
}

double ball_simulation::distance(double x1, double x2, double y1, double y2){

}

double ball_simulation::resolve_collision(ball B1, ball B2){

}
