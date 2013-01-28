
#ifndef IMGTRAINER_HPP
# define IMGTRAINER_HPP

# include <string>
# include <algorithm>
# include <dirent.h>
# include <sys/types.h>

# include "network.hpp"
# include "trainer.hpp"
# include "imgloader.hpp"

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

class ImgTrainer
{
private:

  std::string					_path;
  std::vector<std::pair<std::string, int> >	_fileList;
  unsigned int					_x;
  unsigned int					_y;
  Network					*_net;
  ImgLoader					_imgLoader;

public:

  ImgTrainer(std::string const &path, unsigned int x, unsigned int y) :
    _path(path),
    _x(x),
    _y(y)
  {
    std::vector<int>	hlayer(1);
    // hlayer[0] = ((_x * _y) - (sizeof(folder_tab) / sizeof(*folder_tab))) / 2 ;
    hlayer[0] = 15;

    loadFilelist();
    std::random_shuffle(_fileList.begin(), _fileList.end());
    _net = new Network(_x * _y, sizeof(folder_tab) / sizeof(*folder_tab), hlayer);
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
  
    Network		*getNetwork(void) const
    {
      return _net;
    }

  int			*buildAnswerTab(int answer)
  {
    unsigned		size = _net->getOutLayerSize();
    unsigned 		i;
    int			*answerTab = new int[size];
    
    for (i = 0; i < size; ++i)
	answerTab[i] = -1;
    if (answer < (signed int)size)
      answerTab[answer] = 1;
    return answerTab;
  }

    void		train(void)
    {
      std::vector<std::pair<std::string, int> >::iterator	it;
      std::vector<std::pair<std::string, int> >::iterator	end;
      int		*data;
      int		answer;
      int		*answerTab;
      int		*outs;

      unsigned int i = 0;

      data = NULL;
      end = _fileList.end();
      if (!_net)
	return ;
      while (1)
      for (it = _fileList.begin() ; it != end ; it++)
	{
	  // std::cout << "size : " << _fileList.size() << std::endl;
	  answer = (*it).second;
	  answerTab = this->buildAnswerTab(answer);
	  if (!(data = (int*)_imgLoader.getBmpData((*it).first)))
	    continue;
	  // _net->loadInput(data, _x * _y * 4);
	  _net->loadInput(data, _x * _y);
	  data = NULL;
	  // if (!(i % 2000))
	    std::cout << "reponse : " << answer << std::endl;
	  outs = _net->getOutputs(); //guess
	  _net->adjustWeights(answerTab);
 	  g.update();
	  ++i;
	  // usleep(500000);
	}
      _imgLoader.deleteData();
    }
};

#endif






  // std::string tmp = "convert " + path + "/" + directory->d_name + " " + path + "/" + directory->d_name + ".bmp";
  
  // std::cout << tmp << std::endl;
  // system(tmp.c_str());
