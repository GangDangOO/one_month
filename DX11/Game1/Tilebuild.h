#pragma once
class Tilebuild
{
public:
	void TileRemove(ObTileMap& block, Int2 tilePos, Map& map);
	void TileAdd(ObTileMap& block, Int2 tilePos, Map& map, byte type);
	void TileArrangement(ObTileMap& block, Int2 pos, byte type, Map& map);
};
