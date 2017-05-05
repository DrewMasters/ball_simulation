#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include "ball.h"

using namespace std;

ball::ball(int ball_id, double m, double r, double vx, double vy, double x_pos, double y_pos){
	id = ball_id;
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

double ball::get_x(){
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

void ball::print(){
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
	//function adds a ball to the simulation
	balls.push_back(ball(ball_count, m, r, vx, vy, x, y));
	balls[ball_count].print();
	ball_count++;
}

void ball_simulation::update(){
	int i, j;
	double r, dist;

	/*
	 * First check for collisions and resolve them
	 */
	for (i=0; i < balls.size(); i++){
		for (j=i+1; j < balls.size(); j++){
			r = balls[i].get_r()+balls[j].get_r();
			r = r*r;
			dist = distance(balls[i].get_x(),balls[j].get_x(), balls[i].get_y(),balls[j].get_y());
			dist = dist*dist;

			/* 
			 * Compare squared radius to squared distance
			 * if the distance between the balls center
			 * is less than the radius there has been
			 * a collision
			 */
			if (dist <= r){
				//collision has occurred
				resolve_collision(balls[i],balls[j]);
			}
		}
	}

	/*
	 * Update all balls so they are in correct position after
	 * one time frame
	 */
	for (i=0; i < balls.size(); i++){
		move_ball(balls[i]);
		balls[i].print();
	}
}

double ball_simulation::distance(double x1, double x2, double y1, double y2){
	double x_diff, y_diff;
	x_diff = x2 - x1;
	y_diff = y2 - y1;
	return sqrt((x_diff * x_diff) + (y_diff * y_diff));
}

void ball_simulation::resolve_collision(ball &B1, ball &B2){
	//produce the two velocity vectors
	vec V1(B1.get_vx(),B1.get_vy()), V2(B2.get_vx(),B2.get_vy());
	//produce the normal vector
	vec N = normal_vec(B1,B2);
	//produce the unit normal vector
	vec UN = scale_vec(N, N.magnitude());
	//produce the tangent vector
	vec T = tanget_vec(N);
	vec UT = scale_vec(T, T.magnitude());

	double v1n, v2n, v1t, v2t,v1np,v2np;

	/*
	 * get the normal velocity and tangent
	 * velocity for both vectors by taking 
	 * the dot product of the unit normal vector
	 * and the velocity vector and the dot product
	 * of the unit tangent vector and the velocity
	 * vector
	 */
	v1n = dot_product(UN,V1);
	v1t = dot_product(UT,V1);
	v2n = dot_product(UN,V2);
	v2t = dot_product(UT,V2);

	/*
	 * tangent velocity remains the same 
	 * normal velocity can be updated with
	 * the following equations
	 */
	v1np = (v1n*(B1.get_mass()-B2.get_mass())+2*B2.get_mass()*v2n)/(B1.get_mass()+B2.get_mass());
	v2np = (v2n*(B2.get_mass()-B1.get_mass())+2*B1.get_mass()*v1n)/(B1.get_mass()+B2.get_mass());

	/*
	 * Generate tangent and normal velocity vectors
	 * by scaling the unit tangent and unit normal
	 * vector by the normal velocity and tangent velocity
	 */
	vec V1n = scale_vec(UN,v1np);
	vec V2n = scale_vec(UN,v2np);
	vec V1t = scale_vec(UT,v1t);
	vec V2t = scale_vec(UT,v2t);

	/*
	 * Add the normal and tangent velocity vectors
	 * to get the overall velocity vector
	 */
	V1 = add_vec(V1n,V1t);
	V2 = add_vec(V2n,V2t);

	/*
	 * update the x and y velocities of the balls
	 */
	B1.update_vx(V1.get_x_comp());
	B1.update_vy(V1.get_y_comp());

	B2.update_vx(V2.get_x_comp());
	B2.update_vy(V2.get_y_comp());
}

void ball_simulation::move_ball(ball &B1){
	double tmp_x, tmp_y, tmp_vx, tmp_vy;

	tmp_x = B1.get_x() + B1.get_vx();
	tmp_y = B1.get_y() + B1.get_vy();
	tmp_vx = B1.get_vx();
	tmp_vy = B1.get_vy();

	/*
	 * If ball will hit the side wall in 
	 * the course of this timeframe
	 */
	if (fabs(tmp_x) >= x_wall){
		if (tmp_x>0){
			tmp_x = 2*x_wall - tmp_vx - B1.get_x();
		}
		else{
			tmp_x = -2*x_wall - tmp_vx - B1.get_x();
		}
		tmp_vx = -1*tmp_vx;
	}

	B1.update_vx(tmp_vx);
	B1.update_x(tmp_x);

	/*
	 * If the ball will hit the top or bottom
	 * wall in the course of this timeframe
	 */
	if (fabs(tmp_y) >= y_wall){
		if (tmp_y>0){
			tmp_y = 2*y_wall - tmp_vy - B1.get_y();
		}
		else{
			tmp_y = -2*y_wall - tmp_vy - B1.get_y();
		}
		tmp_vy = -1*tmp_vy;
	}

	B1.update_vy(tmp_vy);
	B1.update_y(tmp_y);
}

double dot_product(vec V1, vec V2){
	return (V1.get_x_comp()*V2.get_x_comp()) + (V1.get_y_comp() * V2.get_y_comp());
}

vec add_vec(vec V1, vec V2){
	vec Vt(V1.get_x_comp()+V2.get_x_comp(),V1.get_y_comp()+V2.get_y_comp());

	return Vt;
}

vec normal_vec(ball B1, ball B2){
	double x1, y1;
	double x2, y2;

	x1 = B1.get_x();
	x2 = B2.get_x();
	y1 = B1.get_y();
	y2 = B2.get_y();

	/*
	 * Normal vector 
	 * x = x2-x1
	 * y = y2-y1
	 */
	x1 = x2-x1;
	y1 = y2-y1;

	vec t(x1,y1);
	return t;
}

vec::vec(double x, double y){
	x_comp = x;
	y_comp = y;
}

vec scale_vec(vec v, double scale){
	if (scale == 0) scale = 1;
	/*
	 * Scale vector by a scalar
	 */
	vec t(v.get_x_comp() / scale, v.get_y_comp() / scale);
	return t;
}

vec tangent_vec(vec v){
	vec t((-1)*v.get_y_comp(), v.get_x_comp());
	return t;
}

double vec::magnitude(){
	return sqrt((x_comp*x_comp)+(y_comp*y_comp));
}

double vec::get_x_comp(){
	return x_comp;
}

double vec::get_y_comp(){
	return y_comp;
}
