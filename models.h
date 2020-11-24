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
         * Model from Kinouchi 2008
        */
        private:
            // network variables
            int N; // number of elements in the system
            int P; // number of patterns to be stored in the system
            std::mt19937 eng; //random number engine. Needs a seed (random device)
            std::uniform_real_distribution<float> dist;

            std::vector<float> State; // State vector
            std::map<int, std::vector<float>> Patterns; // pattern container

            //void tanh();
        public:
            ItineratedMap(int n_elements);
            ItineratedMap(int n_elements, float a, float b);
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