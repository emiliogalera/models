#ifndef NEW_MODEL_H
#define NEW_MODEL_H
#include<random>
#include<vector>


class RateNet{
	private:
		unsigned int N; //number of elements in the system
		unsigned int P; // number of patterns stored
		double f; //average active neurons in a pattern mu, to be an array.
		double gamma; //gain
		double otau;
		double oeps; 

		std::mt19937 gen;
		std::uniform_real_distribution<double> dis;

		std::vector<double> state_vector;
		std::vector<double> hvec;
		std::vector<double> strvec; // pattern strength vector


		std::vector<std::vector<double>> w_matrix;
		std::vector<std::vector<double>> w_fast;
		std::vector<std::vector<int>> p_matrix;

	public:
		// constructor
		RateNet(int net_size, double gma, double fval);

		// random tools
		void add_random_pattern(double probability, double str);
		void add_random_10(unsigned int active_number, double str);
		void add_random_fpattern(double str);
		void random_state_gen(double a, double b);
		void random_setter(double a, double b);
		double sample();

		//Make hebbian matrix
		void make_hebb();
		void start_fast_matrix();

		//Dynamic methods
		double hcalc_svar(std::vector<double>::size_type i);
		double hcalc_xvar(std::vector<double>::size_type i);
		void net_hcalc(char mode);
		double state_update(std::vector<double>::size_type i, double input = 0);
		void net_state_update();
		void net_state_update(std::vector<double>& input_vec);
		void net_state_update(std::vector<int>& input_vec);
		void set_fast_hebb_par(double tau, double eps);
		void w_fast_update();
		double w(std::vector<double>::size_type i, std::vector<double>::size_type j);

		// Probing methods
		double projection(std::vector<double>::size_type u);
		unsigned int get_P();
		std::vector<int>& get_pattern(std::vector<int>::size_type u);
		std::vector<std::vector<double>>::size_type get_sizeof_w_matrix();
		std::vector<std::vector<double>>::size_type get_sizeof_w_matrix_fast();
		std::vector<std::vector<int>>::size_type get_sizeof_p_matrix();


};

#endif
