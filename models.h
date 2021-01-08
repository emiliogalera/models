#ifndef MODELS_H
#define MODELS_H

#include<string>
#include<vector>
#include<random>
#include<map>

namespace rate{
    class ItineratedMap{
        /**
         * Generates a network of rate based neurons interacting in a
         * all to all network.
         * Model from Kinouchi et. al. 2008
        */
        private:
            // network variables
            std::vector<float>::size_type N; // number of elements in the system
            int P; // number of patterns stored in the system
            float gamma; // state update parameter

            std::mt19937 eng; //random number engine. Needs a seed (random device)
            std::uniform_real_distribution<float> dist; // distribution used for random process

            std::vector<float> State; // Current state vector
            std::map<int, std::vector<float>> Patterns; // pattern container

            std::vector<std::vector<float>> J_heb; // Hebbian part of the connection matrix
            std::vector<std::vector<float>> J_nheb; // Anti-hebbian of the connection matrix
            std::vector<float> h_vec; //activity vector, this vector stores the activity of every unit in that instant
            float epsilon; //anti-hebbian dynamic parameter
            float tau; //anti-hebbian dynamic parameter

            //void tanh();
        public:
            // Constructors
            ItineratedMap(int n_elements, float gma);
            ItineratedMap(int n_elements, float a, float b, float gma);

            // Define the parameters for the connection matrix (anti-hebbian term)
            void anti_hebb_param(float _tau, float eps);

            // Define parameter of the model (gamma)
            void model_param(float gma);

            // Store new random state function
            void store_random_state();

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

            //Update the activity vector of the network
            void activity_update();

            //Update the state of the network
            void state_update(); //without input
            void state_update(std::vector<float>& input); //with input

            // Probe function
            std::vector<float>& get_xi(int u);
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