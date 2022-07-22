#include "stdafx.h"

Scene1::Scene1()
{
	
}

Scene1::~Scene1()
{
	
}

void Scene1::Init()
{
	//mapSize = Int2(App.GetWidth() * 0.2, App.GetHeight() * 0.2);
	mapSize = Int2(1000, 400);
	unsigned int seed = RANDOM->Int(200, 350);
	map = new Map(mapSize, seed);
	mapLight = new MapLight(mapSize);
	block = new ObTileMap();
	map->tileMap = block;
	wall = new ObTileMap();
	shadow = new ObTileMap();
	mapWall = new MapWall(mapSize);
	block->scale = Vector2(16.0f, 16.0f) * GAMESIZE;
	wall->scale = Vector2(16.0f, 16.0f) * GAMESIZE;
	shadow->scale = Vector2(16.0f, 16.0f) * GAMESIZE;
	bg = new BackGround(Vector2(mapSize.x * block->scale.x, mapSize.y * block->scale.y));
	block->SetWorldPos(Vector2(-(mapSize.x * block->scale.x) * 0.5, -(mapSize.y * block->scale.y) * 0.5));
	block->ResizeTile(mapSize);
	wall->SetWorldPos(Vector2(-(mapSize.x * wall->scale.x) * 0.5, -(mapSize.y * wall->scale.y) * 0.5));
	wall->ResizeTile(mapSize);
	shadow->SetWorldPos(Vector2(-(mapSize.x * wall->scale.x) * 0.5, -(mapSize.y * wall->scale.y) * 0.5));
	shadow->ResizeTile(mapSize);
	//// 배치
	for (int y = 0; y < mapSize.y; y++) {
		for (int x = 0; x < mapSize.x; x++) {
			switch (map->GetType(Int2(x, y)))
			{
			case AIR:
				block->SetTile(Int2(x, y), Int2(1, 1), AIR, TILE_NONE);
				break;
			case DIRT:
				block->SetTile(Int2(x, y), Int2(1, 1), DIRT, TILE_WALL);
				break;
			case ROCK:
				block->SetTile(Int2(x, y), Int2(1, 1), ROCK, TILE_WALL);
				break;
			default:
				break;
			}
			if (mapWall->isWall[y][x] == true) {
				wall->SetTile(Int2(x, y), Int2(1, 1), DIRT, TILE_NONE);
			}
			else {
				wall->SetTile(Int2(x, y), Int2(1, 1), AIR, TILE_WALL);
			}
			shadow->SetTile(Int2(x, y), Int2(1, 1), 6);
		}
	}
	mapLight->CalcLight(map, mapWall->isWall);
	// 블럭 디테일
	for (int y = 0; y < mapSize.y; y++) {
		for (int x = 0; x < mapSize.x; x++) {
			tb.TileArrangement(*block, Int2(x, y), map->GetType(Int2(x, y)), *map, mapLight->lightPower);
			if (y > mapSize.y * 0.64 && map->GetType(Int2(x, y)) == DIRT) {
				tb.DirtToGrass(*block, Int2(x, y));
			}
			// 그림자 계산
			if (mapLight->lightPower[y][x] == 0)
				mapLight->SpreadLight(Int2(x, y), block, shadow);
		}
	}
	block->UpdateSub();
	wall->UpdateSub();
	shadow->UpdateSub();

	block->Update();
	// 플레이어 배치
	player = new Player(block);
	player->map = map;
	player->mapLight = mapLight;
	player->shadow = shadow;
	player->isWall = mapWall->isWall;
	player->bodySprite->scale *= GAMESIZE;
	player->col->scale *= GAMESIZE;
	player->Spawn();

	player->col->Update();

	slime = new Slime(block);
	slime->playerCol = player->col;
	slime->bodySprite->scale *= GAMESIZE;
	slime->col->scale *= GAMESIZE;
	slime->Spawn(player->col->GetWorldPos());

	zombie = new Zombie(block);
	zombie->playerCol = player->col;
	zombie->bodySprite->scale *= GAMESIZE;
	zombie->col->scale *= GAMESIZE;
	zombie->Spawn(player->col->GetWorldPos());

	CAM->position = player->col->GetWorldPos();
}

void Scene1::Release()
{
	map->Release();
	SafeDelete(mapLight);
	SafeDelete(block);
	SafeDelete(wall);
	SafeDelete(shadow);
	SafeDelete(player);
	SafeDelete(slime);
	SafeDelete(zombie);
}

void Scene1::Update()
{
	// ImGui::Text("FPS: %d", TIMER->GetFramePerSecond());

	// 카메라 움직임
	if (INPUT->KeyDown('C')) camMod = !camMod;
	if (camMod) {
		Vector2 move = Vector2(0.0, 0.0);
		if (INPUT->KeyPress('W'))
			move.y += 1;
		if (INPUT->KeyPress('A'))
			move.x -= 1;
		if (INPUT->KeyPress('S'))
			move.y -= 1;
		if (INPUT->KeyPress('D'))
			move.x += 1;
		move.Normalize();
		CAM->position += move * DELTA * 500;
	}
	else {
		CAM->position = player->col->GetWorldPos();
	}
	// Hit Player
	if (!slime->isDead && slime->col->Intersect(player->col)) {
		player->Hit(slime->stat, slime->col->GetWorldPos());
	}
	if (!zombie->isDead && zombie->col->Intersect(player->col)) {
		player->Hit(zombie->stat, zombie->col->GetWorldPos());
	}
	// Hit Enemy
	if (player->colSword->Intersect(slime->col)) {
		slime->Hit(player->stat, player->col->GetWorldPos());
	}
	if (player->colSword->Intersect(zombie->col)) {
		zombie->Hit(player->stat, player->col->GetWorldPos());
	}

	bg->Update();
	wall->Update();
	block->Update();
	map->Update();
	if (!camMod) player->Update();
	slime->Update();
	zombie->Update();
	shadow->Update();
}

void Scene1::LateUpdate()
{
	
}

void Scene1::Render()
{
	bg->Render();
	wall->Render();
	block->Render();
	map->Render();
	player->Render();
	slime->Render();
	zombie->Render();
	shadow->Render();
}

void Scene1::ResizeScreen()
{
}
