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
}

class ball_simulation{
	public:
		ball_simulation(int x_limit, int y_limit);
		void add_ball(double m, double r, double vx, double vy, double x, double y);
		void update();
	private:
		vector <ball> balls;
		int x_wall;
		int y_wall;
		int ball_count;
}
