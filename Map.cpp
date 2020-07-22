#include "stdafx.h"
#include "Map.h"
#include "ImgResource.h"

#include <locale.h>
#include <math.h>
#include <fstream>
#include <iterator>

#define CALC_XY(pt, xCoord, yCoord, width, height, criterionX, criterionY, heightCnt)	\
{																						\
	pt.x = static_cast<int>((((xCoord + yCoord * 2) - criterionX)) / width);	        \
	pt.y = static_cast<int>(((criterionY - (yCoord - xCoord / 2))) / height);			\
	pt.y = static_cast<int>(heightCnt / 2 - 1 - (pt.y - pt.x));							\
	pt.x = static_cast<int>(pt.x - ceil((float)pt.y / 2));								\
}


namespace ProjectH
{

using std::vector;
using std::ifstream;
using std::ofstream;
using std::iterator;
using std::endl;
using ProjectH::Point;
using ProjectH::Tile;

void Map::TileElevationUp()
{
	map[ index.y ][ index.x ]->ElevationUp();

	int imgIndex = IMGRESOURCE()->ImageIndex( map[ index.y ][ index.x ]->GetType(), map[ index.y ][ index.x ]->GetTypeFormat() );
	map[ index.y ][ index.x ]->SetImgIndex( imgIndex );

	Tile* at;
	for ( int i = 0; i < 4; ++i )
	{
		if ( at = GetAroundTile( i, index.x, index.y ) )
		{
			imgIndex = IMGRESOURCE()->ImageIndex( at->GetType(), at->GetTypeFormat() );
			at->SetImgIndex( imgIndex );
		}
	}
}

void Map::TileElevationDown()
{
	map[ index.y ][ index.x ]->ElevationDown();

	int imgIndex = IMGRESOURCE()->ImageIndex( map[ index.y ][ index.x ]->GetType(), map[ index.y ][ index.x ]->GetTypeFormat() );
	map[ index.y ][ index.x ]->SetImgIndex( imgIndex );

	Tile* at;
	for ( int i = 0; i < 4; ++i )
	{
		if ( at = GetAroundTile( i, index.x, index.y ) )
		{
			imgIndex = IMGRESOURCE()->ImageIndex( at->GetType(), at->GetTypeFormat() );
			at->SetImgIndex( imgIndex );
		}
	}
}

bool Map::SetIndex( int mouseX, int mouseY )
{
	Point pt;
	CALC_XY( pt, mouseX, mouseY, tileWidthPixel, tileHeightPixel, criterionX, criterionY, tileHeightCount );

	// �ε��� ����ó��
	if ( pt.x >= 0 && pt.x < tileWidthCount && pt.y >= 0 && pt.y < tileHeightCount )
	{
		// ���콺 ��ǥ�� �ٲ��� �ʾ����� false ����
		if ( pt.x == index.x && pt.y == index.y )
			return false;

		index.x = pt.x;
		index.y = pt.y;

		return true;
	}
	// ���콺 ��ǥ�� �߸����Ǿ����� false ����
	return false;
}

void Map::SetTile( int imgIndex )
{
	//map[ index.y ][ index.x ]->SetUnLinkType( imgIndex );
	for ( int i = 0; i < 4; ++i )
	{
		Tile *at = NULL;
		if ( !( at = GetAroundTile(i, index.x, index.y) ) )
		{
			map[index.y][index.x]->Link( i, ( at ? true : false ) );
			at->Link( ReverseDirection(i), ( map[index.y][index.x] ? true : false ) );
		}
	}
	map[ index.y ][ index.x ]->SetImgIndex( imgIndex );
}

void Map::AutoTiling( int type )
{
	// Ÿ�ϵ��� Ÿ���� �����Ѵ�. (���� Ÿ�ϰ� �ֺ�Ÿ�� 8��)
	map[ index.y ][ index.x ]->SetLinkType( type );

	for ( int i = 0; i < 8; ++i )
	{
		Tile* at = NULL;
		if ( !( at = GetAroundTile( i, index.x, index.y ) ) )
			continue;

		if ( i < 4 )
		{
			at->Link( ReverseDirection(i), ( map[ index.y ][ index.x ] ? true : false ) );
			map[ index.y ][ index.x ]->Link( i, ( at ? true : false ) );
		}

		at->SetLinkType( type );
	}

	// Ÿ�ϵ��� ��ũ�� �����Ѵ�. ( ���� Ÿ�ϰ� �ֺ�Ÿ�� 8���� ���� �ֺ�Ÿ�� 4�� )
	for ( int i = 0; i < 8; ++i )
	{
		Tile* at = NULL;
		Point ai;
		if ( !( at = GetAroundTileIndex( i, index.x, index.y, ai ) ) )
			continue;

		for ( int j = 0; j < 4; ++j )
		{
			Tile* at2 = NULL;
			if ( !( at2 = GetAroundTile( j, ai.x, ai.y ) ) )
				continue;

			if ( at->GetType() == at2->GetType() )
			{
				at->Link( j, ( at2 ? true : false ) );
				at2->Link( ReverseDirection(j), ( at ? true : false ) );
			}
			else
			{
				at->Link( j, 0 );
				at2->Link( ReverseDirection(j), 0 );
			}
		}
	}

	// Ÿ�ϵ��� Ÿ�Ժ��� �׸��� �ִ��� üũ�Ѵ�. (���� Ÿ�ϰ� �ֺ�Ÿ�� 8���� ���� �ֺ�Ÿ�� 4��)
	for ( int i = 0; i < 8; ++i )
	{
		Point ai;
		if ( !GetAroundTileIndex( i, index.x, index.y, ai ) )
			continue;

		for ( int j = 0; j < 4; ++j )
		{
			Tile* at;
			if ( !( at = GetAroundTile( j, ai.x, ai.y ) ) )
				continue;

			at->CheckSelf();
		}
	}

	// ������ Ÿ�ϵ��� Ÿ��Ÿ�Կ� ���� Ÿ�����˿� ���� �̹��� �ε����� �����Ѵ�. (���� Ÿ�ϰ� �ֺ�Ÿ�� 8���� ���� �ֺ�Ÿ�� 4��)
	for ( int i = 0; i < 8; ++i )
	{
		Point ai;
		if ( !GetAroundTileIndex( i, index.x, index.y, ai ) )
			continue;

		for ( int j = 0; j < 4; ++j )
		{
			Tile* at;
			if ( !( at = GetAroundTile( j, ai.x, ai.y ) ) )
				continue;

			int imgIndex = IMGRESOURCE()->ImageIndex( at->GetType(), at->GetTypeFormat() );
			at->SetImgIndex( imgIndex );
		}

		int imgIndex = IMGRESOURCE()->ImageIndex( map[ ai.y ][ ai.x ]->GetType(), map[ ai.y ][ ai.x ]->GetTypeFormat() );
		map[ ai.y ][ ai.x ]->SetImgIndex( imgIndex );
	}

	int imgIndex = IMGRESOURCE()->ImageIndex( map[ index.y ][ index.x ]->GetType(), map[ index.y ][ index.x ]->GetTypeFormat() );
	map[ index.y ][ index.x ]->SetImgIndex( imgIndex );

} /* void Map::SetTileType( int type ) */

void Map::SetDrawRect( int left, int top, int right, int bottom )
{
	l = left;
	t = top;

	CALC_XY( drawLT, left, top, tileWidthPixel, tileHeightPixel, criterionX, criterionY, tileHeightCount );
	CALC_XY( drawRB, right, bottom, tileWidthPixel, tileHeightPixel, criterionX, criterionY, tileHeightCount );

	if ( drawLT.x >= 0 && drawLT.x < tileWidthCount && drawLT.y >= 0 && drawLT.y < tileHeightCount &&
		drawRB.x >= 0 && drawRB.x <= tileWidthCount && drawRB.y >= 0 && drawRB.y <= tileHeightCount )
	{
		drawLT.x = ( drawLT.x -1 < 0 ? drawLT.x : drawLT.x -1 );
		drawLT.y = ( drawLT.y -1 < 0 ? drawLT.y : drawLT.y -1 );

		drawRB.x = ( drawRB.x +1 > tileWidthCount ? drawRB.x : drawRB.x +1 );
		drawRB.y = ( drawRB.y +1 > tileHeightCount ? drawRB.y : drawRB.y +1 );
		drawRB.x = ( drawRB.x +1 > tileWidthCount ? drawRB.x : drawRB.x +1 );
		drawRB.y = ( drawRB.y +1 > tileHeightCount ? drawRB.y : drawRB.y +1 );

		drawIndex = drawLT;
	}
}

bool Map::NextTile( DrawData& drawData )
{
	if ( drawIndex.y == drawRB.y )
		return false;

	Tile::TileDrawData tileData;
	map[ drawIndex.y ][ drawIndex.x ]->GetDrawData( tileData );

	drawData.mapType = mapType;
	drawData.imgIndex = tileData.imgIndex;
	drawData.tileType = tileData.tileType;
	drawData.tileDrawX = tileData.drawX - l;
	drawData.tileDrawY = tileData.drawY - t;
	drawData.tileElevation = tileData.elevationGrade;
	drawData.startPlayer = tileData.startPlayer;
	drawData.startPoint = tileData.startPoint;
	drawData.playerTyep = tileData.playerType;
	drawData.doodad = tileData.doodad;
	drawData.indexY = ( (float) drawIndex.y ) * 0.000f;
	
	if ( drawIndex.x == drawRB.x -1 )
	{
		drawIndex.y++;
		drawIndex.x = drawLT.x;
	}
	else
	{
		drawIndex.x++;
	}

	return true;
}

void Map::GetDrawMousePoint( Point& pt )
{
	pt = map[ index.y ][ index.x ]->GetDrawCoordinate();
	pt.x -= l;
	pt.y -= t;
}

Tile* Map::GetAroundTile( int direction, int x, int y )
{
	Point pt;

	switch( direction )
	{
	case 0:		// ��
	{
		pt.x = ( y %2 == 1 )	?	x +1	:	x;
		pt.y = y -1;
		break;
	}
	case 1:		// ��
	{
		pt.x = ( y %2 == 1 )	?	x +1	:	x;
		pt.y = y +1;
		break;
	}
	case 2:		// ��
	{
		pt.x = ( y %2 == 0 )	?	x -1	:	x;
		pt.y = y +1;
		break;
	}
	case 3:		// ��
	{
		pt.x = ( y %2 == 0 )	?	x -1	:	x;
		pt.y = y -1;
		break;
	}
	case 4:		// ��
	{
		pt.x = x +1;
		pt.y = y;
		break;
	}
	case 5:		// ��
	{
		pt.x = x;
		pt.y = y +2;
		break;
	}
	case 6:		// ��
	{
		pt.x = x -1;
		pt.y = y;
		break;
	}
	case 7:		// ��
	{
		pt.x = x;
		pt.y = y -2;
		break;
	}
	} /* switch( direction ) */

	if ( CheckIndex( pt ) )
		return map[ pt.y ][ pt.x ];
	else
		return 0;
}

Tile* Map::GetAroundTileIndex( int direction, int x, int y, Point& pt )
{
	switch( direction )
	{
	case 0:		// ��
	{
		pt.x = ( y %2 == 1 )	?	x +1	:	x;
		pt.y = y -1;
		break;
	}
	case 1:		// ��
	{
		pt.x = ( y %2 == 1 )	?	x +1	:	x;
		pt.y = y +1;
		break;
	}
	case 2:		// ��
	{
		pt.x = ( y %2 == 0 )	?	x -1	:	x;
		pt.y = y +1;
		break;
	}
	case 3:		// ��
	{
		pt.x = ( y %2 == 0 )	?	x -1	:	x;
		pt.y = y -1;
		break;
	}
	case 4:		// ��
	{
		pt.x = x +1;
		pt.y = y;
		break;
	}
	case 5:		// ��
	{
		pt.x = x;
		pt.y = y +2;
		break;
	}
	case 6:		// ��
	{
		pt.x = x -1;
		pt.y = y;
		break;
	}
	case 7:		// ��
	{
		pt.x = x;
		pt.y = y -2;
		break;
	}
	} /* switch( direction ) */

	if ( CheckIndex( pt ) )
		return map[ pt.y ][ pt.x ];
	else
		return 0;
}

void Map::GetDrawData( int x, int y, DrawData& drawData )
{
	Tile::TileDrawData tileData;
	map[ y ][ x ]->GetDrawData( tileData );

	drawData.mapType = mapType;
	drawData.imgIndex = tileData.imgIndex;
	drawData.tileType = tileData.tileType;
	drawData.tileDrawX = tileData.drawX;
	drawData.tileDrawY = tileData.drawY;
	drawData.tileElevation = tileData.elevationGrade;
	drawData.startPlayer = tileData.startPlayer;
	drawData.startPoint = tileData.startPoint;
	drawData.playerTyep = tileData.playerType;
	drawData.doodad = tileData.doodad;
	drawData.indexY = ( (float) drawIndex.y ) * 0.000f;
}

void Map::SetStartPoint( int player, int type )
{
	// ���� �������� �϶�
	if ( player == -1 )
	{
		// ���� Ÿ�Ͽ��� �����ϴ� �÷��̾ ���Ͱ� �ִ��� Ȯ��
		if ( map[ index.y ][ index.x ]->GetStartPoint() )
		{
			int p = map[ index.y ][ index.x ]->GetStartPlayer();
			// ���� Ÿ�Ͽ��� ���Ͱ� �����ϸ� ���� ������ġ ����Ʈ���� �ش� ��ġ�� �����Ѵ�.
			if ( p == -1 )
			{
				std::vector< Point >::iterator iter;
				for ( iter = startPointMoster.begin(); iter != startPointMoster.end(); ++iter )
				{
					if ( (*iter) == index )
					{
						startPointMoster.erase( iter );
						break;
					}
				}
			}
			// ���� Ÿ�Ͽ��� ĳ���Ͱ� �����ϸ� ĳ���� ������ġ ����Ʈ���� �ش� �÷��̾��� ĳ������ ��ġ�� �ʱ�ȭ�Ѵ�.
			else
			{
				startPointList[ p ] = 0;
			}
		}
		// ���� ��ǥ�� ���� ������ġ ����Ʈ�� �����Ѵ�.
		startPointMoster.push_back( index );
	}
	// �÷��̾� ĳ���� �������� �϶�
	else
	{
		// ���� ���õ� ĳ������ ���� ������ġ Ÿ���� ���� ������ �ʱ�ȭ�Ѵ�.
		map[ startPointList[ player ].y ][ startPointList[ player ].x ]->SetStartPlayer( 0 );
		map[ startPointList[ player ].y ][ startPointList[ player ].x ]->SetStartPoint( false );
		
		// ���� ��ǥ�� ĳ���� ������ġ ����Ʈ�� �����Ѵ�.
		startPointList[ player ] = index;		
	}

	// ���� ��ġ�� ��Ĺ��� �ʱ�ȭ�Ѵ�.
	map[ index.y ][ index.x ]->ClearDoodad();

	// ���� ��ǥ�� ĳ���� ������ġ�� �����Ѵ�.
	map[ index.y ][ index.x ]->SetStartPlayer( player );
	map[ index.y ][ index.x ]->SetStartPoint( true );
	map[ index.y ][ index.x ]->SetPlayerType( type );
}

void Map::AddDoodad( int doodad )
{
	Tile::TileData tileData;
	map[ index.y ][ index.x ]->GetData( tileData );
	int lastDoodad = ( tileData.doodad->empty() == true ? 0 : tileData.doodad->back() );

	if ( lastDoodad && !( IMGRESOURCE()->DoodadAvailable( lastDoodad, doodad ) ) )
		map[ index.y ][ index.x ]->ClearDoodad();

	map[ index.y ][ index.x ]->AddDoodad( doodad );

	if ( IMGRESOURCE()->IsMajorTile( map[ index.y ][ index.x ]->GetImgIndex() ) )
	{
		int type = map[ index.y ][ index.x ]->GetType();
		int firstDoodad = ( tileData.doodad->empty() == true ? 0 : tileData.doodad->back() );

		if ( firstDoodad )
		{
			int imgIndex = IMGRESOURCE()->GetShadowIndex( type, firstDoodad );

			if ( imgIndex )
				map[ index.y ][ index.x ]->SetImgIndex( imgIndex );
		}
	}

	// �̵� ���� �� ����
	map[ index.y ][ index.x ]->AvailableMove( IMGRESOURCE()->IsMoveable( doodad ) );
}

Map::Map( int _tileWidthCount, int _tileHeightCount, int _mapType /*= 0*/, int _tileWidthPixel /*= 64*/, int _tileHeightPixel /*= 32*/, int _mapBgTopPixel /*= 0*/, int _mapBgBottomPixel /*= 0*/, int _mapBgLeftPixel /*= 0*/, int _mapBgRightPixel /*= 0 */ )
{
	// �� ������ ����
	InitMap( _tileWidthCount, _tileHeightCount, _mapType, _tileWidthPixel, _tileHeightPixel, _mapBgTopPixel, _mapBgBottomPixel, _mapBgLeftPixel, _mapBgRightPixel );

	// Ÿ�� ����, ���� ������ ������� �� ���͸� �ִ�ũ���� 128 �� ������¡�ϰ� Ÿ�� ����Ÿ ����
	map.resize( 128 );
	for ( int y = 0; y < static_cast< int >( map.size() ); ++y )
	{
		map[y].resize( 128 );

		for ( int x = 0; x < static_cast< int >( map[y].size() ); ++x )
		{
			// Ÿ���� �׷��� �ȼ� ��ġ ���
			int drawX =	x * tileWidthPixel	+	( ( y %2 ) ? ( tileWidthPixel /2 ) : 0 ) - ( tileWidthPixel /2 ) + mapBgLeftPixel;
			int drawY = y * ( tileHeightPixel /2 ) - ( tileHeightPixel /2 ) + mapBgTopPixel;

			// Ÿ�� ������ ����
			map[y][x] = new Tile( tileHeightPixel/2, drawX, drawY );
		}
	}

	// ĳ���� ���� ��ġ ���� ������¡
	startPointList.resize( 9 );

	// Ÿ�� ���� ����
	InitTileRelation();
}

void Map::InitMap( int _tileWidthCount, int _tileHeightCount, int _mapType /*= 0*/, int _tileWidthPixel /*= 64*/, int _tileHeightPixel /*= 32*/, int _mapBgTopPixel /*= 0*/, int _mapBgBottomPixel /*= 0*/, int _mapBgLeftPixel /*= 0*/, int _mapBgRightPixel /*= 0 */ )
{
	// �� Ÿ�� ����
	mapType			= _mapType;

	// ���� ��� �ȼ� ũ�� ����
	mapBgTopPixel	= _mapBgTopPixel;
	mapBgLeftPixel	= _mapBgLeftPixel;
	mapBgBottomPixel= _mapBgBottomPixel;
	mapBgRightPixel	= _mapBgRightPixel;

	// Ÿ���� ���� ���� �ȼ�ũ�� ����
	tileWidthPixel	= _tileWidthPixel;
	tileHeightPixel	= _tileHeightPixel;

	// Ÿ���� ���� ���� ������ 0���� ������ 128�� ����
	tileWidthCount	= ( _tileWidthCount < 0 ? 128 : _tileWidthCount );
	tileHeightCount	= ( _tileHeightCount < 0 ? 128 : _tileHeightCount );

	// Ÿ���� ���� ���� ������ 128���� ũ�� 128�� ����
	tileWidthCount	= ( _tileWidthCount > 128 ? 128 : _tileWidthCount );
	tileHeightCount	= ( _tileHeightCount > 128 ? 128 : _tileHeightCount );

	// Ÿ���� ���� ���� ������ ¦���� �ƴϸ� ¦���� ����
	tileWidthCount	= ( _tileWidthCount %2 == 0 ? _tileWidthCount : _tileWidthCount -1 );
	tileHeightCount	= ( _tileHeightCount %2 == 0 ? _tileHeightCount : _tileHeightCount -1 );

	// �� �ȼ� ũ�� ����
	mapWidthPixel	= ( tileWidthPixel * tileWidthCount ) - tileWidthPixel / 2 + mapBgLeftPixel + mapBgRightPixel;
	mapHeightPixel	= ( tileHeightPixel * tileHeightCount / 2 ) - tileHeightPixel / 2 + mapBgTopPixel + mapBgBottomPixel;

	// ���� ���콺��ǥ���� ��ȯ�� ���� �߽ɰ� ����
	criterionX		= ( mapBgLeftPixel - tileWidthPixel / 2 ) + mapBgTopPixel * 2;
	criterionY		= ( mapHeightPixel - mapBgBottomPixel ) - mapBgLeftPixel / 2;
}

void Map::Reset( int _tileWidthCount, int _tileHeightCount, int _mapType /*= 0*/, int _tileWidthPixel /*= 64*/, int _tileHeightPixel /*= 32*/, int _mapBgTopPixel /*= 0*/, int _mapBgBottomPixel /*= 0*/, int _mapBgLeftPixel /*= 0*/, int _mapBgRightPixel /*= 0 */ )
{
	// �� ������ ����
	InitMap( _tileWidthCount, _tileHeightCount, _mapType, _tileWidthPixel, _tileHeightPixel, _mapBgTopPixel, _mapBgBottomPixel, _mapBgLeftPixel, _mapBgRightPixel );

	// Ÿ�� ������ �ʱ�ȭ
	for ( int y = 0; y < static_cast< int >( map.size() ); ++y )
		for ( int x = 0; x < static_cast< int >( map[y].size() ); ++ x )
			map[y][x]->Reset( _tileHeightPixel/2 );

	// ĳ���� ���� ��ġ �ʱ�ȭ
	for ( int y = 0; y != static_cast< int >( startPointList.size() ); ++y )
		startPointList[y] = 0;

	startPointMoster.clear();
}

void Map::Release()
{
	// Ÿ�� ������ �޸� ����
	for ( int y = 0; y != static_cast<int>( map.size() ); ++y )
	{
		for ( int x = 0; x != static_cast<int>( map[y].size() ); ++x )
			delete map[y][x];

		map[y].clear();
		map[y].resize( 0 );
	}

	map.clear();
	map.resize( 0 );
}

void Map::InitTileRelation()
{
	for ( int y = 0; y != tileHeightCount; ++y )
	{
		for ( int x = 0; x != tileWidthCount; ++x )
		{
			// �ֺ�Ÿ�� ���� 8����ŭ ���鼭
			for ( int i = 0; i != 8; ++i )
			{
				Tile* tile = GetAroundTile( i, x, y );		// �ֺ�Ÿ���� �����͸� ���´�.

				map[y][x]->AddAroundTile( tile );			// �ֺ�Ÿ�� ������ ����
			}
		}
	}
}

bool Map::Load( char* c )
{
	// �� �ѱ���� �����ϰ� ������ ����.
	setlocale( LC_ALL, "Korean" );
	ifstream f( c, std::ios::in );
	if ( !f.is_open() ) return false;

	std::istream_iterator<int> iit( f );
	std::istream_iterator<int> eos;

	// �� ������ �ε�
	int mapType = *iit++;
	int mapBgLeftPixel = *iit++;
	int mapBgRightPixel = *iit++;
	int mapBgTopPixel = *iit++;
	int mapBgBottomPixel = *iit++;
	int tileWidthCount = *iit++;
	int tileHeightCount = *iit++;
	int tileWidthPixel = *iit++;
	int tileHeightPixel = *iit++;

	// �� ������ �ʱ�ȭ
	Reset( tileWidthCount, tileHeightCount, mapType, tileWidthPixel, tileHeightPixel, mapBgTopPixel, mapBgBottomPixel, mapBgLeftPixel, mapBgRightPixel );

	// ĳ���� �������� ������ �ε�
	int iMax = *iit++;
	for ( int i = 0; i != iMax; ++i )
	{
		startPointList[i].x = *iit++;
		startPointList[i].y = *iit++;
	}

	// ���� �������� ������ �ε�
	iMax = *iit++;
	startPointMoster.resize( iMax );
	for ( int i = 0; i != iMax; ++i )
	{
		startPointMoster[i].x = *iit++;
		startPointMoster[i].y = *iit++;
	}

	Tile::TileData tileData;
	for ( int y = 0; y != tileHeightCount; ++y )
	{
		// Ÿ�� ������ ����
		for ( int x = 0; x != tileWidthCount; ++x )
		{
			tileData.imgIndex = *iit++;
			tileData.tileType = *iit++;
			tileData.coordinateX = *iit++;
			tileData.coordinateY = *iit++;
			tileData.drawCoordinateX = *iit++;
			tileData.drawCoordinateY = *iit++;
			tileData.availableMove = (*iit++) ? true : false;
			tileData.elevationPixel = *iit++;
			tileData.elevationGrade = *iit++;
			tileData.startPoint = (*iit++) ? true : false;
			tileData.startPlayer = (*iit++);
			tileData.playerType = (*iit++);
			tileData.linkUp = (*iit++) ? true : false;
			tileData.linkRight = (*iit++) ? true : false;
			tileData.linkDown = (*iit++) ? true : false;
			tileData.linkLeft = (*iit++) ? true : false;

			if ( tileData.startPoint )
			{
				StartPlayer s;
				s.point.x = tileData.drawCoordinateX;
				s.point.y = tileData.drawCoordinateY;
				s.type = tileData.playerType;

				startPlayer.push_back( s );
			}

			map[y][x]->SetData( tileData );

			int doodadSize = *iit++;
			for ( int i = 0; i < doodadSize; ++i )
				map[y][x]->AddDoodad( *iit++ );
		}
	}

	// ���� ���� ������ Ÿ�Ͽ� ����
	for ( int i = 0; i != startPointList.size(); ++i )
	{
		if ( startPointList[i].x > 1 && startPointList[i].y > 1 )
		{
			map[ startPointList[i].y ][ startPointList[i].x ]->SetStartPoint( true );
			map[ startPointList[i].y ][ startPointList[i].x ]->SetStartPlayer( i );
		}
	}
	f.close();

	return true;
}

bool Map::Save( char* c )
{
	// �� ������ ���
	setlocale( LC_ALL, "Korean" );
	ofstream f( c, std::ios::out );
	if ( !f.is_open() )	return false;

	f << mapType << endl;
	f << mapBgLeftPixel << endl;
	f << mapBgRightPixel << endl;
	f << mapBgTopPixel << endl;
	f << mapBgBottomPixel << endl;
	f << tileWidthCount << endl;
	f << tileHeightCount << endl;
	f << tileWidthPixel << endl;
	f << tileHeightPixel << endl;
	
	// ĳ���� ���� ���� ������ ����
	f << startPointList.size() << endl;
	for ( int i = 0; i != startPointList.size(); ++i )
		f << startPointList[i].x << "\t" << startPointList[i].y << endl;

	// ���� ���� ���� ������ ����
	f << startPointMoster.size() << endl;
	for ( int i = 0; i != startPointMoster.size(); ++i )
		f << startPointMoster[i].x << "\t" << startPointMoster[i].y << endl;

	Tile::TileData tileData;
	for( int y = 0; y < tileHeightCount; ++y )
	{
		for ( int x = 0; x < tileWidthCount; ++x )
		{
			// ������ Ÿ�� ������ ������
			map[y][x]->GetData( tileData );

			// Ÿ�� ������ ���
			f	<< tileData.imgIndex << "\t"
				<< tileData.tileType << "\t"
				<< tileData.coordinateX << "\t"
				<< tileData.coordinateY << "\t"
				<< tileData.drawCoordinateX << "\t"
				<< tileData.drawCoordinateY << "\t"
				<< tileData.availableMove << "\t"
				<< tileData.elevationPixel << "\t"
				<< tileData.elevationGrade << "\t" 
				<< tileData.startPoint << "\t"
				<< tileData.startPlayer << "\t"
				<< tileData.playerType << "\t"
				<< tileData.linkUp << "\t"
				<< tileData.linkRight << "\t"
				<< tileData.linkDown << "\t"
				<< tileData.linkLeft << "\t"
				<< tileData.doodad->size() << "\t";

			for ( std::list< int >::iterator iter = tileData.doodad->begin(); iter != tileData.doodad->end(); ++iter )
				f << (*iter) << "\t";
			
			f << endl;
		}
	}

	f.close();

	return true;
}

void Map::ClearDoodad()
{
	map[ index.y ][ index.x ]->ClearDoodad();

	int imgIndex = IMGRESOURCE()->ImageIndex( map[ index.y ][ index.x ]->GetType(), map[ index.y ][ index.x ]->GetTypeFormat() );
	map[ index.y ][ index.x ]->SetImgIndex( imgIndex );

}
} /* ProjectH namespace */