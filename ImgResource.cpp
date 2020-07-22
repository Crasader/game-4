#include "stdafx.h"
#include "ImgResource.h"
#include <stdexcept>
#include <utility>

using std::ifstream;
using std::ofstream;
using std::vector;
using stdext::hash_map;
using std::getline;
using std::string;
using std::logic_error;	
using std::make_pair;
using stdext::hash_map;

namespace ProjectH
{
	ImgResource::ImgResource(string fpath, Theme thm)
	{
		logFile.open("Log/Resource.log", ofstream::app);
		fileListPath = fpath;
		SetTheme(thm);
		LoadResource();
	}

	/* �׸� ���� �����Ѵ�. */
	void ImgResource::LoadResource()
	{
		ifstream fileList;
		ifstream inputFile;

		/* ���� ����Ʈ�� ����Ǿ��ִ� ������ ���� */
		fileList.open(fileListPath.c_str(), ifstream::in);
		if (fileList.bad())
		{
			logFile.write("SetTheme(): ���ϸ���Ʈ �������� ���� ����\n",
						  strlen("SetTheme(): ���ϸ���Ʈ �������� ���� ����\n"));
		}
		else
		{
			string file;
			int nLine;
			
			/* ���ϸ���Ʈ ������ ���μ��� üũ */
			for (nLine = 0; fileList.eof() == false; ++nLine)
				getline(fileList, file);
			file.clear();
			
			/* ������ Seek�� �ٽ� ó������ �ǵ��� */
			fileList.seekg(0, std::ios::beg);
			
			imgPaths.resize(5);
			Theme thm;
			while (fileList.good() && fileList.eof() == false)
			{
				/* �׸� ���� �о���� */
				ReadTheme(fileList, thm);

				getline(fileList, file);
				imgPaths[thm] = file;

				getline(fileList, file);

				/* ���ҽ� ���� ���� ���� */
				inputFile.open(file.c_str(), ifstream::in);
				if (inputFile.bad())
					continue;

				info.resize(nLine / 3);
				imgIdxColl.resize(nLine - (nLine / 3));
				imgDataColl.resize(nLine / 3);
				for (int i = 0; i < nLine - (nLine / 3); ++i)
					imgIdxColl[i].resize(1300);

				try
				{
					/* �����ѹ�, Ÿ���� ���� �� ����ũ��, �̹��� ������, �̹��� ������ �ε���,
					 * Doodad ���������� �о��.
					 */
					LoadVersionNum(inputFile, thm);
					LoadTileWidth(inputFile, thm);
					LoadTileHeight(inputFile, thm);
					LoadNumOfCombo(inputFile);
					LoadComboNames(inputFile);
					LoadImageData(inputFile, thm);
					LoadImageIndex(inputFile, thm);
					LoadImageIndex(inputFile, thm);
					LoadDoodadInfo(inputFile, thm);
				}
				catch (logic_error msg)
				{
					/* �����Ͽ��� ��� �α������� ����Ѵ�. */
					logFile.write(msg.what(), strlen(msg.what()));
					fileList.close();
				}

				inputFile.close();
			}
			fileList.close();
		}
	}

	/* ifstream���� �׸� ���� �о���� */
	std::ifstream& ImgResource::ReadTheme(std::ifstream &in, Theme &thm)
	{
		string line;
		
		getline(in, line);
		if (line == "GRASS")
			thm = GRASS;
		else if (line == "STON")
			thm = STON;
		else if (line == "CASTLE")
			thm = CASTLE;

		themeList.push_back(line);
		return in;
	}

	/* ���� �ؽ�Ʈ ���Ͽ��� �� ���ο� int�� �����Ͱ� �ϳ��� ���� ��쿡
	 * ���Ǵ� �޼ҵ�� �ڵ��� ������ ���̱� ���� ���Ǿ���.
	 */
	ifstream& ImgResource::GetOneValueInLine(ifstream &input, int &container, string errMsg)
	{
		string line;
		getline(input, line);
		if (input.good() == false)
			throw logic_error(errMsg);
		container = atoi(line.c_str());

		return input;
	}

	/* ����ڰ� �Ѱ��� mainKey, imgHash �Ű������� �̿��Ͽ�
	 * �ش� �̹��� �ε����� ���� ���� �̹��� �����͸� ����ڰ�
	 * �Ѱ��� container ��Ƽ� �����Ѵ�. �̹��� �ε����� ��ȿ����
	 * �ʴٸ� false�� ������.
	 */
	bool ImgResource::GetImageData(const int mainKey, const int imgHash, 
								   ImageData &container)
	{
		int idx = ImageIndex(mainKey, imgHash);
		if (idx == -1)
		{
			logFile.write("GetImageData(): ��ȿ���� ���� �̹��� �ε���\n",
						  strlen("GetImageData(): ��ȿ���� ���� �̹��� �ε���\n"));
			return false;
		}

		memcpy(&container, &imgDataColl[theme][idx], sizeof(ImageData));
		return true;
	}

	bool ImgResource::GetImageData(const int imgIdx, ImageData &container)
	{
		if (imgIdx < 0 || imgIdx >= static_cast<int>(imgDataColl[theme].size()))
		{
			logFile.write("GetImageData() (using index): ��ȿ���� ���� �̹��� �ε���\n",
						  strlen("GetImageData() (using index): ��ȿ���� ���� �̹��� �ε���\n"));
			return false;
		}

		memcpy(&container, &imgDataColl[theme][imgIdx], sizeof(ImageData));
		return true;
	}

	/* ����ڰ� �Ѱ��� container�� ��ư���� ���̴� �̹�������
	 * ������ ä���ش�.
	 */
	void ImgResource::GetBtnImgData(vector<ImageData> &container, vector<int> &idx)
	{
		vector<ImageData>::iterator iter = imgDataColl[theme].begin();
		DistinctionBtn op = DistinctionBtn();
		
		for ( ; iter != imgDataColl[theme].end(); ++iter)
		{
			iter = find_if(iter, imgDataColl[theme].end(), op);

			if (iter == imgDataColl[theme].end())
				break;
				
			container.push_back((*iter));
			container.back().width		= container.back().width - container.back().x;
			container.back().height		= container.back().height - container.back().y;
			idx.push_back(distance(imgDataColl[theme].begin(), iter));
		}
	}

	/* ����ڰ� �䱸�� Doodad�� ���� ������ container�� ��Ƽ� ���� */
	bool ImgResource::GetDoodadData(const int imgIdx, ImageData &container)
	{
		//if (imgIdx < info[static_cast<int>(theme)].doodadStartNum ||
		//	imgIdx >= info[static_cast<int>(theme)].doodadStartNum + DoodadDataColl.size())
		//{
		//	logFile.write("GetDoodadData() (using index): ��ȿ���� ���� �̹��� �ε���\n",
		//				  strlen("GetDoodadData() (using index): ��ȿ���� ���� �̹��� �ε���\n"));
		//	return false;
		//}
		memcpy(&container, &imgDataColl[theme][imgIdx], sizeof(ImageData));
		
		/* x, y �߽����� ��� */
		container.centerPointY = container.y - container.centerPointY;
		container.centerPointX = container.x - container.centerPointX;

		return true;
	}

	/* ����ڰ� �Ѱ��� Ÿ�� �� �̹��� �ε����� �̿��Ͽ� Ÿ�� ����
	 * �̹����� �÷��� �� �ִ��� ���θ� �����Ѵ�.
	 */
	bool ImgResource::AvailableBrush(const int tileIdx, const int imgIdx)
	{
		if (tileIdx >= static_cast<int>(imgDataColl[theme].size()) || 
			imgIdx >= static_cast<int>(imgDataColl[theme].size()))
		{
			logFile.write("AvailableBrush(): ��ȿ���� ���� Ÿ�� �Ǵ� �̹��� �ε���\n",
						  strlen("AvailableBrush(): ��ȿ���� ���� Ÿ�� �Ǵ� �̹��� �ε���\n"));
			return false;
		}
		if (imgDataColl[theme][imgIdx].availableTile == -1 ||
			imgDataColl[theme][imgIdx].availableTile == tileIdx)
			return true;
		return false;
	}

	/* ����ڰ� �Ѱ��� mainKey, imgHash �Ű������� �̿��Ͽ�
	 * �ؽ��ʿ� ����Ǿ� �ִ� �̹����� �ε����� ���´�.
	 * ���� �̹��� �ε����� �������� �ʴ´ٸ� -1�� ������.
	 */
	int ImgResource::ImageIndex(const int mainKey, const int imgHash)
	{
		return imgIdxColl[mainKey + (theme * 2)][imgHash];
		//return imgIdxColl[mainKey][imgHash];
	}

	/* Doodad ������ �����ϸ� true�� �ƴϸ�, false�� ����. */
	bool ImgResource::DoodadAvailable(const int idx, const int imgIdx)
	{
		if ((idx - info[theme].doodadStartNum) < 0 ||
			(idx - info[theme].doodadStartNum) >= static_cast<int>(DoodadDataColl.size()))
		{
			logFile.write("DoodadAvailable(): ��ȿ���� ���� Doodad �ε���\n",
						  strlen("DoodadAvailable(): ��ȿ���� ���� Doodad �ε���\n"));
			return false;
		}
		vector<int>::iterator iter;
		iter = find(DoodadDataColl[idx - info[theme].doodadStartNum].begin(), 
					DoodadDataColl[idx - info[theme].doodadStartNum].end(), imgIdx);

		if (iter == DoodadDataColl[idx - info[theme].doodadStartNum].end())
			return false;
		return true;
	}

	bool ImgResource::IsDoodad(const int imgIdx)
	{
		if (imgIdx >= info[theme].doodadStartNum && 
			imgIdx <= info[theme].doodadStartNum + static_cast<int>(DoodadDataColl.size()))
			return true;
		return false;
	}

	bool ImgResource::CheckImageType(int imgIdx, ImageType type)
	{
		if (imgIdx < 0 || imgIdx > static_cast<int>(imgIdxColl[theme * 2].size()))
		{
			logFile.write("CheckImageType(): ��ȿ���� ���� �̹��� �ε���\n",
						  strlen("CheckImageType(): ��ȿ���� ���� �̹��� �ε���\n"));
			return false;
		}
		if (imgDataColl[theme][imgIdx].type == type)
			return true;
		return false;
	}

	/* �޺��ڽ��� Name�� �о���� */
	ifstream& ImgResource::LoadComboNames(ifstream &input)
	{
		ComboNames.reserve(nComboBox);
		string name;
		for (int i = 0; i < nComboBox; ++i)
		{
			getline(input, name);
			if (input.bad() == true)
			{
				logFile.write("LoadComboNames(): �޺��ڽ� �̸� �о���̱� ����\n",
							  strlen("LoadComboNames(): �޺��ڽ� �̸� �о���̱� ����\n"));
				return input;
			}
			ComboNames.push_back(name);
		}
		return input;
	}

	/* �޺��ڽ��� ������ �о���� */
	ifstream& ImgResource::LoadNumOfCombo(ifstream &input)
	{
		GetOneValueInLine(input, nComboBox, "LoadNumOfCombo(): �޺��ڽ� ���� �о���̱� ����\n");
		return input;
	}

	/* �����ѹ��� �о���δ�. */
	ifstream& ImgResource::LoadVersionNum(ifstream &input, int thm)
	{
		GetOneValueInLine(input, info[thm].version, "LoadVersionNum(): �����ѹ� �о���̱� ����\n");
		return input;
	}

	/* Ÿ���� ���α��̸� �о���δ�. */
	ifstream& ImgResource::LoadTileWidth(ifstream &input, int thm)
	{
		GetOneValueInLine(input, info[thm].width, "LoadTileWidth(): Ÿ���� ���α��� �о���̱� ����\n");
		return input;
	}

	/* Ÿ���� ���α��̸� �о���δ�. */
	ifstream& ImgResource::LoadTileHeight(ifstream &input, int thm)
	{
		GetOneValueInLine(input, info[thm].height, "LoadTileHeight(): Ÿ���� ���α��� �о���̱� ����\n");
		return input;
	}

	/* �̹��� �����͸� ��� �о���δ�. */
	ifstream& ImgResource::LoadImageData(ifstream &input, int thm)
	{
		int nImgData = 0;

		GetOneValueInLine(input, nImgData, "LoadImageData(): �̹��� ������ ���� �о���̱� ����\n");
		ImageData data;
		string line;
		vector<string> splitedStr;
		for (int i = 0; i < nImgData; ++i)
		{
			getline(input, line);
			if (input.good() == false)
				throw logic_error("LoadImageData(): �̹��� �������� ������ �߸���\n");

			/* ���� �����ڷ� ���ڿ��� �ɰ��� ��, splitedStr���Ͱ� ����ִٸ�,
			 * �̹��� ���ҽ� ������ ����ִ� �ؽ�Ʈ ���Ͽ� ���͹��ڰ� ���
			 * ��������̴�.
			 */
			SplitString(line, splitedStr);
			if (splitedStr.empty() == true)
				throw logic_error("LoadImageData(): �̹��� ������ �о���̱� ����\n");
			
			data.x				= atoi(splitedStr[0].c_str());
			data.y				= atoi(splitedStr[1].c_str());
			data.width			= atoi(splitedStr[0].c_str()) + atoi(splitedStr[2].c_str());
			data.height			= atoi(splitedStr[1].c_str()) + atoi(splitedStr[3].c_str());
			data.moveable		= (atoi(splitedStr[4].c_str()) == 1)? true : false;
			data.centerPointX	= atoi(splitedStr[0].c_str()) + atoi(splitedStr[5].c_str());
			data.centerPointY	= atoi(splitedStr[1].c_str()) + atoi(splitedStr[6].c_str());
			data.availableTile	= atoi(splitedStr[7].c_str());
			data.type			= atoi(splitedStr[8].c_str());
			data.isBtn			= atoi(splitedStr[9].c_str());
			data.tileType		= atoi(splitedStr[10].c_str());
			data.shadow			= atoi(splitedStr[11].c_str());

			/* ���� ��ǥŸ���̶��... */
			if (data.type == 0)
			{
				for_each(imgIdxColl[data.tileType].begin(), imgIdxColl[data.tileType].end(),
						 ValueGenerator(i));
			}

			imgDataColl[thm].push_back(data);

			line.clear();
			splitedStr.clear();
		}
		return input;
	}

	/* �̹��� �ε��� ������ ��� �о���δ�. */
	ifstream& ImgResource::LoadImageIndex(ifstream &input, int thm)
	{
		int mainKey;
		int nImgKey;

		GetOneValueInLine(input, mainKey, "LoadImageIndex(): ����Ű �о���̱� ����\n");
		GetOneValueInLine(input, nImgKey, "LoadImageIndex(): ����Ű�� ���� �о���̱� ����\n");
		
		string line;
		vector<string> splitedStr;
		for (int i = 0; i < nImgKey; ++i)
		{
			getline(input, line);
			if (input.good() == false)
				throw logic_error("LoadImageIndex(): �̹��� �ε��� �о���̱� ����\n");

			/* ���� �����ڷ� ���ڿ��� �ɰ��� ��, splitedStr���Ͱ� ����ִٸ�,
			 * �̹��� ���ҽ� ������ ����ִ� �ؽ�Ʈ ���Ͽ� ���͹��ڰ� ���
			 * ��������̴�.
			 */
			SplitString(line, splitedStr);
			if (splitedStr.empty() == true)
				throw logic_error("LoadImageIndex(): �̹��� �ε����� ������ �߸���\n");
			//tempColl.insert(make_pair(atoi(splitedStr[0].c_str()), atoi(splitedStr[1].c_str())));
			imgIdxColl[mainKey + (thm * 2)][atoi(splitedStr[0].c_str())] = atoi(splitedStr[1].c_str());

			line.clear();
			splitedStr.clear();
		}
		//imgIdxColl.insert(make_pair(mainKey, tempColl));
		return input;
	}

	/* Doodad ������ ��� �о���δ�. */
	ifstream& ImgResource::LoadDoodadInfo(std::ifstream &input, int thm)
	{
		GetOneValueInLine(input, info[thm].nDoodad, "LoadDoodadInfo(): Doodad�� ���� �о���̱� ����\n");
		GetOneValueInLine(input, info[thm].doodadStartNum, "LoadDoodadInfo(): Doodad�� ���� �о���̱� ����\n");
		DoodadDataColl.reserve(info[thm].nDoodad);

		string line;
		vector<string> splitedStr;
		for (int i = 0; i < info[thm].nDoodad; ++i)
		{
			getline(input, line);
			if (input.good() == false)
				throw logic_error("LoadDoodadInfo(): Doodad ���� �о���̱� ����\n");

			/* ���� �����ڷ� ���ڿ��� �ɰ��� ��, splitedStr���Ͱ� ����ִٸ�,
			 * �̹��� ���ҽ� ������ ����ִ� �ؽ�Ʈ ���Ͽ� ���͹��ڰ� ���
			 * ��������̴�.
			 */
			SplitString(line, splitedStr);
			if (splitedStr.empty() == true)
				throw logic_error("LoadDoodadInfo(): Doodad�� ������ �߸���\n");

			int key = atoi(splitedStr[0].c_str());
			vector<int> tempColl;
			tempColl.reserve(splitedStr.size() - 1);
			for (unsigned int j = 1; j < splitedStr.size(); ++j)
				tempColl.push_back(atoi(splitedStr[j].c_str()));

			DoodadDataColl.push_back(tempColl);

			line.clear();
			splitedStr.clear();
		}
		return input;
	}

	/* ���� �����ڷ� �Ͽ� ���ڿ��� �ɰ���. */
	void ImgResource::SplitString(string &str, vector<string> &coll)
	{
		string::iterator startPos, endPos;

		startPos = str.begin();
		endPos = startPos;
		while (endPos != str.end())
		{
			if (*endPos == '\t' && endPos != str.begin())
			{
				coll.push_back(string(startPos, endPos));
				startPos = endPos + 1;
			}
			++endPos;
			if (endPos == str.end() && startPos != endPos)
				coll.push_back(string(startPos, endPos));
		}
	}

	void ImgResource::GetPillar(const int tileType, ImageData &container, bool top)
	{
		if (top == true)
			GetImageData(tileType, 1296, container);
		else
			GetImageData(tileType, 1297, container);
	}

	int ImgResource::GetShadowIndex(const int tileType, const int doodadIdx)
	{
		int shadow = imgDataColl[theme][doodadIdx].shadow;
		if (shadow == 1)
			return ImageIndex(tileType, 1298);
		else if (shadow == 2)
			return ImageIndex(tileType, 1299);
		else
			return 0;
	}

	ImgResource* ImgResource::Instance()
	{
		static ImgResource singleton;

		return &singleton;
	}
} /* namespace ProjectH */