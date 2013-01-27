
#ifndef XORTRAINER_HPP_
# define XORTRAINER_HPP_

# include "network.hpp"

typedef struct
{
  bool		op1;
  bool		op2;
  bool		result;
}		t_xor_training_data;

static const t_xor_training_data	data_tab[] =
  {
    {0, 0, 0},
    {1, 0, 1},
    {0, 1, 1},
    {1, 1, 0},
  };

class XorTrainer
{

private:

  Network	*_net;

public:

  XorTrainer()
  {
    std::vector<int>    hlayer(1);

    hlayer[0] = 2;
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
	    _net->loadInput((unsigned char*)data, 2);
	    std::cout << "reponse : " << data_tab[i].result << std::endl;
	    outTab = _net->getOutputs();
	    _net->adjustWeights(answerTab);
	    g.update();
	    if (outTab[0] == answerTab[0])
	      std::cout << "GOOD ANSWER!!!" << std::endl;
	    usleep(1);
	  }
      }
  }
};

#endif
