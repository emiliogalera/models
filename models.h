#ifndef MODELS_H
#define MODELS_H

#include<string>
#include<vector>

namespace rate{
    class itinerated_map{
        private:
            // network variables
            int N;
            std::vector<float> State;
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