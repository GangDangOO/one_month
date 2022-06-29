#include "stdafx.h"

Map::Map(Int2 MapSize, unsigned int Seed)
{
	tileSize = MapSize;
	PerlinNoise pn(Seed);
	type = new byte * [tileSize.y];
	for (int i = 0; i < tileSize.y; i++) {
		type[i] = new byte[tileSize.x];
	}
	for (int i = 0; i < tileSize.y; ++i) {
		for (int j = 0; j < tileSize.x; ++j) {
			if (i < tileSize.y * 0.66) {
				double x = (double)j / ((double)tileSize.x);
				double y = (double)i / ((double)tileSize.y);

				double n = pn.noise(x / 32, y / 32, 1.0);

				if (n > 0.5f) {
					type[i][j] = AIR;
				}
				else {
					type[i][j] = DIRT;
				}
			}
			else {
				if (i > tileSize.y * 0.68) {
					type[i][j] = AIR;
				}
				else {
					type[i][j] = DIRT;
				}
			}
		}
	}
	isRectVisible = false;
	CellularAutomata();
	CellularAutomata();
}

Map::~Map()
{
}

void Map::Release()
{
	for (int i = 0; i < tileSize.y; i++) {
		SafeDelete(type[i]);
	}
	SafeDelete(type);
}

void Map::Update()
{
	if (INPUT->KeyDown('Q')) {
		isRectVisible = !isRectVisible;
	}
}

void Map::LateUpdate()
{

}

void Map::Render()
{
	
}

void Map::CellularAutomata()
{
	for (int i = 1; i < tileSize.y - 1; ++i) {
		for (int j = 1; j < tileSize.x - 1; ++j) {
			if (type[i][j] == AIR) {
				int count = 0;
				if (type[i + 1][j - 1] != AIR) count++;
				if (type[i + 1][j] != AIR) count++;
				if (type[i + 1][j + 1] != AIR) count++;
				if (type[i][j - 1] != AIR) count++;
				if (type[i][j + 1] != AIR) count++;
				if (type[i - 1][j - 1] != AIR) count++;
				if (type[i - 1][j] != AIR) count++;
				if (type[i - 1][j + 1] != AIR) count++;
				if (count >= 5) {
					type[i][j] = DIRT;
				}
			}
		}
	}
}

bool Map::IsThisWall(Int2 pos)
{
	return type[pos.y][pos.x] == Type::DIRT;
}
