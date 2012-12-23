
#ifndef NETWORK_HPP_
# define NETWORK_HPP_

# include <vector>
# include <time.h>
# include "neuron.hpp"

class	Network
{
private:

  std::vector<Neuron*>	_INeurons;
  std::vector<Neuron*>	_HNeurons;
  std::vector<Neuron*>	_ONeurons;

public:

  Network()
  {
    srand(time(NULL));
  }
};

#endif
