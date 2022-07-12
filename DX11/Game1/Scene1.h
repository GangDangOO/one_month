#pragma once
class Scene1 : public Scene
{
private:
	Int2 mapSize;
	Map* map;
	ObTileMap* block;
	ObTileMap* wall;
	MapWall* mapWall;
	MapLight* mapLight;
	BackGround* bg;
	Tilebuild tb;
	byte		addBlockType;
	Player* player;

	bool camMod;
public:
	Scene1();
	~Scene1();
	
	virtual void Init() override;
	virtual void Release() override; //����
	virtual void Update() override;
	virtual void LateUpdate() override;//����
	virtual void Render() override;
	virtual void ResizeScreen() override;
};


