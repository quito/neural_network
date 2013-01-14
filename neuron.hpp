
#ifndef NEURON_HPP_
# define NEURON_HPP_

# include <cstdlib>
# include <cmath>
# include <list>
# include <vector>
# include <iostream>

class Neuron;

typedef struct	s_connection
{
  Neuron	&neuron;
  float		connnectionWeight;

  s_connection(Neuron &n, float w):
    neuron(n),
    connnectionWeight(w)
  {}
}		t_connection;

class	Neuron
{
protected:

  std::list<t_connection*>		_OConnections;
  float					_outputSignal;
  float					_sum;
  float					_biais;

public:

  Neuron() :
    _outputSignal(1.0f),
    _sum(0),
    _biais(1.f)
  {
  }

  void			addConnection(Neuron &neuron)
  {
    t_connection	*c;
    float		t[] = {-1, 1};

    c = new t_connection(neuron, t[rand() % (sizeof(t) / sizeof(*t))]);
    _OConnections.push_back(c);
  }

  void			linkToLayer(std::vector<Neuron*> &Layer)
  {
    std::vector<Neuron*>::iterator	it;

    for (it = Layer.begin(); it != Layer.end(); it++)
      {
	this->addConnection(*(*it));
	// std::cout << "add connection" << std::endl;
      }
  }

  // sigmoid threshold check
  int		threshold(float x)
  {
    float	thresholdValue = 1.f;
    float	k = 1.f;
    float	result = 1.f / (1.f + exp(-k + x));

    // std::cout << "result " << result << std::endl;
    if (result > thresholdValue)
      return 1;
    return 0;
  }

  void		addSignal(float signal)
  {
    _sum += signal;
  }

  int		getOutput(void)
  {
    if (this->threshold(_sum + _biais) == 1)
      return 1;
    return 0;
  }

  void		proceed(void)
  {
    if (this->threshold(_sum + _biais) == 1)
      this->fire();
  }

  void		fire(void)
  {
    std::list<t_connection*>::iterator	it;
    std::list<t_connection*>::iterator	end;

    for (it = _OConnections.begin(); it != end; it++)
      if (*it)
	(*it)->neuron.addSignal(_outputSignal * (*it)->connnectionWeight);
    _sum = 0;
  }
};

#endif
