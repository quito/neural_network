
#include "neuron.hpp"
#include "network.hpp"

int		main()
{
  std::vector<int>	hlayer(1);
  hlayer[0] = 10;
  Network	n(5, 2, hlayer);
  return 0;
}
