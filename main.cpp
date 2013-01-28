
#include "neuron.hpp"
#include "network.hpp"
#include "imgtrainer.hpp"
#include "graphic.hpp"
#include "xortrainer.hpp"

Graphic	g(800, 800);


int		main()
{
  // Network	n(1764 * 1200, 2, hlayer);
  std::cout << "[+] Creating Neural Network" << std::endl;
  ImgTrainer	trainer("./numbers/", 10, 15);
  // XorTrainer	trainer;
  std::cout << "[+] Done" << std::endl;
  Network	*n = NULL;

  n = trainer.getNetwork();
  trainer.train();
  while (1);
  return 0;
}
