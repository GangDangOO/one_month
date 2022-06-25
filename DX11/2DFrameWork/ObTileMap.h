#pragma once

class Tile
{
public:
    int         F, G, H;//타일 비용
    Int2        idx;    //타일 인덱스
    Tile*       P;      //나를 갱신시킨 타일
    bool        isFind; //검사한적이 있는가?
    int         state;
    Vector2     Pos;

    void ClearCost();         //비용 초기화
    void ClacH(Int2 DestIdx); //H계산해라
    void ClacF(); //F계산해라
};
//               타일주소,비교값
using PTile = pair<Tile*, int>;

struct compare
{
    //연산자 오버로딩
    bool operator()(PTile& a, PTile& b)
    {
        return a.second > b.second;
    }
};

enum TileState
{
    TILE_NONE,
    TILE_WALL
};

class ObTileMap : public GameObject
{
private:
    VertexTile*             vertices;
    ID3D11Buffer *          vertexBuffer;
    Int2                    tileSize;   //10x10 , 4x4
    vector<vector<Tile>>    Tiles;
public:
    ObImage *               tileImages[4];
    string                  file;

public:
    ObTileMap();
    ~ObTileMap();


    void        Render()override;
    void        SetTile(Int2 TileIdx, Int2 FrameIdx, int ImgIdx = 0 ,
        int TileState = TILE_NONE , Color color = Color(0.5f, 0.5f, 0.5f, 0.5f));
    int         GetTileState(Int2 TileIdx);
    Vector2     GetTilePosition(Int2 TileIdx);
    bool        WorldPosToTileIdx(Vector2 WPos, Int2 & TileIdx);
    void        ResizeTile(Int2 TileSize);
    void        Save();
    void        Load();
    Int2        GetTileSize() { return tileSize; };
    void        CreateTileCost();
    bool        PathFinding(Int2 sour, Int2 dest, OUT vector<Tile*>& way);
};

