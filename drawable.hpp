
#ifndef DRAWABLE_HPP_
# define DRAWABLE_HPP_

# include "graphic.hpp"

class	Drawable
{
protected:

  unsigned	_posX;
  unsigned	_posY;
  u_color	_color;
  // Graphic	&graphic;

public:

  virtual void		draw(void) = 0;
};
#endif
