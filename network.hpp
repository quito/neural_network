
#ifndef NETWORK_HPP_
# define NETWORK_HPP_

# include <vector>
# include <time.h>
# include <iostream>
# include <cstring>
# include "neuron.hpp"

class	Network
{
private:

  unsigned int		*_Inputs;
  std::vector<Neuron*>	_INeurons;
  std::vector<std::vector<Neuron*> >	_HNeurons;
  std::vector<Neuron*>	_ONeurons;
  int		*_Outputs;

public:

  Network(unsigned int NbIn, unsigned int NbOut, std::vector<int> &NBHidden);

  inline unsigned	getOutLayerSize(void) const {return _ONeurons.size();}
  void			allocateNeurons(void);
  void			linkNeurons(void);
  void			loadInput(unsigned char *data, unsigned int size);
  int			*getOutputs(unsigned char *data = NULL, unsigned int size = 0);
  int			guess();
  inline double		getOutSigma(int output, int answer);
  void			adjustOutConnectionWeight(double learning_ratio, Neuron &neuron,
						  double answer);
  void			adjustWeight(int *output, int *answer);
  void			adjustOutLayerWeights(double learning_ratio, std::vector<Neuron *> &layer,
					      int *output, int *answer);
  void			adjustWeights(int *output, int *answer);
};

#endif
