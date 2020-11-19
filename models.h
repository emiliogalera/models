#ifndef MODELS_H
#define MODELS_H

#include<string>
#include<vector>
#include<map>

namespace rate{
    class itinerated_map{
        private:
            // network variables
            // number of elements in the system
            int N;
            // State vector
            std::vector<float> State;
            // pattern container
            std::map<int, std::vector<float>> Patterns;
        public:
            void hello(std::string msg);
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