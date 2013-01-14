
#ifndef IMGTRAINER_HPP
# define IMGTRAINER_HPP

# include <string>
# include <algorithm>
# include <dirent.h>
# include <sys/types.h>

# include "network.hpp"
# include "trainer.hpp"

typedef struct 
{
  const std::string	folder_path;
  int			solution;
  }			t_training_data;

static const t_training_data		folder_tab[] = 
  {
    {"0", 0},
    {"1", 1},
    {"2", 2},
    {"3", 3},
    {"4", 4},
    {"5", 5},
    {"6", 6},
    {"7", 7},
    {"8", 8},
    {"9", 9}
  };

class ImgTrainer : public Trainer
{
private:

  std::string		_path;
  std::vector<std::pair<std::string, int> >	_fileList;

public:

  ImgTrainer(Network &net, std::string const &path) :
    Trainer(net),
    _path(path)
  {
    loadFilelist();
    std::random_shuffle(_fileList.begin(), _fileList.end());
  }

  void			loadFilelist(void)
  {
    DIR			*dir = NULL;
    std::string		path;
    unsigned int	i = 0;
    struct dirent	*directory;

    while (i < sizeof(folder_tab) / sizeof(*folder_tab))
      {
	path = _path + "/" + folder_tab[i].folder_path;
	dir = opendir(path.c_str());
	if (!dir)
	  {
	    std::cerr << "Cannot open " << path << std::endl;
	    i++;
	    continue;
	  }
	while ((directory = readdir(dir)) != NULL)
	  {
	    if (directory->d_name[0] == '.')
	      continue;
	    _fileList.push_back(make_pair(path + "/" + directory->d_name,
					  folder_tab[i].solution));
	    std::cout << path + "/" + directory->d_name << std::endl;
	  }
	closedir(dir);
	i++;
      }
    }
  
};

#endif






  // std::string tmp = "convert " + path + "/" + directory->d_name + " " + path + "/" + directory->d_name + ".bmp";
  
  // std::cout << tmp << std::endl;
  // system(tmp.c_str());
