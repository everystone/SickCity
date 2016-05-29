#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <noise\noise.h>
#include "Map.h"
#include "Tile.h"


/* MicroPather */
// https://github.com/leethomason/MicroPather/blob/master/dungeon.cpp

void Map::findPath(Tile& origin, Tile& destination)
{
	float totalCost;
	//int result = pather->Solve(XYToTile(from.x, from.y), XYToNode(to.x, to.y), &path, &totalCost);
	int result = pather->Solve(&origin, &destination, &path, &totalCost);
	if (result == micropather::MicroPather::SOLVED) {
		std::cout << "Found path: ";
	}
	else {
		std::cout << "no path: ";
	}
}

int Map::Passable(int x, int y)
{
	if (x >= 0 && x < width && y >= 0 && y < height) {
		Tile tile = this->tiles[width*y + x];
		switch (tile.tileType) {
		case TileType::GRASS:
		case TileType::FOREST:
			return 1;
			break;

		default:
			return 2;
			break;
		}
	}

	
}

void Map::TileToXY(void* tile, int* x, int* y) {
	Tile* tileRef = (Tile*)tile;

	float sx = tileRef->sprite.getPosition().x;
	float sy = tileRef->sprite.getPosition().y;
	std::cout << "sprite pos " << sx << ", " << sy << std::endl;
	*x = sy / (this->tileSize) + sx / (2 * this->tileSize) - this->width * 0.5;;
	*y = sy / (this->tileSize) - sx / (2 * this->tileSize) + this->width * 0.5;
}
Tile* Map::XYToTile(int x, int y)
{
	return &tiles[y*height + x];
}

float Map::LeastCostEstimate(void * nodeStart, void * nodeEnd)
{
	int xStart, yStart, xEnd, yEnd;
	TileToXY(nodeStart, &xStart, &yStart);
	TileToXY(nodeEnd, &xEnd, &yEnd);

	/* Compute the minimum path cost using distance measurement. It is possible
	to compute the exact minimum path using the fact that you can move only
	on a straight line or on a diagonal, and this will yield a better result.
	*/
	int dx = xStart - xEnd;
	int dy = yStart - yEnd;
	return (float)sqrt((double)(dx*dx) + (double)(dy*dy));
}

void Map::AdjacentCost(void * node, MP_VECTOR<micropather::StateCost>* neighbors)
{
	int x, y;
	const int dx[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
	const int dy[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
	const float cost[8] = { 1.0f, 1.41f, 1.0f, 1.41f, 1.0f, 1.41f, 1.0f, 1.41f };

	TileToXY(node, &x, &y);

	for (int i = 0; i<8; ++i) {
		int nx = x + dx[i];
		int ny = y + dy[i];

		int pass = Passable(nx, ny);
		if (pass > 0) {
			if (pass == 1)
			{
				// Normal floor
				micropather::StateCost nodeCost = { XYToTile(nx, ny), cost[i] };
				neighbors->push_back(nodeCost);
			}
			else
			{
				// Obstacle
				micropather::StateCost nodeCost = { XYToTile(nx, ny), FLT_MAX };
				neighbors->push_back(nodeCost);
			}
		}
	}
}

void Map::PrintStateInfo(void * state)
{
}


/* Load map from disk */
void Map::load(const std::string& filename, unsigned int width, unsigned int height,
    std::map<std::string, Tile>& tileAtlas)
{
    std::ifstream inputFile;
    inputFile.open(filename, std::ios::in | std::ios::binary);

    this->width = width;
    this->height = height;

    for(int pos = 0; pos < this->width * this->height; ++pos)
    {
        this->resources.push_back(255);
		this->selected.push_back(0);

        TileType tileType;
        inputFile.read((char*)&tileType, sizeof(int));
        switch(tileType)
        {
            default:
            case TileType::VOID:
            case TileType::GRASS:
                this->tiles.push_back(tileAtlas.at("grass"));
                break;
            case TileType::FOREST:
                this->tiles.push_back(tileAtlas.at("forest"));
                break;
            case TileType::WATER:
                this->tiles.push_back(tileAtlas.at("water"));
                break;
            case TileType::RESIDENTIAL:
                this->tiles.push_back(tileAtlas.at("residential"));
                break;
            case TileType::COMMERCIAL:
                this->tiles.push_back(tileAtlas.at("commercial"));
                break;
            case TileType::INDUSTRIAL:
                this->tiles.push_back(tileAtlas.at("industrial"));
                break;
            case TileType::ROAD:
                this->tiles.push_back(tileAtlas.at("road"));
                break;
        }
        Tile& tile = this->tiles.back();
        inputFile.read((char*)&tile.tileVariant, sizeof(int));
        inputFile.read((char*)&tile.regions, sizeof(int)*1);
        inputFile.read((char*)&tile.population, sizeof(double));
        inputFile.read((char*)&tile.storedGoods, sizeof(float));
    }

    inputFile.close();
	setTilePositions();
    return;
}

void Map::save(const std::string& filename)
{
  std::ofstream outputFile;
  outputFile.open(filename, std::ios::out | std::ios::binary);
  
    for(auto tile : this->tiles)
    {
        outputFile.write((char*)&tile.tileType, sizeof(int));
        outputFile.write((char*)&tile.tileVariant, sizeof(int));
        outputFile.write((char*)&tile.regions, sizeof(int)*1);
        outputFile.write((char*)&tile.population, sizeof(double));
        outputFile.write((char*)&tile.storedGoods, sizeof(float));
    }

    outputFile.close();

    return;
  
}
void Map::generate(int width, int height, std::map<std::string, Tile>& tileAtlas)
{
	noise::module::Perlin myModule;

	this->tiles.clear();
	this->width = width;
	this->height = height;
	float seed = 0.1f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3.0f - 0.1f)));
	for (float y = 0; y < this->height; ++y)
	{
		for (float x = 0; x < this->width; ++x)
		{
			this->resources.push_back(255);
			this->selected.push_back(0);
			double value = myModule.GetValue(x/16, y/16, seed); // x/16, y/16, 0.55 = waterworld
			
			//std::cout << value << std::endl;
			if(value > 0 && value < 0.7) this->tiles.push_back(tileAtlas.at("grass"));
			else if(value < 0) this->tiles.push_back(tileAtlas.at("water"));
			else this->tiles.push_back(tileAtlas.at("forest"));
		}
	}
	setTilePositions();
}

void Map::setTilePositions()
{
	// moved positioning logic from map.draw, to here.
	for (int y = 0; y < this->height; ++y)
	{
		for (int x = 0; x < this->width; ++x)
		{
			/* Set the position of the tile in the 2d world */
			sf::Vector2f pos;
			// Bend the outer tiles downwards, to create the look of a 3d Cube. ( Is it even possible? )
			if (x > this->width - 5 || y > this->height - 5) {
				this->tiles[y*this->width + x].sprite.setScale(1, 2);
				//this->tiles[y*this->width + x].sprite.setRotation(90);
				//int edge = (x - this->width)
				pos.x = (x - y) * this->tileSize + this->width * this->tileSize;
				pos.y = (x + y - 1) * this->tileSize * 0.5;
			}
			else {
				// normal
				pos.x = (x - y) * this->tileSize + this->width * this->tileSize;
				pos.y = (x + y) * this->tileSize * 0.5;
			}
			this->tiles[y*this->width + x].sprite.setPosition(pos);
		}
	}
}

void Map::draw(sf::RenderWindow& window, float dt)
{
    for(int y = 0; y < this->height; ++y)
    {
        for(int x = 0; x < this->width; ++x)
        {
			/* Change color if tile is selected */
			if (this->selected[y*this->width + x])
				this->tiles[y*this->width + x].sprite.setColor(sf::Color(0x7d, 0x7d, 0x7d));
			else
				this->tiles[y*this->width + x].sprite.setColor(sf::Color(0xff, 0xff, 0xff));

			/* Change color if tile is hovered */
			if (y*this->width + x == this->hovered)
				this->tiles[y*this->width + x].sprite.setColor(sf::Color(40, 40, 40));

            /* Draw the tile */
            this->tiles[y*this->width+x].draw(window, dt);
        }
    }
    return;
}

void Map::updateDirection(TileType tileType)
{
    for(int y = 0; y < this->height; ++y)
    {
        for(int x = 0; x < this->width; ++x)
        {
            int pos = y*this->width+x;

            if(this->tiles[pos].tileType != tileType) continue;

            bool adjacentTiles[3][3] = {{0,0,0},{0,0,0},{0,0,0}};

            /* Check for adjacent tiles of the same type */
            if(x > 0 && y > 0)
                adjacentTiles[0][0] = (this->tiles[(y-1)*this->width+(x-1)].tileType == tileType);
            if(y > 0)
                adjacentTiles[0][1] = (this->tiles[(y-1)*this->width+(x  )].tileType == tileType);
            if(x < this->width-1 && y > 0)
                adjacentTiles[0][2] = (this->tiles[(y-1)*this->width+(x+1)].tileType == tileType);
            if(x > 0)
                adjacentTiles[1][0] = (this->tiles[(y  )*this->width+(x-1)].tileType == tileType);
            if(x < width-1)
                adjacentTiles[1][2] = (this->tiles[(y  )*this->width+(x+1)].tileType == tileType);
            if(x > 0 && y < this->height-1)
                adjacentTiles[2][0] = (this->tiles[(y+1)*this->width+(x-1)].tileType == tileType);
            if(y < this->height-1)
                adjacentTiles[2][1] = (this->tiles[(y+1)*this->width+(x  )].tileType == tileType);
            if(x < this->width-1 && y < this->height-1)
                adjacentTiles[2][2] = (this->tiles[(y+1)*this->width+(x+1)].tileType == tileType);

            /* Change the tile variant depending on the tile position */
            if(adjacentTiles[1][0] && adjacentTiles[1][2] && adjacentTiles[0][1] && adjacentTiles[2][1])
                this->tiles[pos].tileVariant = 2;
            else if(adjacentTiles[1][0] && adjacentTiles[1][2] && adjacentTiles[0][1])
                this->tiles[pos].tileVariant = 7;
            else if(adjacentTiles[1][0] && adjacentTiles[1][2] && adjacentTiles[2][1])
                this->tiles[pos].tileVariant = 8;
            else if(adjacentTiles[0][1] && adjacentTiles[2][1] && adjacentTiles[1][0])
                this->tiles[pos].tileVariant = 9;
            else if(adjacentTiles[0][1] && adjacentTiles[2][1] && adjacentTiles[1][2])
                this->tiles[pos].tileVariant = 10;
            else if(adjacentTiles[1][0] && adjacentTiles[1][2])
                this->tiles[pos].tileVariant = 0;
            else if(adjacentTiles[0][1] && adjacentTiles[2][1])
                this->tiles[pos].tileVariant = 1;
            else if(adjacentTiles[2][1] && adjacentTiles[1][0])
                this->tiles[pos].tileVariant = 3;
            else if(adjacentTiles[0][1] && adjacentTiles[1][2])
                this->tiles[pos].tileVariant = 4;
            else if(adjacentTiles[1][0] && adjacentTiles[0][1])
                this->tiles[pos].tileVariant = 5;
            else if(adjacentTiles[2][1] && adjacentTiles[1][2])
                this->tiles[pos].tileVariant = 6;
            else if(adjacentTiles[1][0])
                this->tiles[pos].tileVariant = 0;
            else if(adjacentTiles[1][2])
                this->tiles[pos].tileVariant = 0;
            else if(adjacentTiles[0][1])    
                this->tiles[pos].tileVariant = 1;
            else if(adjacentTiles[2][1])
                this->tiles[pos].tileVariant = 1;
        }
    }

    return;
}


void Map::depthfirstsearch(std::vector<TileType>& whitelist,
    sf::Vector2i pos, int label, int regionType=0)
{
    if(pos.x < 0 || pos.x >= this->width) return;
    if(pos.y < 0 || pos.y >= this->height) return;
    if(this->tiles[pos.y*this->width+pos.x].regions[regionType] != 0) return;
    bool found = false;
    for(auto type : whitelist)
    {
        if(type == this->tiles[pos.y*this->width+pos.x].tileType)
        {
            found = true;
            break;
        }
    }
    if(!found) return;

    this->tiles[pos.y*this->width+pos.x].regions[regionType] = label;
	
	// add this tileIndex to zonemap, with regionId ( label ) as key
	zones[label].push_back(pos.y*this->width + pos.x);
	

    depthfirstsearch(whitelist, pos + sf::Vector2i(-1,  0), label, regionType);
    depthfirstsearch(whitelist, pos + sf::Vector2i(0 ,  1), label, regionType);
    depthfirstsearch(whitelist, pos + sf::Vector2i(1 ,  0), label, regionType);
    depthfirstsearch(whitelist, pos + sf::Vector2i(0 , -1), label, regionType);

    return;
}

void Map::select(sf::Vector2i start, sf::Vector2i end, std::vector<TileType> blacklist)
{
	// Swap coordinates if necessary
	// if we started from low to high
	if (end.y < start.y) std::swap(start.y, end.y);
	if (end.x < start.x) std::swap(start.x, end.x);

	/* Clamp in range */
	if (end.x >= this->width)      end.x = this->width - 1;
	else if (end.x < 0)               end.x = 0;
	if (end.y >= this->height)         end.y = this->height - 1;
	else if (end.y < 0)               end.y = 0;
	if (start.x >= this->width)        start.x = this->width - 1;
	else if (start.x < 0)             start.x = 0;
	if (start.y >= this->height)  start.y = this->height - 1;
	else if (start.y < 0)             start.y = 0;

	for (int y = start.y; y <= end.y; ++y)
	{
		for (int x = start.x; x <= end.x; ++x)
		{
			/* Check if the tile type is in the blacklist. If it is, mark it as
			* invalid, otherwise select it */
			this->selected[y*this->width + x] = 1;
			++this->numSelected;
			for (auto type : blacklist)
			{
				if (this->tiles[y*this->width + x].tileType == type)
				{
					this->selected[y*this->width + x] = 2;
					--this->numSelected;
					break;
				}
			}
		}
	}
}

/* Clear Tile Selection */
void Map::clearSelected()
{
	this->numSelected = 0;
	for (auto& tile : this->selected) tile = 0;
}


// RegionType is currently 0 for everything.
// 
void Map::findConnectedRegions(std::vector<TileType> whitelist, int regionType=0)
{
    int regions = 1;
	zones.clear(); // do I need to manually delete the Vector values?

    for(auto& tile : this->tiles) tile.regions[regionType] = 0;

    for(int y = 0; y < this->height; ++y)
    {
        for(int x = 0; x < this->width; ++x)
        {
            bool found = false;
            for(auto type : whitelist)
            {
                if(type == this->tiles[y*this->width+x].tileType)
                {
                    found = true;
                    break;
                }
            }
            if(this->tiles[y*this->width+x].regions[regionType] == 0 && found)
            {
                depthfirstsearch(whitelist, sf::Vector2i(x, y), regions++, regionType);
            }
        }
    }
    this->numRegions[regionType] = regions;
}