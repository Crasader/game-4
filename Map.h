#ifndef ProjectH_Map_H
#define ProjectH_Map_H

#include <vector>
#include <list>

#include "Point.h"
#include "tile.h"

namespace ProjectH
{

class Map
{
public:
	// Ÿ���� �׸��� ���� ��� �����͸� ��� ����ü
	struct DrawData
	{
		int mapType;
		int imgIndex;
		int tileType;
		int tileFormat;
		int tileDrawX;
		int tileDrawY;
		int tileElevation;
		int startPlayer;
		int playerTyep;
		int startObjectType;
		bool startPoint;
		float indexY;
		std::list< int >* doodad;
	};

	struct StartPlayer
	{
		Point point;
		int type;
	};

public:
	/**************************************************************************
		�� ������ ���� �ϴ� �⺻ ������, �Ҹ��� ( �޸� �Ҵ��� ���� �ѹ��� �Ѵ�. )
	**************************************************************************/
	Map( int tileWidthCount, int tileHeightCount, int mapType = 0, int tileWidthPixel = 64, int tileHeightPixel = 32,
				int mapBgTopPixel = 0, int mapBgBottomPixel = 0, int mapBgLeftPixel = 0, int mapBgRightPixel = 0 );
	~Map() { Release(); }

	/**************************************************************************
		�� ������ �ʱ�ȭ ������ �ϴ� �޼���
	**************************************************************************/
	// ���� ����� ( ������ �ʱ�ȭ )
	void Reset( int tileWidthCount, int tileHeightCount, int mapType = 0, int tileWidthPixel = 64, int tileHeightPixel = 32,
				int mapBgTopPixel = 0, int mapBgBottomPixel = 0, int mapBgLeftPixel = 0, int mapBgRightPixel = 0 );
	// ������ �б�
	bool Load( char* c );
	// ������ ����
	bool Save( char* c );
	// ������ ����
	void Release();

	/**************************************************************************
		���� ���� ������ �� ����������� �ʿ� ���� ����
	**************************************************************************/
	// �� ������ ����
	void InitMap( int tileWidthCount, int tileHeightCount, int mapType = 0, int tileWidthPixel = 64, int tileHeightPixel = 32,
				int mapBgTopPixel = 0, int mapBgBottomPixel = 0, int mapBgLeftPixel = 0, int mapBgRightPixel = 0 );
	// Ÿ�� ���� ����
	void InitTileRelation();
	
	/**************************************************************************
		�� �����͸� ��� ���� �޼���
	**************************************************************************/
	// �� ����ũ�� ���
	int GetWidth() { return mapWidthPixel; }
	// �� ���� ũ�� ���
	int GetHeight() { return mapHeightPixel; }
	// Ÿ�� ���ΰ��� ���
	int GetTileWidthCount() { return tileWidthCount; }
	// Ÿ�� ���ΰ��� ���
	int GetTileHeightCount() { return tileHeightCount; }
	// �� �ȼ� ũ�� ���
	void GetMapSize( Point& point ) { point.SetPoint( mapWidthPixel, mapHeightPixel ); }
	// ���� ����Ʈ ���
	std::vector< StartPlayer >& GetStartList() { return startPlayer; }

	/**************************************************************************
		Ÿ�� ������ ���ٿ� ���� �޼���
	**************************************************************************/
	// Tile �� �����͸� ����ִ� Vector �� �ε����� Ÿ���� �����Ϳ� �����Ѵ�.
	const std::vector< ProjectH::Tile* >& operator[] ( int y ) { return map[y]; }
	// Ŭ���̾�Ʈ�� ���콺 ��ǥ�� �ش��ϴ� Ÿ���� �����Ѵ�.
	bool SetIndex( int mouseX, int mouseY );
	// SetIndex �� ���õ� Ÿ���� Ÿ���� �ٲ۴�.
	void SetTile( int imgIndex );
	// SetIndex �� ���õ� Ÿ���� Ÿ�԰� �ֺ�Ÿ���� Ÿ���� �ٲ۴�.
	void AutoTiling( int type );
	// SetIndex �� ���õ� Ÿ���� ���̸� �ø���.
	void TileElevationUp();
	// SetIndex �� ���õ� Ÿ���� ���̸� ������.
	void TileElevationDown();
	// SetIndex �� ���õ� Ÿ�Ͽ� ��Ĺ��� �߰��Ѵ�.
	void AddDoodad( int doodad );
	// SetIndex �� ���õ� Ÿ���� ��Ĺ��� �ʱ�ȭ�Ѵ�.
	void ClearDoodad();
	// SetIndex �� ���õ� Ÿ���� ĳ���� ������������ �����Ѵ�.
	void SetStartPoint( int player, int type );

	/**************************************************************************
	*	Ŭ���̾�Ʈ�� �׸��� ���� �� �����͸� ��� ���� �޼���
	**************************************************************************/
	// �׸� ������ �����Ѵ�. ( ���� Ŭ���̾�Ʈ�� �������� ��ǥ�� NextTile �� ������ �����Ѵ�. )
	void SetDrawRect( int left, int top, int right, int bottom );
	// �׸� �����͸� ���´�. ( SetDrawRect �� ������ ������ Ÿ���� �����ϰ� �� �����͸� ���´�. )
	bool NextTile( DrawData& drawData );

	/**************************************************************************
		�� ��ü�� �׸��� ���� �� �����͸� ��� ���� �޼���
	**************************************************************************/
	// �׸� Ÿ���� �����͸� ���´�.
	void GetDrawData( int x, int y, DrawData& drawData );
	// ���콺 �̹����� �׸� ��ǥ�� ���´�.
	void GetDrawMousePoint( Point& pt );

private:
	/**************************************************************************
		AutoTiling() �� ���� �޼��� ( direction = 0:��, 1:��, 2:��, 3:�� )
	**************************************************************************/
	// Ÿ�� ������ �ε��� x, y �� direction ���⿡ �ִ� Ÿ���� �����͸� �����Ѵ�.
	Tile* GetAroundTile( int direction, int x, int y );
	// Ÿ�� ������ �ε��� x, y �� direction ���⿡ �ִ� Ÿ���� �ε����� ��� �����͸� �����Ѵ�.
	Tile* GetAroundTileIndex( int direction, int x, int y, Point& pt );
	// direction �� ������ ������ �ش��ϴ� ��ȣ�� �����Ѵ�.
	int ReverseDirection( int direction ) { return ( ( direction + 2 ) % 4 ); }

	/**************************************************************************
		Map Class ���� ������ ���� �޼���
	**************************************************************************/
	// �� �ε��� ��ȿ�� �˻�
	bool CheckIndex( Point& pt )
	{ return ( ( pt.x >= 0 && pt.y >= 0 && pt.x < tileWidthCount && pt.y < tileHeightCount ) ? true : false ); }

private:
	int mapType;				// �� ����

	int tileWidthCount;			// Ÿ�� ���� ����
	int tileHeightCount;		// Ÿ�� ���� ����

	int mapWidthPixel;			// �� ���� ũ��
	int mapHeightPixel;			// �� ���� ũ��

	int mapBgTopPixel;			// ��� ��� �ȼ� ũ��
	int mapBgLeftPixel;			// ���� ��� �ȼ� ũ��
	int mapBgBottomPixel;		// �ϴ� ��� �ȼ� ũ��
	int mapBgRightPixel;		// ���� ��� �ȼ� ũ��

	int tileWidthPixel;			// Ÿ�� ���� ũ��(�ȼ�)
	int tileHeightPixel;		// Ÿ�� ���� ũ��(�ȼ�)

	int criterionX;				// �� ������X (Ŭ���̾�Ʈ ��ǥ�� �� �ε����� ��ȯ�ϱ� ����)
	int criterionY;				// �� ������Y (Ŭ���̾�Ʈ ��ǥ�� �� �ε����� ��ȯ�ϱ� ����)

	Point index;				// ���콺( SetIndex() )�� ������ Ÿ�� �ε��� ��ȣ

	Point drawLT;				// SetDrawRect() �� ������ ���� ��� �ε���
	Point drawRB;				// SetDrawRect() �� ������ ���� �ϴ� �ε���
	Point drawIndex;			// NextTile() �� ���Ǵ� drawLT, drawRB ������ �ε���

	std::vector< StartPlayer > startPlayer;	// ��� ������Ʈ ���� ������( ��ġ, Ÿ�� )
	std::vector< Point > startPointList;	// ĳ���� �������� ����Ʈ
	std::vector< Point > startPointMoster;	// ���� �������� ����Ʈ
	std::vector< std::vector< ProjectH::Tile* > > map;	// �� ����Ÿ

	int l;
	int t;
};

} /* ProjectH namespace */

#endif /* ProjectH_Map_H */