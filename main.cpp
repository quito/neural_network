
#include "neuron.hpp"
#include "network.hpp"
#include "imgtrainer.hpp"

int		main()
{
  std::vector<int>	hlayer(1);
  hlayer[0] = 10;
  Network	n(5, 2, hlayer);
  ImgTrainer	trainer(n, "./numbers/");
  
  return 0;
}
