#ifndef ProjectH_ImgResource_H
#define ProjectH_ImgResource_H

#include <iostream>
#include <vector>
#include <string>
#include <hash_map>
#include <fstream>
#include <algorithm>

namespace ProjectH
{
	class ImgResource
	{
		friend class DistinctionBtn;

	public:
		struct ImageData
		{
			int x, y;			/* ������ x, y */	
			int width;			/* x + width */
			int height;			/* y + height */
			int centerPointX;	/* x + x �߽ɰ� */
			int centerPointY;	/* y + y �߽ɰ� */
			int availableTile;	/* �ش� �̹����� ��� Ÿ������ �÷��� �� �ִ��� �Ǻ�. -1�̸� ���Ÿ�Ͽ� �ö� �� ���� */
			int type;			/* �̹����� Ÿ�� */
			int isBtn;			/* ��ư ���� */
			int tileType;		/* Ÿ���� ����(Ű ��) */
			int shadow;			/* �׸����� ���� */
			bool moveable;		/* �̵� ���ɿ��� */
		};

		typedef enum
		{
			MAJOR_TILE = 0,		/* ��ǥŸ�� */
			AROUND_TILE,		/* �ֺ�Ÿ�� */
			GENERAL_TILE,		/* �Ϲ�Ÿ�� */
			ACCESSORY,			/* �׼����� */
			PILLAR				/* ��� */
		} ImageType;

		typedef enum
		{
			GRASS		= 0,
			STON,
			CASTLE
		} Theme;

	public:
		static
		ImgResource*	Instance		();

		~ImgResource() { logFile.close(); }

		int ImageIndex(const int, const int);									/* �̹��� �ε��� ���� */
		int TileWidth() const { return info[static_cast<int>(theme)].width; }	/* Ÿ���� ���α��� ���� */
		int TileHeight() const { return info[static_cast<int>(theme)].height; }	/* Ÿ���� ���α��� ���� */
		bool DoodadAvailable(const int idx, const int imgIdx);					/* Doodad�� ���հ��� ���� ���� */

		/* ����ڰ� �Ѱ��� Ű ���� �̿��Ͽ� �̹����� ���� �����͵���
		 * ����ڰ� �Ѱ��� ImageData ����ü�� �����Ͽ� �ش�.
		 * ���� Ű ���� �ش��ϴ� �����͸� ã�� �� �������, false��
		 * �����Ͽ� �ش簪�� ��µ� ���������� �˸�.
		 */
		bool GetImageData(const int, const int, ImageData&);
		bool GetImageData(const int, ImageData&);
		const std::string& GetBtnImgPath() const { return imgPaths[static_cast<int>(theme)]; }
		const std::vector<std::string>& GetThemeList() { return themeList; }
		const int NumberOfComboBox() const { return nComboBox; }
		const std::string& ComboBoxName(int idx) const { return ComboNames[idx]; }
		const int GetTileType(const int imgIdx) { return imgDataColl[theme][imgIdx].tileType; }

		/* Ÿ�ϰ� ����Ǵ� �κ��� ��հ��� ���� ä�� */
		void GetPillar(const int tileType, ImageData &container, bool top = false);

		/* Doodad �ε���, TileType�� �޾Ƽ� �׸��� �ε��� ���� */
		int GetShadowIndex(const int tileType, const int doodadIdx);

		/* ����ڿ��� ��ư�̹����� ���õ� �̹��� �������� �����ؼ�
		 * �Ѱ��ش�.
		 */
		void GetBtnImgData(std::vector<ImageData>&, std::vector<int>&);

		/* �׸����� ���� */
		void SetTheme(int thm = 0) { theme = static_cast<Theme>(thm); }

		/* ����ڰ� �Ѱ��� �ε����� �ش��ϴ� Doodad �����͸� ä����. */
		bool GetDoodadData(const int, ImageData&);

		/* ����ڰ� �Ѱ��� �̹��� �ε��� ���� Doodad������ ���θ� ���� */
		bool IsDoodad(const int);

		/* ����ڰ� �Ѱ��� Ÿ�� �� �̹��� �ε����� ������ ��������
		 * ���θ� �����Ѵ�.
		 */
		bool AvailableBrush(const int, const int);

		/* ��ǥŸ�� ���� */
		bool IsMajorTile(const int imgIdx) { return CheckImageType(imgIdx, MAJOR_TILE); }

		/* �ֺ�Ÿ�� ���� */
		bool IsAroundTile(const int imgIdx) { return CheckImageType(imgIdx, AROUND_TILE); }
		
		/* �Ϲ�Ÿ�� ���� */
		bool IsGeneralTile(const int imgIdx) { return CheckImageType(imgIdx, GENERAL_TILE); }

		/* �׼����� ���� */
		bool IsAccessory(const int imgIdx) { return CheckImageType(imgIdx, ACCESSORY); }

		/* ��� ���� */
		bool IsPillar(const int imgIdx) { return CheckImageType(imgIdx, PILLAR); }

		/* �̵� ���� */
		bool IsMoveable(const int imgIdx) { return imgDataColl[theme][imgIdx].moveable; }

	private:
		ImgResource(std::string fpath = "Data/ImgResource.txt", Theme theme = GRASS);

		void LoadResource();
		std::ifstream& LoadVersionNum(std::ifstream&, int);
		std::ifstream& LoadTileWidth(std::ifstream&, int);
		std::ifstream& LoadTileHeight(std::ifstream&, int);
		std::ifstream& LoadImageData(std::ifstream&, int);
		std::ifstream& LoadImageIndex(std::ifstream&, int);
		std::ifstream& LoadDoodadInfo(std::ifstream&, int);
		std::ifstream& LoadNumOfCombo(std::ifstream&);
		std::ifstream& LoadComboNames(std::ifstream&);

		/* �׸� ���� �о���� */
		std::ifstream& ReadTheme(std::ifstream&, Theme&);

		/* �� �޼ҵ�� �� ���ο� int�� ���� �ϳ��� ��쿡�� ������ �ϸ�,
		 * �� ������ �о������ �ش� ���� int������ ��ȯ�Ͽ� �����Ͽ� �ش�.
		 */
		std::ifstream& GetOneValueInLine(std::ifstream&, int&, std::string);
		
		/* ���� �����ڷ� �Ͽ� ���ڿ��� �ɰ���. */
		void SplitString(std::string&, std::vector<std::string>&);

		bool CheckImageType(int, ImageType);

	protected:
		struct GeneralInfo 
		{
			int	version;			/* ���� �ѹ� */
			int	width;				/* Ÿ���� ���� ���� */
			int	height;				/* Ÿ���� ���� ���� */
			int	nImgData;			/* Number of ImageData */
			int	nDoodad;			/* Number of DoodadData */
			int doodadStartNum;		/* Doodad ���� ��ȣ */
		};

	protected:
		Theme theme;							/* �׸� �� */
		std::string fileListPath;				/* �̹��� ���ҽ� ���ϵ��� ����Ʈ�� ����ִ� ������ ��ġ */
		std::ofstream logFile;					/* Log�� ����ϱ� ���� ���� */
		std::vector<std::string> imgPaths;		/* ��ư�� ���� �̹����� �׸��� ��ε� */	
		std::vector<std::string> themeList;		/* �׸� ���ڿ� ����Ʈ */
		std::vector<std::string> ComboNames;	/* �޺��ڽ��� �� �̸��� */
		int nComboBox;							/* �޺��ڽ� ������ ���� */

		/* �׸����� ����, Ÿ���� ���� �� ����, �̹��� �������� ����,
		 * Doodad�� ����, Doodad ���۹�ȣ ���� �����ϰ� �ִ� ����
		 */
		std::vector<GeneralInfo> info;

		/* �̹��� ������ �ε����� �����ϰ� �ִ� �ؽ��� */
		std::vector<std::vector<int> > imgIdxColl;

		/* Doodad �����͸� �����ϰ� �ִ� �ؽ��� */
		std::vector<std::vector<int> > DoodadDataColl;

		/* �̹��� �����͸� �����ϰ� �ִ� ���� */
		std::vector<std::vector<ImageData> > imgDataColl;
	};


	/* �̹��� �����Ϳ��� ��ư�̹����� �����ϱ� ����
	 * Function Object
	 */
	class DistinctionBtn
	{
	public:
		bool operator() (const ImgResource::ImageData &val)
		{
			if (val.isBtn)
				return true;
			return false;
		}
	};

	class ValueGenerator
	{
	public:
		ValueGenerator(const int val): value(val) {}
		void operator() (int &val)
		{
			val = value;
		}

	private:
		int value;
	};
} /* namespace ProjectH */

#define IMGRESOURCE() ProjectH::ImgResource::Instance()

#endif /* ProjectH_ImgResource_H */