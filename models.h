#ifndef MODELS_H
#define MODELS_H

#include<cstddef>
#include<string>
#include<vector>
#include<random>

/*TODO: Rate models and spiking models share many variables and logic.
 * - Create a namespace to group vars and pars common to both type of models.
 * - Find a way to separete learning and synapse dynamic from classes. This
 * should help to build generic models by blocks*/


/*Everything related to rate model neurons*/
namespace rate{
    /* Group the parameters of the model:
     * N = size of the network
     * gamma = gain parameter of the state map function
     * tau = characteristic time of the anti-hebbian dynamics ***stores 1-1/t
     * epsilon = equilibrium term of the anti-hebbian dynamics ***stores e/N
     */
    struct parameters{
        std::vector<double>::size_type N;
        double gamma;
        double tau;
        double eps_ah;
        double eps_h;
    };

    /* Group the network vectors and matrix variables
     * s_vec = state vector of the model
     * h_vec = activity of the network
     * f_vec = weight of a pattern from the P_matrix
     *
     * P_matrix = matrix that stores patterns to be imprinted in the
     * connections of the network by the hebbian dynamics
     * Hebb_matrix = static part of the connection matrix, based on
     * hebbian learning.
     * antiHebb_matrix = dynamic part of the connection matrix, anti-learning
     * dynamics used to temporarily destabilise attractors created by Hebbian learning
     */
    struct network_var{
        std::vector<double> s_vec;
        std::vector<double> h_vec;
        std::vector<double> f_vec;

        std::vector<std::vector<double>> P_matrix;
        std::vector<std::vector<double>> Hebb_matrix;
        std::vector<std::vector<double>> antiHebb_matrix;
    };

    /* Group the variables responsible for the random number generators
     * eng = Egine used to generate randomness
     * dist = Distribution used to draw random numbers, for now it uses a
     * uniform distribution
     */
    struct random_device{
        std::mt19937 eng; //random number engine. Needs a seed (random device)
        std::uniform_real_distribution<double> dist; // distribution used for random process
    };


    class ItineratedMap{
        /**
         * Generates a network of rate based neurons interacting in a
         * all to all network.
         * This model is a fusion from Kinouchi et. al. 2008 and Kinouchi & Kinouchi 2002
         * Addaptations:
         * - Fast dynamics in Kinouchi 2002 is the anti-Hebbian dynamics from 2008
         * eta_f = eps/N
         * - Slow dynamics in 2002 acts on the static hebbian matrix from the 2008 paper.
         * - There is no correspondency for tau in the slow dynamics between the two papers,
         * assuming tau_s -> \infity
         * - TODO: ask osame if we should put a tau_s >> tau_f, so unlearning will not be permanent.
        */
        private:
            //parameters
            parameters net_par;

            // network variables
            network_var state_var;

            // random device
            random_device device;

            // prepares the random device
            void prepare_random_device(double a, double b);

            //Draws from the distribution
            double draw();

        public:
            // Constructor of the ItineratedMap class
            // Initiate the state vector with random values in the range (a, b)
            ItineratedMap(parameters& par, double a, double b);

            // Instanciate a class from parameters and initial state provided by the user
            ItineratedMap(parameters& par, std::vector<double>& vec);

            // Stores a random pattern, where xi in [-1, 1].
            void add_random_pattern(double prob, double strength);

            // Stores a pattern provided by the user
            void add_exterior_pattern(std::vector<double>& vec, double strength);

            // Generates the Hebbian matrix part of the connection
            void make_hebb_matrix();

            /*---- Probing methods ----*/
            /*---- These methods return references, be careful! ----*/
            // Get pattern by index mu
            std::vector<double>& get_xi(int u);

            //Probe state vector;
            std::vector<double>& get_State();

            //Probe activity
            std::vector<double>& get_activity();

            //Probe Hebb matrix
            std::vector<std::vector<double>>& get_hebb();

            //Probe antiHebb matrix
            std::vector<std::vector<double>>& get_antihebb();

            // Generate m_vector of projection of the current state on to stored
            //patterns
            void generate_m(std::vector<double>& m_recipient);

            /*---- Update methods ----*/
            //Update the activity vector of the network h(t)
            void activity_update();

            //Update the state of the network
            void state_update_tgh(); //without input
            void state_update_tgh(std::vector<double>& input); //with input

            void state_update_rational(); //without input
            void state_update_rational(std::vector<double>& input); //with input

            // IMPORTANT: both connection matrices don't considere self-connections
            // Jii = 0, in hebb_update and antiHebb_update

            //Update the anti-Hebbian matrix
            void antiHebb_update();

            // Hebbian dynamics, based on the slow synaptic synamics in Kinouchi2002
            void hebb_update();


    };
}

/*Everything related spiking neurons*/
namespace spiking{

		/*--- GGL structures, parameters and variables ---*/
		struct GGL_parameters{
			double v_reset;
			double mu;
			double v_base;
			double theta;
			double Gamma;
			std::vector<double>::size_type N;

		};
		struct GGL_synapses_parameters{
			double tau;
			double eps_fast;
			double eps_slow;
		};

		//GGL model vectors TODO: refactor - create generic structures
		struct GGL_vectors{
			//container for the state of neurons
			std::vector<int> x_vector;
			//container for the membrane potential
			std::vector<double> v_vector;
			//container the strenght of each pattern
			std::vector<double> pattern_strength;
			//activity vector, named to keep consistency with rate model
			std::vector<double> h_vec;
		};

		//GGL model matrices TODO: refactor - create generic structure
		//TODO: find a better way to store synapse info, this will get big for
		//large networks
		struct GGL_matrix{
			//slow hebbian dynamics container
			std::vector<std::vector<double>> slow_matrix;
			//fast anti-hebbian dynamics container
			std::vector<std::vector<double>> fast_matrix;
			//container for the patterns
			std::vector<std::vector<double>> pattern_matrix;
		};

		struct random_device{
        	std::mt19937 eng;
        	std::uniform_real_distribution<double> dist;
    	};



        class GGL{
            private:
			    GGL_parameters pars;
			    GGL_synapses_parameters synp;
			    GGL_vectors vecs;
			    GGL_matrix mat;

			    random_device device;

			    void prepare_random_device(double a, double b);
			    double draw();
				double phy(std::vector<double>::size_type i);

		    public:
				// important, the dynamics must start with an update on X!
				/*GGL contructor
				 * - ggl_par := parameters for the model
				 * - syn_par := parameters related to the synaptic matrices
				 * and dynamics
				 * - alpha := modulates the initial state of the potential
				 *   vector V_0 = alpha*random(0, 1)*/
                GGL(GGL_parameters ggl_par, GGL_synapses_parameters syn_par, double alpha);

				/*---- Supporting functionds ----*/
				/*Returns the network activity of nerun i*/
				double activity(std::vector<std::vector<double>>::size_type i);
				void net_activity();

				/* Very important patterns are stored in {-1, 1}
				 * pat must be a {0, 1} vector of states of the model
				 *pat will be converted to {-1, 1}. si = 2*xi - 1*/
				void add_exterior_pattern(std::vector<double>& pat, double strength);
				void add_random_pattern(double prob, double strength);
				void add_pn_pattern(std::vector<double>::size_type pn, double str);
				void add_orthogonal_pair(double str);

				/*Generates a random spike in the network*/
				void random_spike();

				/*---- Synapses functions----*/
				void make_hebb_matrix();
				void fast_syn_update();
				void slow_syn_update();

				/*---- Dynamics functions----*/
				double vtt(std::vector<double>::size_type i);
				int xtt(std::vector<int>::size_type i);
				void net_vtt();
				//void net_vtt(std::vector<double>& input);
				void net_xtt();

				/*---- Probing functions ----*/
				int rho();
				std::vector<int>& get_state();
				std::vector<double>& get_vvec();
				std::vector<double>& get_pattern(std::vector<std::vector<double>>::size_type mu);
				std::vector<std::vector<double>>& get_hebb();
				std::vector<std::vector<double>>& get_antiHebb();
    };

	/* Simplest GGL class which uses the adaped model
	 * IMPORTANT: S_i[t] = 2X_i[t] - 1. The state variable is not
	 * X in this model, it is S.
	 * IMPORTANT: rho*/
	class SimpleGGL{
		private:
			// network and neuron parameters
			// mu := neuron leakage term
			// gma := firing function gain
			// a := activity norm parameter

			std::vector<int>::size_type N;
			std::vector<std::vector<double>>::size_type Pnbr;
			unsigned int rho;
			double mu, gma, a; 

			// model variables
			std::vector<int> sstate; // binary state of network
			std::vector<double> vstate; //membrane potential
			std::vector<double> mvec; // projection vector
			std::vector<double> mvec_prime; // see quation 10 of GGL + patt
			std::vector<double> activity_vec;

			// patterns parameters
			std::vector<double> vstrg; //Fmu: stores the strength of patts
			std::vector<double> patt_sum; //sum of patterns *normalized
			std::vector<int> patt_matrix; //stores -1,1 patterns

			//random device variables
			random_device device;

			//private methods
			void prepare_random_device(double a, double b);
			double draw();
			double phy(std::vector<double>::size_type i);

		public:
			/*Constructor - pass basic parameters
			 * and generate a inital state*/
			SimpleGGL(std::vector<int>::size_type Nelem, double mupar, double gamma, double alpha);

			/*---- Supporting functions ----*/
			void add_random_pattern(double prob, double strength);
			void add_random_pm_pattern(std::vector<int>::size_type Pn, double strength);
			void add_exterior_pattern(std::vector<int>& patt, double strength);
			void random_spike();
			//void zero_mvec();
			/*---- Dynamic functions ----*/

			unsigned int rho_tt(); //ok TODO: remove, seems redundant
			double vtt(std::vector<int>::size_type i);//ok
			double vtt(std::vector<int>::size_type i, double inp);
			double m_utt(std::vector<int>::size_type u);//ok
			double mp_utt(std::vector<double>::size_type u);//ok
			int stt(std::vector<int>::size_type i);// ok
			double act(std::vector<int>::size_type i); // ok

		    void net_m_utt();// ok
			void net_vtt();// ok
			void net_vtt(std::vector<double>& input);// ok
			void net_stt();// ok
			void net_mp_utt(); // ok
			void net_activity();

			/*---- Probing functionsi ----*/
			std::vector<int>& get_state(); // S state vector
			std::vector<double>& get_vstate(); //membrane potential
			std::vector<double>& get_mvec(); // projection vector
			std::vector<int> get_pattern(std::vector<int>::size_type u); // pattern
			unsigned int get_rho();

	};

}

#endif
