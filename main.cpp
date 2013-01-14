
#include "neuron.hpp"
#include "network.hpp"
#include "imgtrainer.hpp"

int		main()
{
  // Network	n(1764 * 1200, 2, hlayer);
  std::cout << "[+] Creating Neural Network" << std::endl;
  ImgTrainer	trainer("./numbers/", 1200, 1764);
  std::cout << "[+] Done" << std::endl;
  Network	*n = NULL;

  n = trainer.getNetwork();
  return 0;
}
