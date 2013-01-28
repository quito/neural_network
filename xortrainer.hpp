
#ifndef XORTRAINER_HPP_
# define XORTRAINER_HPP_

# include "network.hpp"

typedef struct
{
  int		op1;
  int		op2;
  int		result;
}		t_xor_training_data;

static const t_xor_training_data	data_tab[] =
  {
    {-1, 1, 1},
    {1, -1, 1},
    {-1, -1, -1},
    {1, 1, -1}
  };

class XorTrainer
{

private:

  Network	*_net;

public:

  XorTrainer()
  {
    std::vector<int>    hlayer(1);

    hlayer[0] = 3;
    _net = new Network(2, 1, hlayer);
  }

  Network	*getNetwork(void) const {return _net;}

  void		train(void)
  {
    int		i;
    int		nb_passe = -1;
    int		answerTab[1];
    int		data[2];
    int		*outTab;

    while (nb_passe-- != 0)
      {
	for (i = 0; i < sizeof(data_tab) / sizeof(*data_tab); ++i)
	  {
	    answerTab[0] = data_tab[i].result;
	    data[0] = data_tab[i].op1;
	    data[1] = data_tab[i].op2;
	    std::cout << "i: " << i << " | " << data[0] << " ^ "<< data[1] << std::endl;
	    _net->loadInput(data, 2);
	    std::cout << "reponse : " << data_tab[i].result << std::endl;
	    outTab = _net->getOutputs();
	    _net->adjustWeights(answerTab);
	    g.update();
	    if (outTab[0] == answerTab[0])
	      std::cout << "GOOD ANSWER!!!" << std::endl;
	    std::cout << "================================="<< std::endl;
	    usleep(1);
	  }
      }
  }
};

#endif
