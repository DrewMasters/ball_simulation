#include <vector>

class vec{
	public:
		vec(double x, double y);
		double magnitude();
		double get_x_comp();
		double get_y_comp();
	private:
		double x_comp;
		double y_comp;
};

double dot_product(vec V1,vec V2);
vec add_vec(vec V1, vec V2);
vec scale_vec(vec v, double scale);
vec tangent_vec(vec v);

class ball{
	public:
		ball(int ball_id, double m, double r, double vx, double vy, double x, double y);
		double get_mass();
		double get_vx();
		double get_vy();
		double get_r();
		double get_x();
		double get_y();
		void update_vx(double vx);
		void update_vy(double vy);
		void update_x(double new_x);
		void update_y(double new_y);
		void print();
	private:
		int id;
		double mass;
		double v_x;
		double v_y;
		double radius;
		double x;
		double y;
};

vec normal_vec(ball B1, ball B2);

class ball_simulation{
	public:
		ball_simulation(int x_limit, int y_limit);
		void add_ball(double m, double r, double vx, double vy, double x, double y);
		void update();
		double distance(double x1, double x2, double y1, double y2);
		void resolve_collision(ball &B1, ball &B2);
		void move_ball(ball &B1);
	private:
		std::vector <ball> balls;
		int x_wall;
		int y_wall;
		int ball_count;
};
