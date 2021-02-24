#ifndef MODELS_H
#define MODELS_H

#include<string>
#include<vector>
#include<random>
#include<map>

namespace rate{
    /* Group the parameters of the model:
     * N = size of the network
     * gamma = gain parameter of the state map function
     * tau = characteristic time of the anti-hebbian dynamics ***stores 1-1/t
     * epsilon = equilibrium term of the anti-hebbian dynamics ***stores e/N
     */
    struct parameters{
        std::vector<float>::size_type N;
        float gamma;
        float tau;
        float eps_ah;
        float eps_h;
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
        std::vector<float> s_vec;
        std::vector<float> h_vec;
        std::vector<float> f_vec;

        std::vector<std::vector<float>> P_matrix;
        std::vector<std::vector<float>> Hebb_matrix;
        std::vector<std::vector<float>> antiHebb_matrix;
    };

    /* Group the variables responsible for the random number generators
     * eng = Egine used to generate randomness
     * dist = Distribution used to draw random numbers, for now it uses a
     * uniform distribution
     */
    struct random_device{
        std::mt19937 eng; //random number engine. Needs a seed (random device)
        std::uniform_real_distribution<float> dist; // distribution used for random process
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
            void prepare_random_device(float a, float b);

            //Draws from the distribution
            float draw();

        public:
            // Constructor of the ItineratedMap class
            // Initiate the state vector with random values in the range (a, b)
            ItineratedMap(parameters& par, float a, float b);

            // Instanciate a class from parameters and initial state provided by the user
            ItineratedMap(parameters& par, std::vector<float>& vec);

            // Stores a random pattern, where xi in [-1, 1].
            void add_random_pattern(float prob, float strength);

            // Stores a pattern provided by the user
            void add_exterior_pattern(std::vector<float>& vec, float strength);

            // Generates the Hebbian matrix part of the connection
            void make_hebb_matrix();

            /*---- Probing methods ----*/
            /*---- These methods return references, be careful! ----*/
            // Get pattern by index mu
            std::vector<float>& get_xi(int u);

            //Probe state vector;
            std::vector<float>& get_State();

            //Probe activity
            std::vector<float>& get_activity();

            //Probe Hebb matrix
            std::vector<std::vector<float>>& get_hebb();

            //Probe antiHebb matrix
            std::vector<std::vector<float>>& get_antihebb();

            // Generate m_vector of projection of the current state on to stored
            //patterns
            void generate_m(std::vector<float>& m_recipient);

            /*---- Update methods ----*/
            //Update the activity vector of the network h(t)
            void activity_update();

            //Update the state of the network
            void state_update_tgh(); //without input
            void state_update_tgh(std::vector<float>& input); //with input

            void state_update_rational(); //without input
            void state_update_rational(std::vector<float>& input); //with input

            // IMPORTANT: both connection matrices don't considere self-connections
            // Jii = 0, in hebb_update and antiHebb_update

            //Update the anti-Hebbian matrix
            void antiHebb_update();

            // Hebbian dynamics, based on the slow synaptic synamics in Kinouchi2002
            void hebb_update();


    };
}

namespace spiking{
        class lif{
        private:
            int N;
        public:
            void hello(std::string msg);
    };
}

#endif