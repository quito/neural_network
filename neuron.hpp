
#ifndef NEURON_HPP_
# define NEURON_HPP_

# include <cstdlib>
# include <cmath>
# include <list>
# include <vector>
# include <iostream>

# include "graphic.hpp"
# include "drawable.hpp"

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

class	Neuron : public Drawable
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
    _posX = 50;
    _posY = 50;
    _color.color = 0x00FF00;
  }

  // Draw
  unsigned				getPosX(void) const {return _posX;}
  unsigned				getPosY(void) const {return _posY;}
  void					setPos(unsigned x, unsigned y)
  {
    _posX = x;
    _posY = y;
  }
  void					setColor(unsigned color)
  {
    _color.color = color;
  }
  void					modifyColorFromSum(void)
  {
    _color.color = 0x0;
    if (_sum < 0)
      _color.tab[1] = 0xFF * (_sum * -1);
    else
      _color.tab[2] = 0xFF * (_sum);
  }
  unsigned			getColorFromConnection(t_connection *c)
  {
    u_color			color;

    color.color = 0;
    if (c->connnectionWeight < 0)
      color.tab[1] = 0xFF * (c->connnectionWeight * -1);
    else
      color.tab[2] = 0xFF * (c->connnectionWeight);
    return color.color;
  }

  void					drawConnections(void)
  {
    std::list<t_connection *>::iterator	it;
    std::list<t_connection *>::iterator	end;

    end = _OConnections.end();
    for (it = _OConnections.begin(); it != end; ++it)
      {
	g.drawLine(_posX, _posY, (*it)->neuron.getPosX(), (*it)->neuron.getPosY(),
		   this->getColorFromConnection(*it));
      }
  }

  virtual void				draw(void)
  {
    // g.PutPixel(_posX, _posY, _color.color);
    // g.PutPixel(_posX-1, _posY, _color.color);
    // g.PutPixel(_posX-2, _posY, _color.color);
    // g.PutPixel(_posX+1, _posY, _color.color);
    // g.PutPixel(_posX+2, _posY, _color.color);
    // g.PutPixel(_posX, _posY+1, _color.color);
    // g.PutPixel(_posX, _posY+2, _color.color);
    // g.PutPixel(_posX, _posY-1, _color.color);
    // g.PutPixel(_posX, _posY-2, _color.color);
    modifyColorFromSum();
    g.drawSquare(_posX, _posY, _color.color, NEURON_SIZE);
  }
  //=========


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
    // if (result != 0 && x != 1)
    //   std::cout << "result = " << result << "car exp " << -x << " = " << exp(-x) << std::endl; 
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
    this->draw();
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
