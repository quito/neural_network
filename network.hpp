
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
  unsigned int		*_Outputs;

public:

  Network(unsigned int NbIn, unsigned int NbOut, std::vector<int> &NBHidden) :
    _INeurons(NbIn),
    _HNeurons(NBHidden.size()),
    _ONeurons(NbOut)
  {
    srand(time(NULL));

    std::vector<int>::iterator	it;
    int				i = 0;

    _Inputs = new unsigned int[NbIn];
    _Outputs = new unsigned int[NbOut];
    for (it = NBHidden.begin(); it != NBHidden.end(); it++)
      {
	_HNeurons[i].resize(*it);
	i++;
      }
    this->allocateNeurons();
    this->linkNeurons();
  }
  

  void		allocateNeurons(void)
  {
    int		i = 0;
    int		s = _INeurons.size();
    std::vector<std::vector<Neuron*> >::iterator it;

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

  void		linkNeurons(void)
  {
    std::vector<Neuron*>::iterator	it;
    std::vector<Neuron*>::iterator	end;
    std::vector<Neuron*>		*out = NULL;

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

  void			loadInput(unsigned char *data, unsigned int size)
  {
    memcpy(_Inputs, data, size * sizeof(*data));
  }

  int			guess(unsigned char *data = NULL, unsigned int size = 0)
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
    return 0;
  }
};

#endif
