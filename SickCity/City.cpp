#include "stdafx.h"
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <iostream>
#include "City.h"
#include "Tile.h"

// Used to move people around
double City::distributePool(double& pool, Tile& tile, double rate)
{
	const static int moveRate = 4;
	unsigned int maxPop = tile.maxPopPerLevel * (tile.tileVariant + 1);

	/* If there is room in the tile, move up to 4 people fro mthe pool into the zone */
	if (pool > 0)
	{
		int moving = maxPop - tile.population;
		if (moving > moveRate) moving = moveRate;
		if (pool - moving < 0) moving = pool;
		pool -= moving;
		tile.population += moving;
	}

	// Adjust tile population for births and deaths
	tile.population += tile.population * rate;

	// Move population that cannot be sutained by the tile back into the pool
	if (tile.population > maxPop)
	{
		pool += tile.population - maxPop;
		tile.population = maxPop;
	}
	return tile.population;
}

void City::bulldoze(const Tile& tile)
{
	/* Replace the selected tiles on the map with the tile and
	* update populations etc accordingly */
	for (int pos = 0; pos < this->map.width * this->map.height; ++pos)
	{
		if (this->map.selected[pos] == 1)
		{
			if (this->map.tiles[pos].tileType == TileType::RESIDENTIAL)
			{
				this->populationPool += this->map.tiles[pos].population;
			}
			else if (this->map.tiles[pos].tileType == TileType::COMMERCIAL)
			{
				this->employmentPool += this->map.tiles[pos].population;
			}
			else if (this->map.tiles[pos].tileType == TileType::INDUSTRIAL)
			{
				this->employmentPool += this->map.tiles[pos].population;
			}
			this->map.tiles[pos] = tile;
		}
	}
	this->map.setTilePositions();
	return;
}

void City::shuffleTiles()
{
	this->shuffledTiles.clear();
	while (this->shuffledTiles.size() < this->map.tiles.size())
	{
		this->shuffledTiles.push_back(0);
	}
	std::iota(shuffledTiles.begin(), shuffledTiles.end(), 0);
	std::random_shuffle(shuffledTiles.begin(), shuffledTiles.end());

	return;
}

void City::tileChanged()
{
	this->map.updateDirection(TileType::ROAD);
	this->map.updateDirection(TileType::BRIDGE);
	this->map.findConnectedRegions(
	{
		TileType::ROAD, TileType::RESIDENTIAL,
		TileType::COMMERCIAL, TileType::INDUSTRIAL, TileType::BRIDGE
	}, 0);

	return;
}

void City::load(std::string cityName, std::map<std::string, Tile>& tileAtlas)
{
	int width = 0;
	int height = 0;

	std::ifstream inputFile(cityName + "_cfg.dat", std::ios::in);

	std::string line;

	while (std::getline(inputFile, line))
	{
		std::istringstream lineStream(line);
		std::string key;
		if (std::getline(lineStream, key, '='))
		{
			std::string value;
			if (std::getline(lineStream, value))
			{
				if (key == "width")                  width = std::stoi(value);
				else if (key == "height")            height = std::stoi(value);
				else if (key == "day")               this->day = std::stoi(value);
				else if (key == "populationPool")    this->populationPool = std::stod(value);
				else if (key == "employmentPool")    this->employmentPool = std::stod(value);
				else if (key == "population")        this->population = std::stod(value);
				else if (key == "employable")        this->employable = std::stod(value);
				else if (key == "birthRate")         this->birthRate = std::stod(value);
				else if (key == "deathRate")         this->deathRate = std::stod(value);
				else if (key == "residentialTax")    this->residentialTax = std::stod(value);
				else if (key == "commercialTax")     this->commercialTax = std::stod(value);
				else if (key == "industrialTax")     this->industrialTax = std::stod(value);
				else if (key == "funds")             this->funds = std::stod(value);
				else if (key == "earnings")          this->earnings = std::stod(value);
			}
			else
			{
				std::cerr << "Error, no value for key " << key << std::endl;
			}
		}
	}

	inputFile.close();

	this->map.load(cityName + "_map.dat", width, height, tileAtlas);
	tileChanged();
	std::cout << "Loaded Savegame\nDay: " << std::to_string(this->day) << "\nMap: " << std::to_string(width) << ", " << std::to_string(height);
	return;
}

void City::save(std::string cityName)
{
	std::ofstream outputFile(cityName + "_cfg.dat", std::ios::out);

	outputFile << "width=" << this->map.width << std::endl;
	outputFile << "height=" << this->map.height << std::endl;
	outputFile << "day=" << this->day << std::endl;
	outputFile << "populationPool=" << this->populationPool << std::endl;
	outputFile << "employmentPool=" << this->employmentPool << std::endl;
	outputFile << "population=" << this->population << std::endl;
	outputFile << "employable=" << this->employable << std::endl;
	outputFile << "birthRate=" << this->birthRate << std::endl;
	outputFile << "deathRate=" << this->deathRate << std::endl;
	outputFile << "residentialTax=" << this->residentialTax << std::endl;
	outputFile << "commercialTax=" << this->commercialTax << std::endl;
	outputFile << "industrialTax=" << this->industrialTax << std::endl;
	outputFile << "funds=" << this->funds << std::endl;
	outputFile << "earnings=" << this->earnings << std::endl;

	outputFile.close();

	this->map.save(cityName + "_map.dat");

	return;
}

void City::update(float dt)
{
	double popTotal = 0;
	double commercialRevenue = 0;
	double industrialRevenue = 0;

	/* Update the game time */
	this->currentTime += dt;
	if (this->currentTime < this->timePerDay) return;

	/* New Day */
	sf::Clock timer;
	timer.restart();
	int numTiles[3] = { 0, 0, 0 }; // res, com, ind

	++day;
	this->currentTime = 0.0;
	if (day % 30 == 0) 	// End of month
	{
		this->funds += this->earnings;
		std::cout << "Padyday! earned " << this->earnings << " at day " << this->day << std::endl;
		this->earnings = 0;
	}

	/* Run first pass of tile updates. Mostly handles pool distributions.
	First, people attempt to move from the populationPool into the residential zones, 
	and the population of each zone is adjusted according to the net birth rate.
	The commercial zones then attempt to hire people,
	where they will hire more people the lower the commercialTax is.
	Finally the industrial zones will attempt to hire people too,
	but will also extract resources from the ground if any are left.
	(Remember the resources variable in Map?)
	We then call update on the tile to change its tileVariant if necessary
	(i.e. the population is full).
	*/
	for (int i = 0; i < this->map.tiles.size(); i++)
	{
		Tile& tile = this->map.tiles[this->shuffledTiles[i]];

		//if (tile.tileType == TileType::GRASS || tile.tileType == TileType::WATER)
		//	continue;

		if (tile.tileType == TileType::RESIDENTIAL) {
			// Redistribute the pool and increase the population total by the tile's population
			this->distributePool(this->populationPool, tile, this->birthRate - this->deathRate);
			popTotal += tile.population;
			numTiles[0]++;
		}
		else if (tile.tileType == TileType::COMMERCIAL) {
			// Hire people
			if (rand() % 100 < 15 * (1.0 - this->commercialTax))
				this->distributePool(this->employmentPool, tile, 0.00);
			numTiles[1]++;

			// Handle goods distribution
			int receivedGoods = 0;
			double maxCustomers = 0.0;

			for (auto& tileIndex : this->map.zones[tile.regions[0]]) {
				Tile& tile2 = this->map.tiles[tileIndex];
				if (tile2.tileType == TileType::INDUSTRIAL && tile2.storedGoods > 0) {
					while (tile2.storedGoods > 0 && receivedGoods != tile.tileVariant + 1) {
						--tile2.storedGoods;
						++receivedGoods;
						industrialRevenue += 100 * (1.0 - industrialTax);
					}
				}
				else if (tile2.tileType == TileType::RESIDENTIAL) {
					maxCustomers += tile2.population;
				}
				if (receivedGoods == tile.tileVariant + 1) break;
			}

			// Calculate overall revenue for the tile
			tile.production = (receivedGoods*100.0 + rand() % 20) * (1.0 - this->commercialTax);
			double revenue = tile.production * maxCustomers * tile.population / 100.0;
			commercialRevenue += revenue;

			// Emit dollar particle at tile
			//int x = (i / this->map.height);
			//int y = (i % this->map.width);
			if (revenue > 0) {
				std::cout << "revenue: " << revenue << std::endl;
				//int clamped = std::max(0.1f, std::min((float)revenue, 2.0f));
				int scale = 1 + (revenue / 100 * 0.5f);
				this->game->emitParticle(0, tile.sprite.getPosition(), scale);
			}
		}
		else if (tile.tileType == TileType::INDUSTRIAL) {
			// Extract resources from the ground
			// If population is > 100, 100% success chance.
			// chance should increase by how many people working at tile?
			if (this->map.resources[i] > 0 && rand() % 100 < this->population) {
				++tile.production;
				--this->map.resources[i];
			}
			// Hire people
			if (rand() % 100 < 15 * (1.0 - this->industrialTax))
				this->distributePool(this->employmentPool, tile, 0.0);
			numTiles[2]++;

			int receivedResources = 0;

			// Extract resources form the ground
			if (tile.regions[0] != 0) {
				// lookup all tiles in same zone as tile
				for (auto& tileIndex : this->map.zones[tile.regions[0]]) {
					Tile& tileInZone = this->map.tiles[tileIndex];
					if (tileInZone.tileType == TileType::INDUSTRIAL && tileInZone.production > 0) {
						++receivedResources;
						--tileInZone.production;
					}
					if (receivedResources >= tile.tileVariant + 1) break;
				}
			}

			// Turn resources into goods
			tile.storedGoods += (receivedResources + tile.production)*(tile.tileVariant + 1);
		}
		else if (tile.tileType == TileType::FIRE) {
			// Spread Fire ! Pyroman 4 Life

			// 20% chance to spread in random direction
			if (rand() % 5 == 0) {
				int index = this->shuffledTiles[i];
				int dir = rand() % 4;
				// spread upwards
				if(dir == 0 && index > this->map.height) { index -= this->map.height; }
				// spread right
				if (dir == 1 && index < this->map.tiles.size()+1) { index++; }
				// spread left
				if (dir == 2 && index > 1) { index--; }
				// spread down
				if (dir == 3 && index < (this->map.tiles.size()-this->map.height)) { index += this->map.height; }

				if(index != this->shuffledTiles[i] && this->map.tiles[index].tileType != TileType::FIRE && this->map.tiles[index].tileType != TileType::WATER)
				this->map.tiles[index] = this->game->tileAtlas.at("fire");
			}
		}

		tile.update();			
	}

	/* Adjust population pool for births and deaths */
	this->populationPool += this->populationPool * (this->birthRate - this->deathRate);
	popTotal += this->populationPool;

	/* Adjust the employment pool for the changing population. */
	float newWorkers = (popTotal - this->population) * this->propCanWork;
	newWorkers *= newWorkers < 0 ? -1 : 1;
	this->employmentPool += newWorkers;
	this->employable += newWorkers;
	if (this->employmentPool < 0) this->employmentPool = 0;
	if (this->employable < 0) this->employable = 0;

	/* Update the city population. */
	this->population = popTotal;

	/* Calculate city income from tax. */
	this->earnings = (this->population - this->populationPool) * 15 * this->residentialTax;
	this->earnings += commercialRevenue * this->commercialTax;
	this->earnings += industrialRevenue * this->industrialTax;

	float timeSpent = timer.getElapsedTime().asMilliseconds();
	std::cout << "Day " << day << " done in " << timeSpent << "ms (" << numTiles[0] << "R, " << numTiles[1] << "C, " << numTiles[2] << "I)" <<std::endl;
	return;
}