
#include "network.hpp"


#define ALPHA	0.00

Network::Network(unsigned int NbIn, unsigned int NbOut, std::vector<int> &NBHidden) :
    _INeurons(NbIn),
    _HNeurons(NBHidden.size()),
    _ONeurons(NbOut)
  {
    srand(time(NULL));

    std::vector<int>::iterator	it;
    int				i = 0;

    _Inputs = new int[NbIn];
    _InputsWeights = new double[NbIn];
    _Outputs = new int[NbOut];
    for (it = NBHidden.begin(); it != NBHidden.end(); it++)
      {
	_HNeurons[i].resize(*it);
	i++;
      }
    this->allocateNeurons();
    this->linkNeurons();
  }
  

void		Network::allocateNeurons(void)
{
  int		i = 0;
  int		s = _INeurons.size();
  int		y = 0;
  std::vector<std::vector<Neuron*> >::iterator it;
  
  std::cout << "[+] Allocate neurons" << std::endl;
  while (i < s)
    {
      _INeurons[i] = new Neuron;
      _INeurons[i]->setPos(NEURON_X_START_POS, (NEURON_SIZE + SPACE_BETWEEN_NEURON) * i + NEURON_Y_START_POS);
      i++;
    }
  ++y;
  for (it = _HNeurons.begin(); it != _HNeurons.end(); it++)
    {
      s = (*it).size();
      i = 0;
      while (i < s)
	{
	  (*it)[i] = new Neuron;
	  (*it)[i]->setPos(NEURON_X_START_POS + y * 300, (NEURON_SIZE + SPACE_BETWEEN_NEURON + 30) * i + NEURON_Y_START_POS + 120);
	  i++;
	}
      ++y;
    }
  i = 0;
  s = _ONeurons.size();
  while (i < s)
    {
      _ONeurons[i] =  new Neuron;
      _ONeurons[i]->setPos(NEURON_X_START_POS + y * 300, (NEURON_SIZE + SPACE_BETWEEN_NEURON + 40) * i + NEURON_Y_START_POS + 150);
      i++;
    }
}

void		Network::linkNeurons(void)
  {
    std::vector<Neuron*>::iterator	it;
    std::vector<Neuron*>::iterator	end;
    std::vector<Neuron*>		*out = NULL;

    std::cout << "[+] Linking first layer" << std::endl;
    end = _INeurons.end();
    if (_HNeurons.size() != 0)
      out = &(_HNeurons[0]);
    else if (_ONeurons.size() != 0)
      out = &_ONeurons;
    else
      out = NULL;

    if (out)
      for (it = _INeurons.begin() ; it != end; it++)
	{
	  (*it)->linkToLayer(*out);
	  // std::cout << "linked" << std::endl;
	}
    std::vector<std::vector<Neuron*> >::iterator	ith;
    std::vector<std::vector<Neuron*> >::iterator	endh;
    int							nbLayers = _HNeurons.size();
    int							i = 0;

    endh = _HNeurons.end();
    for (ith = _HNeurons.begin() ; ith != endh; ith++)
      {
	std::cout << "[+] Linking hiddens layers : " << i << "\n" << std::endl;
	++i;
	if (i != nbLayers)
	  {
	    it = (*ith).begin();
	    end = (*ith).end();
	    for (; it != end; it++)
	      {
		(*it)->linkToLayer(_HNeurons[i]);
		// std::cout << "linked" << std::endl;
	      }
	  }
	else
	  {
	    it = (*ith).begin();
	    end = (*ith).end();
	    for (; it != end; it++)
	      {
		(*it)->linkToLayer(_ONeurons);
		// std::cout << "linked" << std::endl;
	      }
	  }
	
      }
  }

void			Network::loadInput(int *data, unsigned int size)
{
  unsigned		i = 0;

  while (i < size)
    {
      // _Inputs[i] = data[i * 4] ? 1 : 0;
      _Inputs[i] = data[i] & 0xFF0000 ? 1 : -1;
      // _InputsWeights[i] = (double)(rand() % 100) / 100.f;
      _InputsWeights[i] = (double)(rand() % 100) / 50.f - 1.0;
      // std::cout << "data = "<< data[i] << std::endl;
      ++i;
    }
  // memcpy(_Inputs, data, size * sizeof(*data));
}

int			*Network::getOutputs(int *data, unsigned int size)
{
  std::vector<Neuron*>::iterator	it;
  std::vector<Neuron*>::iterator	end;
  unsigned int			i = 0;
  
  end = _INeurons.end();
  if (data)
    this->loadInput(data, size);
  // propagate through the first layer
  for (it = _INeurons.begin() ; it != end; it++)
    {
      // (*it)->addSignal((_Inputs[i] & 0xFFFFFF) ? 1 : 0);
      (*it)->addSignal((_Inputs[i] > 0.0 ? 1 : -1) * _InputsWeights[i]);
      // g.update();
      i++;
    }
  for (it = _INeurons.begin() ; it != end; it++)
    (*it)->proceed();
  
  std::vector<std::vector<Neuron*> >::iterator	ith;
  std::vector<std::vector<Neuron*> >::iterator	endh;
  endh = _HNeurons.end();
  for (ith = _HNeurons.begin(); ith != endh; ith++)
    {
      end = (*ith).end();
      for (it = (*ith).begin(); it != end; it++)
	(*it)->proceed();
    }

  end = _ONeurons.end();
  i = 0;
  static unsigned z = 0;
  for (it = _ONeurons.begin() ; it != end; it++)
    {
      _Outputs[i] = (*it)->getOutput() < 0.0 ? -1 : 1;
      // if (!(z % 2000))
	std::cout << "Output[" << i << "] = " << _Outputs[i] << std::endl;
      i++;
    }
  ++z;
  // if (!(z % 2000))
  return _Outputs;
}

int			Network::guess()
{
  int			*outs;
  unsigned int	i;
  
  outs = this->getOutputs();
  i = 0;
    while (i < _ONeurons.size())
      {
	if (outs[i] != 0)
	  return outs[i];
	i++;
      }
    return -1;
}

inline double		Network::getOutSigma(int output, int answer)
{
  return (((double)answer - (double)output) * (double)output * (1.f - output));
}

// //erase me
// void			Network::adjustWeight(int *output, int *answer)
// {
//   double		delta;
//   unsigned int	i = 0;
//   double		sig;
//   double		learning_ratio = 1.f;
    
//   while (i < _ONeurons.size())
//     {
//       sig = this->getOutSigma(output[i], answer[i]);
      
//       delta = learning_ratio * sig * (double)output[i];
//       (void)delta;
//       i++;
//     }
// }

//FIXME
void			Network::adjustOutConnectionWeight(double learning_ratio, Neuron &neuron,
							   int *answers)
{
  std::list<t_connection *>::iterator	it;
  std::list<t_connection *>::iterator	end;
  std::list<t_connection *>		&connections = neuron.getConnections();
  double					delta;
  double					sigma;
  int					i = 0;

  end = connections.end();
  for (it = connections.begin(); it != end; ++it)
    {
      sigma = ((float)answers[i] - (*it)->neuron.getLastOut()) * (*it)->neuron.getLastOut() * (1.f - (*it)->neuron.getLastOut());
      (*it)->neuron.setLastSigma(sigma);
      delta = learning_ratio * sigma * neuron.getLastOut() + ALPHA * (*it)->connnectionWeight;
      (*it)->connnectionWeight += delta;
      // if (delta != 0.f)
	// std::cout << "delta = " << delta<< std::endl;
      // std::cout << "last out : " << (*it)->neuron.getLastOut() << std::endl;
      ++i;
    }
  neuron.drawConnections();
}

//FIXME
void			Network::adjustOutLayerWeights(double learning_ratio,
						       std::vector<Neuron *> &layer,
						       int *answers)
{
  std::vector<Neuron*>::iterator	it;
  std::vector<Neuron*>::iterator	end;
  int					i = 0;

  end = layer.end();
  for (it = layer.begin(); it != end; ++it)
    {
      this->adjustOutConnectionWeight(learning_ratio, *(*it), answers);
      ++i;
    }
}

double			Network::getSigmaSum(Neuron &neuron)
{
  std::list<t_connection *>::iterator	it;
  std::list<t_connection *>::iterator	end;
  std::list<t_connection *>             &connections = neuron.getConnections();
  double				sigma = 0;
  
  end = connections.end();
  for (it = connections.begin(); it != end; ++it)
    sigma += ((*it)->neuron.getLastSigma() * (*it)->connnectionWeight);
  return sigma;
}

void			Network::adjustLayerConnectionWeight(double learning_ratio, Neuron &neuron)
{
  std::list<t_connection *>::iterator	it;
  std::list<t_connection *>::iterator	end;
  std::list<t_connection *>		&connections = neuron.getConnections();
  double					delta;
  double					sigma;
  int					i = 0;

  end = connections.end();
  for (it = connections.begin(); it != end; ++it)
    {
      sigma = this->getSigmaSum((*it)->neuron) * (*it)->neuron.getLastOut() * (1.f - (*it)->neuron.getLastOut());
      (*it)->neuron.setLastSigma(sigma);
      delta = learning_ratio * sigma * neuron.getLastOut() + ALPHA * (*it)->connnectionWeight;
      // std::cout << "delta = " << delta << std::endl;
      (*it)->connnectionWeight += delta;
      // if (delta != 0.f)
	// std::cout << "delta = " << delta<< std::endl;
      // std::cout << "last out : " << (*it)->neuron.getLastOut() << std::endl;
      ++i;
    }
  neuron.drawConnections();
}

void			Network::adjustLayerWeights(double learning_ratio,
						    std::vector<Neuron *> &layer)
{
  std::vector<Neuron*>::iterator	it;
  std::vector<Neuron*>::iterator	end;
  int					i = 0;

  end = layer.end();
  for (it = layer.begin(); it != end; ++it)
    {
      this->adjustLayerConnectionWeight(learning_ratio, *(*it));
      ++i;
    }  
}

void			Network::adjustInputWeights(double learning_ratio,
						    std::vector<Neuron*> &Input)
{
  double					delta;
  double					sigma;
  int					i = 0;

  for (; i < Input.size(); ++i)
    {
      sigma = this->getSigmaSum(*(Input[i])) * Input[i]->getLastOut() * (1.f - Input[i]->getLastOut());
      Input[i]->setLastSigma(sigma);
      delta = learning_ratio * sigma * _Inputs[i] + ALPHA * _InputsWeights[i];
      // std::cout << "delta = " << delta << std::endl;
      _InputsWeights[i] += delta;
      // if (delta != 0.f)
	// std::cout << "delta = " << delta<< std::endl;
      // std::cout << "last out : " << (*it)->neuron.getLastOut() << std::endl;
      ++i;
    }
  // Input->drawConnections();
}

//FIXME
void			Network::adjustWeights(int *answer)
{
  std::vector<std::vector<Neuron*> >::reverse_iterator	rit;
  std::vector<std::vector<Neuron*> >::reverse_iterator	rend;
  int			i = 0;

  rit = _HNeurons.rbegin();
  rend = _HNeurons.rend();
  for (; rit != rend; ++rit)
    {
      if (i == 0)
	this->adjustOutLayerWeights(1.f, *rit, answer);
      else
	this->adjustLayerWeights(1.f, *rit);
    }
  this->adjustLayerWeights(1.f, _INeurons);
  this->adjustInputWeights(1.f, _INeurons);
}
