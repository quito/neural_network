
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
  double		connnectionWeight;

  s_connection(Neuron &n, double w):
    neuron(n),
    connnectionWeight(w)
  {}
}		t_connection;

class	Neuron
{
protected:

  std::list<t_connection*>		_OConnections;
  double					_outputSignal;
  double					_sum;
  double					_biais;
  double					_lastOut;
  double					_lastSigma;

public:

  Neuron() :
    _outputSignal(1.0f),
    _sum(0),
    _biais(1.f),
    _lastOut(0),
    _lastSigma(0)
  {
  }

  inline std::list<t_connection*>	&getConnections(void)
  {
    return _OConnections;
  }
  inline double				getLastOut(void)
  {
    return _lastOut;
  }

  inline void		setLastSigma(double s)
  {
    _lastSigma = s;
  }
  
  inline double		getLastSigma(void) const
  {
    return _lastSigma;
  }

  void			addConnection(Neuron &neuron)
  {
    t_connection	*c;
    // double		t[] = {-1, 1};

    // c = new t_connection(neuron, t[rand() % (sizeof(t) / sizeof(*t))]);
    c = new t_connection(neuron, (double)(rand() % 100) / 100.f - 0.5);
    // std::cout << "connection weight : " << c->connnectionWeight<< std::endl;
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


  double		getSigmoidResult(double x)
  {
    return 1.f / (1.f + exp(-(x)));
  }

  // sigmoid threshold check
  double		threshold(double x)
  {
    // double	thresholdValue = 1.f;
    // double	k = 1.f;
    // double	result = 1.f / (1.f + (double)expf(-k + x));
    // double	result = 1.f / (1.f);
    double	result = 1.f / (1.f + exp(-x));
    if (result != 0 && x != 1)
      std::cout << "result = " << result << "car exp " << -x << " = " << exp(-x) << std::endl; 
    if (result > 0.5)
      return 1.f;
    return 0;
    // if (result > thresholdValue)
    //   return 1;
    // return 0;
  }

  void		addSignal(double signal)
  {
    _sum += signal;
    // if (_sum != 0)
    //   std::cout << "sum_value = " << _sum << std::endl;
  }

  //FIXME
  double		getOutput(void)
  {
    double	out = this->threshold(_sum + _biais);
    _lastOut = this->getSigmoidResult(_sum + _biais);
    // std::cout << "OUT:   " << out << std::endl;
    return out;
  }

  void		proceed(void)
  {
    // if (this->threshold(_sum + _biais) > 0.5)
      this->fire();
  }

  void		fire(void)
  {
    std::list<t_connection*>::iterator	it;
    std::list<t_connection*>::iterator	end;

    // _lastOut = this->threshold(_sum);
    _lastOut = this->getSigmoidResult(_sum + _biais);
    end = _OConnections.end();
    for (it = _OConnections.begin(); it != end; it++)
      if (*it)
	(*it)->neuron.addSignal(this->threshold(_sum + _biais) * (*it)->connnectionWeight);
	// (*it)->neuron.addSignal(_outputSignal * (*it)->connnectionWeight);
    _sum = 0;
  }
};

#endif
