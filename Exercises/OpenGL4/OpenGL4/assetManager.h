#include <map>

class AssetManager
{

public:
	AssetManager(void);
	~AssetManager(void);

private:
	std::map <char * , unsigned int> texture_list;

};

