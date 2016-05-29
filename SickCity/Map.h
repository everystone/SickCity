#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include "Tile.h"
#include <unordered_map>
#include "micropather\micropather.h"


class Map : public ::micropather::Graph
{
  private:

    void depthfirstsearch(std::vector<TileType>& whitelist,
        sf::Vector2i pos, int label, int type);
	micropather::MicroPather* pather;
	micropather::MPVector<void*> path;
    public:

    unsigned int width;
    unsigned int height;
    std::vector<Tile> tiles;
	// Tile Selection
	std::vector<int> selected;
	unsigned int hovered;
	std::unordered_map<char, std::vector<std::size_t>> zones; // region data.
	//std::vector<std::vector<std::size_t>> zones;
	void select(sf::Vector2i start, sf::Vector2i end, std::vector<TileType> blacklist);
	void clearSelected();

    /* Resource map */
    std::vector<int> resources;

    unsigned int tileSize;
    unsigned int numSelected;
    unsigned int numRegions[1];

    /* Load map from disk */
    void load(const std::string& filename, unsigned int width, unsigned int height,
        std::map<std::string, Tile>& tileAtlas);

    /* Save map to disk */
    void save(const std::string& filename);

    /* Draw the map */  
    void draw(sf::RenderWindow& window, float dt);

	/* Generate the map */
	void generate(int width, int height, std::map<std::string, Tile>& tileAtlas);

	/* Set sprite positions */
	void setTilePositions();

    /* Checks if one position in the map is connected to another by
     * only traversing tiles in the whitelist */
    void findConnectedRegions(std::vector<TileType> whitelist, int type);

    /* Update the direction of directional tiles so that they face the correct
     * way. Used to orient roads, pylons, rivers etc */
    void updateDirection(TileType tileType);

    /* Blank map constructor */
    Map()
    {
        this->tileSize = 16;
        this->width = 0;
        this->height = 0;
        this->numRegions[0] = 1;
		this->numSelected = 0;
		this->hovered = 0;
		srand(static_cast <unsigned> (time(0))); // seed random generator
		pather = new micropather::MicroPather(this); //64x64
    }
    /* Load map from file constructor */
    Map(const std::string& filename, unsigned int width, unsigned int height,
        std::map<std::string, Tile>& tileAtlas) : pather(0)
    {
		this->numSelected = 0;
		this->hovered = 0;
        this->tileSize = 16;
        load(filename, width, height, tileAtlas);
		pather = new micropather::MicroPather(this, 4096); //64x64
		//@todo: delete pather in destructor
    }

	// Pathfinding related
	void findPath(Tile& origin, Tile& destination);
	int Passable(int x, int y);
	Tile * XYToTile(int x, int y);
	void TileToXY(void* tile, int* x, int* y);
	/**
	Return the least possible cost between 2 states. For example, if your pathfinding
	is based on distance, this is simply the straight distance between 2 points on the
	map. If you pathfinding is based on minimum time, it is the minimal travel time
	between 2 points given the best possible terrain.
	*/
	virtual float LeastCostEstimate(void* stateStart, void* stateEnd);

	/**
	Return the exact cost from the given state to all its neighboring states. This
	may be called multiple times, or cached by the solver. It *must* return the same
	exact values for every call to MicroPather::Solve(). It should generally be a simple,
	fast function with no callbacks into the pather.
	*/
	virtual void AdjacentCost(void* state, MP_VECTOR< micropather::StateCost > *adjacent);

	/**
	This function is only used in DEBUG mode - it dumps output to stdout. Since void*
	aren't really human readable, normally you print out some concise info (like "(1,2)")
	without an ending newline.
	*/
	virtual void  PrintStateInfo(void* state);
};