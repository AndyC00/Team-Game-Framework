#ifndef  DUNGEONROOM_H 
#define  DUNGEONROOM_H  

#include "sprite.h"
#include "json.hpp" // Include the nlohmann/json header
#include <fstream>  // For file reading
#include <iostream>

using json = nlohmann::json;

class dungeonRoom
{
public:
	dungeonRoom();
	~dungeonRoom();

public:
	int nWidth;
	int nHeight;
	int roomID;
	Sprite* pSprite;

	int GetIndex(int x, int y);
	bool GetSolid(int x, int y);
	bool CreateFromJSON(const std::string& filename);

private:
	int *m_indices = nullptr;
	bool *m_solids = nullptr;
};

#endif //   DUNGEONROOM_H 