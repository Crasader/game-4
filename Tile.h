#ifndef ProjectH_Tile_H
#define ProjectH_Tile_H

#include <vector>
#include <list>

#include "Point.h"

namespace ProjectH
{

class Tile
{
friend class Tile;
friend class Map;

public:
	/**************************************************************************
		�׸��⸦ ���� ����ü
	**************************************************************************/
	struct TileDrawData
	{
		int imgIndex;
		int tileType;
		int drawX;
		int drawY;
		int elevationGrade;
		int startPlayer;
		int playerType;
		bool startPoint;
		std::list< int >* doodad;
	};

	/**************************************************************************
		������ ���� ����ü
	**************************************************************************/
	struct TileData
	{
		int tileType;
		int imgIndex;
		int elevationPixel;
		int elevationGrade;

		int coordinateX;
		int coordinateY;
		int drawCoordinateX;
		int drawCoordinateY;
		
		int playerType;
		int startPlayer;
		bool startPoint;

		bool availableMove;

		bool linkUp;
		bool linkRight;
		bool linkDown;
		bool linkLeft;

		std::list< int >* doodad;
	};

public:
	/**************************************************************************
		Ÿ�� ������ �⺻ ������
	**************************************************************************/
	Tile( int elevation, int x, int y ) :
		elevationPixel( elevation ), startPoint( false ), startPlayer( 0 ), playerType( 0 ), imgIndex( 0 ),
		tileType( 0 ), elevationGrade( 0 ), availableMove( true ),
		linkUp( false ), linkRight( false ), linkDown( false ), linkLeft( false ),
		coordinate( x, y ), drawCoordinate( x, y ), isTransform( false ) {}
	
	~Tile() {}

	// ������ �ʱ�ȭ
	void Reset( int elevationPixel );

	/**************************************************************************
		Ÿ�� �����͸� ��� ���� �޼���
	**************************************************************************/
	// Ÿ��Ÿ�� ���
	int GetType() { return tileType; }
	// Ÿ������ ���
	int GetTypeFormat();
	// �̹��� �ε��� ���
	int GetImgIndex() { return imgIndex; }
	// ���̴ܰ� ������
	int GetElevationGrade() { return elevationGrade; }
	// �⺻ �ȼ���ǥ ���
	const Point& GetCoordinate() { return coordinate; }
	// �׷��� �ȼ���ǥ ���
	const Point& GetDrawCoordinate();
	// ������ ������ ���
	void GetData( TileData& data );
	// �׸��� ���� ������ ���
	void GetDrawData( TileDrawData& data );

	/**************************************************************************
		Ÿ�� ������ ������ ���� �޼���
	**************************************************************************/
	// �̹��� �ε��� ����
	void SetImgIndex( int index ) { imgIndex = index; }
	// ���� �ø���
	void ElevationUp();	
	// ���� ���߱�
	void ElevationDown();
	// Ÿ�� ���� ���� ( �ֺ�Ÿ�� ��ũ�� ���� �ʴ´�. )
	void SetLinkType( int type ) { tileType = type; }
	// Ÿ�� ���� ���� ( �ֺ�Ÿ�� ��ũ�� ���´�. )
	void SetUnLinkType( int type );
	// ��Ĺ� �߰�
	void AddDoodad( int thing );
	// ��Ĺ� �ʱ�ȭ
	void ClearDoodad();
	// �̵� ���� ����
	void AvailableMove( bool b ) { availableMove == true ? availableMove = b : availableMove = availableMove; }
	// �������� ����
	void SetStartPoint( bool b ) { startPoint = b; }
	// �������� ���� ���
	bool GetStartPoint() { return startPoint; }
	// ���� ĳ���� ����
	void SetStartPlayer( int player ) { startPlayer = player; }
	// ���� �÷��̾� ��ȣ ���
	int GetStartPlayer() { return startPlayer; }
	// ĳ���� ���� ����
	void SetPlayerType( int type ) { playerType = type; }	
	// Ÿ�� ������ ����
	void SetData( TileData &data );

private:
	/**************************************************************************
		Friend Map; �� ���� �޼���
	**************************************************************************/
	// �ֺ�Ÿ�� �߰�
	void AddAroundTile( Tile* tile ) { aroundTile.push_back( tile ); }
	// ����Ÿ�ϸ��� ���� �޼���
	void Link( int direction, bool link );				// Ÿ�� ����
	void CheckSelf();									// �������� �ϴ��� üũ
	bool CheckEraseableSelf();							// �������� �ϴ��� üũ

	/**************************************************************************
		Tile Class ���� ������ ���� �޼���
	**************************************************************************/
	// �׷��� �ȼ���ǥ ����
	void SetDrawCoordinate();

private:
	bool isTransform;
	int imgIndex;					// Ÿ�� �̹��� �ε���
	int tileType;					// Ÿ�� ����
	int elevationGrade;				// ���� �ܰ�

	int elevationPixel;				// Ÿ�� ���� �� (�ȼ� ����)

	bool availableMove;				// �̵� ���� ����

	bool linkUp;					// ��
	bool linkRight;					// ��
	bool linkDown;					// ��
	bool linkLeft;					// ��

	int playerType;					// ĳ���� ����
	int startPlayer;				// ���� ĳ���� ��ȣ
	bool startPoint;				// ĳ���� ���� ��ġ����

	Point coordinate;				// ���� ��ǥ (�ȼ� ����)
	Point drawCoordinate;			// �׷��� ��ǥ (�ȼ� ����)

	std::list< int > doodad;		// ��Ĺ� ����Ʈ

	std::vector< Tile* > aroundTile;// �ֺ�Ÿ�� �֢٢עء���� �������

	static const int indexTable[4][6];	// ���� Ÿ�ϸ��� ���� ������
};

} /* ProjectH NameSpace */

#endif /* ProjectH_Tile_H */