#ifndef MODELS_H
#define MODELS_H

#include<string>
#include<vector>
#include<random>
#include<map>

namespace rate{
    struct parameters{
        std::vector<float>::size_type N;
        float gamma;
        float tau;
        float epsilon;
    };

    struct network_var{
        std::vector<float> s_vec;
        std::vector<float> h_vec;

        std::vector<std::vector<float>> P_matrix;
        std::vector<std::vector<float>> Hebb_matrix;
        std::vector<std::vector<float>> antiHebb_matrix;
    };

    struct random_device{
        std::mt19937 eng; //random number engine. Needs a seed (random device)
        std::uniform_real_distribution<float> dist; // distribution used for random process
    };


    class ItineratedMap{
        /**
         * Generates a network of rate based neurons interacting in a
         * all to all network.
         * Model from Kinouchi et. al. 2008
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
            // Constructors
            ItineratedMap(parameters& par, float a, float b);
            /*ItineratedMap(int n_elements, float gma);
            ItineratedMap(int n_elements, float a, float b, float gma);

            // Define the parameters for the connection matrix (anti-hebbian term)
            void anti_hebb_param(float _tau, float eps);

            // Define parameter of the model (gamma)
            void model_param(float gma);

            // Store new random state function
            void store_random_state(float a, float b);
            void store_external_state(std::vector<float>& vec);
            void store_pmone_random_state(float prob);

            //Genarate a new initial random state
            void state_zero_random();

            //Generate a new initial state from user
            void state_zero_external(std::vector<float> &state0);

            //Calculate the Hebbian part of the connection matrix
            void make_JHebbian();

            //Make the initial state of the anti-Hebbian matrix
            void anti_Hebb_init();

            //Update the anti-Hebbian matrix
            void anti_Hebb_update();

            //Update the activity vector of the network h(t)
            void activity_update();

            //Update the state of the network
            void state_update(); //without input
            void state_update(std::vector<float>& input); //with input

            // Probe function, gets a stored pattern
            std::vector<float>& get_xi(int u);

            //Probe state vector;
            std::vector<float>& get_State();

            //Probe activity
            std::vector<float>& get_activity();

            //Probe Hebb matrix
            std::vector<std::vector<float>>& get_hebb();
        */

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