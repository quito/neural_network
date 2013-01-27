
#ifndef GRAPHIC_HPP_
# define GRAPHIC_HPP_

# include <SDL/SDL.h>

// # define NEURON_SIZE 4
# define NEURON_SIZE 30
// # define SPACE_BETWEEN_NEURON 1.15
# define SPACE_BETWEEN_NEURON 200
# define NEURON_X_START_POS 30
# define NEURON_Y_START_POS 10

typedef union
{
  unsigned	color;
  unsigned char	tab[4];
}		u_color;

class Graphic
{
private:
  unsigned	_x;
  unsigned	_y;
  SDL_Surface	*_screen;

public:

  Graphic(unsigned x, unsigned y):
    _x(x),
    _y(y),
    _screen(NULL)
  {
    SDL_Init(SDL_INIT_VIDEO);
    _screen = SDL_SetVideoMode(_x, _y, 32, SDL_HWSURFACE);
    SDL_WM_SetCaption("neural network", NULL);
    SDL_FillRect(_screen, NULL, SDL_MapRGB(_screen->format, 0, 0, 0));
  }
  
  ~Graphic()
  {
    SDL_Quit();
  }

  void update(void)
  {
    SDL_Flip(_screen);
  }

  void PutPixel(int x, int y, unsigned long pixel)
  {
    if (!_screen)
      return ;
    int bpp = _screen->format->BytesPerPixel;
    unsigned char *p = (unsigned char *)_screen->pixels + y * _screen->pitch + x * bpp;
    if (bpp==4)
      *(unsigned long*)p = pixel;
  }

  void	drawSquare(int x, int y, unsigned color, unsigned size)
  {
    unsigned	i = 0;
    unsigned	j = 0;

    for (i = 0; i < size; ++i)
      for (j = 0; j < size; ++j)
	this->PutPixel(i + x, j + y, color);
  }

  void drawLine(int x1,int y1, int x2,int y2, unsigned color)
  {
    unsigned long couleur = color;
    int x,y;
    int Dx,Dy;
    int xincr,yincr;
    int erreur;
    int i;
 
    Dx = abs(x2-x1);
    Dy = abs(y2-y1);
    if(x1<x2)
      xincr = 1;
    else
      xincr = -1;
    if(y1<y2)
      yincr = 1;
    else
      yincr = -1;
 
    x = x1;
    y = y1;
    if(Dx>Dy)
      {
	erreur = Dx/2;
	for(i=0;i<Dx;i++)
	  {
	    x += xincr;
	    erreur += Dy;
	    if(erreur>Dx)
	      {
		erreur -= Dx;
		y += yincr;
	      }
	    this->PutPixel(x, y,couleur);
	  }
      }
    else
      {
	erreur = Dy/2;
	for(i=0;i<Dy;i++)
	  {
	    y += yincr;
	    erreur += Dx;
 
	    if(erreur>Dy)
	      {
		erreur -= Dy;
		x += xincr;
	      }
	    this->PutPixel(x, y, couleur);
	  }
      }
  }
};

extern Graphic g;


#endif
