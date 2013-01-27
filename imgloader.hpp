
#ifndef IMGLOADER_HPP_
# define IMGLOADER_HPP_

// # include <cstdio>


# include <exception>
# include <iostream>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>

# pragma pack(2)

typedef struct
{
  unsigned short	magic;
  unsigned int		bmp_size;
  unsigned int		id_app;
  unsigned int		start_offset;
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

#pragma pack()

class ImgLoader
{
private:

  unsigned char		*_data;
  unsigned int		_allocated_size;

public:

  ImgLoader() :
    _data(NULL),
    _allocated_size(0)
  {
  }

  ~ImgLoader()
  {
    if (_data)
      delete[] _data;
  }

  unsigned char		*getBmpData(const std::string &path)
  {
    int			fd;
    t_bmp_header	header;
    t_bmp_img_header	img_header;

    // std::cout << sizeof(header) << std::endl;
    // std::cout << sizeof(img_header) << std::endl;
    // if (_data)
    //   {
    // 	delete[] _data;
    // 	_data = NULL;
    //   }
    std::cout << "path : " << path << std::endl;
    if ((fd = open(path.c_str(), O_RDONLY)) == -1)
      {
	std::cerr << "Cannot open file " << path << std::endl;
	close(fd);
	return NULL;
      }
    if ((read(fd, &header, sizeof(header))) < 1)
      {
	std::cerr << "Cannot read file " << path << std::endl;
	close(fd);
	return NULL;
      }
    if ((read(fd, &img_header, sizeof(img_header))) < 1)
      {
	std::cerr << "Cannot read file " << path << std::endl;
	close(fd);
	return NULL;
      }
    unsigned int size = img_header.width * img_header.height * (img_header.bpp / 8);
    try
      {
	if (_allocated_size != size)
	  {
	    delete[] _data;
	    _data = NULL;
	  }
	if (!_data)
	  {
	    std::cout << "allocation de " << size << std::endl;
	    _data = new unsigned char[size];
	  }
      }
    catch (std::exception& e)
      {
	std::cerr << "Error : " <<  e.what() << std::endl;
	close(fd);
	return NULL;
      }
    _allocated_size = size;
    // std::cout << "done " << std::endl;
    // std::cout << "offset " << header.start_offset << " bmp size "<< header.bmp_size << std::endl;
    // std::cout << "inage header size " << img_header.img_header_size << "width " << img_header.width << " height " << img_header.height << "size " << size << " bpp " << img_header.bpp<< std::endl;
    if ((read(fd, _data, size)) < 1)
      {
	std::cerr << "Cannot read file data " << path << std::endl;
	close(fd);
	return NULL;
      }
    close(fd);

    // int h=0;
    // while (h < 600)
    //   {
    // 	printf("%hhX",_data[h]);
    // 	h++;
    //   }
    return _data;
  }

  void		deleteData(void)
  {
    if (_data)
      delete[] _data;
    _data = NULL;
  }
};

#endif
