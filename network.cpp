
#include "network.hpp"


Network::Network(unsigned int NbIn, unsigned int NbOut, std::vector<int> &NBHidden) :
    _INeurons(NbIn),
    _HNeurons(NBHidden.size()),
    _ONeurons(NbOut)
  {
    srand(time(NULL));

    std::vector<int>::iterator	it;
    int				i = 0;

    _Inputs = new unsigned int[NbIn];
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
  std::vector<std::vector<Neuron*> >::iterator it;
  
  std::cout << "[+] Allocate neurons" << std::endl;
  while (i < s)
    {
      _INeurons[i] = new Neuron;
      i++;
    }
  i = 0;
  s = _ONeurons.size();
  while (i < s)
    {
      _ONeurons[i] =  new Neuron;
      i++;
    }
  for (it = _HNeurons.begin(); it != _HNeurons.end(); it++)
    {
      s = (*it).size();
      i = 0;
      while (i < s)
	{
	  (*it)[i] = new Neuron;
	  i++;
	}
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

void			Network::loadInput(unsigned char *data, unsigned int size)
{
  memcpy(_Inputs, data, size * sizeof(*data));
}

int			*Network::getOutputs(unsigned char *data, unsigned int size)
{
  std::vector<Neuron*>::iterator	it;
  std::vector<Neuron*>::iterator	end;
  unsigned int			i = 0;
  
  end = _INeurons.end();
  if (data)
    this->loadInput(data, size);
  for (it = _INeurons.begin() ; it != end; it++)
    {
      (*it)->addSignal(_Inputs[i]);
      (*it)->proceed();
      i++;
    }
  
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
  for (it = _ONeurons.begin() ; it != end; it++)
    {
      _Outputs[i] = (*it)->getOutput();
      std::cout << "Output[" << i << "] = " << _Outputs[i] << std::endl;
      i++;
    }
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

inline float		Network::getOutSigma(int output, int answer)
{
  return (((float)answer - (float)output) * (float)output * (1.f - output));
}
//FIXME
inline float		Network::getMidSigma(int output, int answer)
{
  return (((float)answer - (float)output) * (float)output * (1.f - output));
}

//erase me
void			Network::adjustWeight(int *output, int *answer)
{
  float		delta;
  unsigned int	i = 0;
  float		sig;
  float		learning_ratio = 1.f;
    
  while (i < _ONeurons.size())
    {
      sig = this->getOutSigma(output[i], answer[i]);
      delta = learning_ratio * sig * (float)output[i];
      (void)delta;
      i++;
    }
}

//FIXME
void			Network::adjustLayerWeights(float learning_ratio,
						    std::vector<Neuron *> &layer)
{
  std::vector<Neuron*>::iterator	it;
  std::vector<Neuron*>::iterator	end;
  float					delta;
  float					output = 0.0;

  end = layer.end();
  for (it = layer.begin(); it != end; ++it)
    {
      delta = learning_ratio * this->getMidSigma(12,12) * output;      
    }
}
//FIXME
void			Network::adjustOutLayerWeights(float learning_ratio,
						       std::vector<Neuron *> &layer)
{
  std::vector<Neuron*>::iterator	it;
  std::vector<Neuron*>::iterator	end;
  float					delta;
  float					output = 0.0;

  end = layer.end();
  for (it = layer.begin(); it != end; ++it)
    {
      
      delta = learning_ratio * this->getOutSigma(12,12) * output;      
    }
}
//FIXME
void			Network::adjustWeights(int *output, int *answer)
{
  std::vector<std::vector<Neuron*> >::reverse_iterator	rit;
  std::vector<std::vector<Neuron*> >::reverse_iterator	rend;
  int			i = 0;

  rit = _HNeurons.rbegin();
  rend = _HNeurons.rend();
  for (; rit != rend; ++rit)
    {
      if (i == 0)
	{
	  this->adjustOutLayerWeights(1.f, *rit);
	}
      else
	{
	  this->adjustLayerWeights(1.f, *rit);
	}
    }
}
