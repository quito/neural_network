
#ifndef TRAINER_HPP
# define TRAINER_HPP

# include "network.hpp"

class Trainer
{
protected:

  Network	&_net;

public:

  Trainer(Network &net) :
    _net(net)
  {
    
  }
};

#endif
