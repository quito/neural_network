
#ifndef IMGLOADER_HPP_
# define IMGLOADER_HPP_

# include <iostream>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>

typedef struct
{
  unsigned short	magic;
  unsigned int		bmp_size;
  unsigned short	id_app1;
  unsigned short	id_app2;
  char			*start_offset;
}		t_bmp_header;

typedef struct
{
  unsigned int		img_header_size;
  unsigned int		width;
  unsigned int		height;
  unsigned short	nb_plan;
  unsigned short	bpp;
  unsigned int		compression;
  unsigned int		img_size;
  unsigned int		wppm;
  unsigned int		hppm;
  unsigned int		palette;
  unsigned int		important_color;
}		t_bmp_img_header;

class ImgLoader
{
private:

  unsigned char		*_data;

public:

  ImgLoader() :
    _data(NULL)
  {
  }

  ~ImgLoader()
  {
    if (_data)
      delete _data;
  }

  unsigned char		*getBmpData(const std::string &path)
  {
    int			fd;
    t_bmp_header	header;
    t_bmp_img_header	img_header;
    unsigned char	*data;

    if ((fd = open(path.c_str(), O_RDONLY)) == -1)
      {
	std::cerr << "Cannot open file " << path << std::endl;
	return NULL;
      }
    if (_data)
      delete _data;
    _data = NULL;
    if ((read(fd, &header, sizeof(header))) < 1)
      {
	std::cerr << "Cannot read file " << path << std::endl;
	return NULL;
      }
    if ((read(fd, &img_header, sizeof(img_header))) < 1)
      {
	std::cerr << "Cannot read file " << path << std::endl;
	return NULL;
      }
    unsigned int size = img_header.width * img_header.height * (img_header.bpp / 8);
    data = new unsigned char[size];
    if ((read(fd, data, size)) < 1)
      {
	std::cerr << "Cannot read file data " << path << std::endl;
	return NULL;
      }
    close(fd);
    return data;
  }

  void		deleteData(void)
  {
    if (_data)
      delete _data;
    _data = NULL;
  }
};

#endif
