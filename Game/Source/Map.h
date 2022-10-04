#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"
#include "List.h"
#include "Point.h"

#include "PugiXml\src\pugixml.hpp"

// L04: TODO 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
struct TileSet
{
	SString	name;
	int	firstgid;
	int margin;
	int	spacing;
	int	tile_width;
	int	tile_height;

	SDL_Texture* texture;

};

// L04: TODO 1: Create a struct needed to hold the information to Map node
struct MapData
{
	int width;
	int	height;
	int	tileWidth;
	int	tileHeight;
	List<TileSet*> tilesets;
};

class Map : public Module
{
public:

    Map();

    // Destructor
    virtual ~Map();

    // Called before render is available
    bool Awake(pugi::xml_node& conf);

    // Called each loop iteration
    void Draw();

    // Called before quitting
    bool CleanUp();

    // Load new map
    bool Load();

private:

	bool LoadMap(pugi::xml_node mapFile);

	// L04: TODO 4: Create and call a private function to load a tileset
	bool LoadTileSet(pugi::xml_node mapFile);

public: 

	// L04: TODO 1: Declare a variable data of the struct MapData
	MapData mapData;

private:

    SString mapFileName;
	SString mapFolder;

    bool mapLoaded;
};

#endif // __MAP_H__