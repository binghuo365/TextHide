#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<cmath>
#include<ctime>
#include<stdlib.h>
#include<conio.h>
#include<io.h> 
#include<direct.h>
#include<cstdlib>
#include <stdio.h>
#include<windows.h>
#include <dos.h>
#include <bitset>
#include <assert.h>

using namespace std;

struct Syword
{
	string str;			//ͬ����ַ���
	bool code;		//ͬ��ʵı���
};

//��¼���ҵķ���λ��
struct position
{
	int i;
	int j;
};			

struct Pos
{
	int di;
	int dj;			//����������ͬ����ֵ��е�λ��
	int i;				//�������е�λ��
};

vector<vector<Syword>> dictwords;				//����Ϊ2��ͬ�����
std::string c1;													//��Ȼ����1
std::vector<std::string> c1_words;
std::vector<bool> X;											//��Ȼ���侭��ͬ��ʱ����
std::vector<std::vector<bool>> Xx;					
std::string c2;													//��Ȼ����2
std::vector<std::string> c2_words;
std::vector<bool> Y;											//��Ȼ����2����ͬ��ʱ����
std::vector<std::vector<bool>> Yy;					
std::string source_word;										//Ҫ���ܵĶ�����
std::vector<bool> source_binary;
std::vector<int>	mergeSet;								//���ϲ�
std::string o1;													//�������1
std::string o2;													//�������2
vector<Pos> c1Map;											//�����ı����ֵ�ͬ��ʵ�ӳ�� 
vector<Pos> c2Map;											

//����ͬ��ʿⲢ���ҳ�����Ϊ2��ͬ���
void LoadWordLib()
{
	ifstream fin("tlex.data.txt");
	string str;
	while (getline(fin, str))
	{
		string tstr = str;
		int count = 0;
		vector<Syword> temp;
		while (tstr != "")
		{
			Syword t;
			int  p = tstr.find(" ");
			t.str = tstr.substr(0, p);
			tstr = tstr.substr(p + 1);
			t.code = count;
			count++;
			temp.push_back(t);
			if (count == 3) break;
		}
		if (count == 2)
		{
			dictwords.push_back(temp);
		}
		temp.clear();
	}
	fin.close();
}

//������������Ҫ���ܵĶ�����
void LoadData()
{
	ifstream fin("c_1.txt");
	string str;
	while (getline(fin, str))
	{
		c1 += str;
	}
	fin.close();

	ifstream fin2("c_2.txt");
	while (getline(fin2, str))
	{
		c2 += str;
	}
	fin2.close();

	ifstream fin3("source_binary.txt");
	while (getline(fin3, str))
	{
		source_word += str;
	}
	for (int i = 0; i < str.length(); ++i)
	{
		source_binary.push_back(str.at(i) == '1');
	}
	fin3.close();
}

//�ַ��ָ�
void str_split_ex(std::vector<std::string>& pvectorstr, const char * const pstr, const char * chEx)
{
	if (0 == pstr)
	{
		return;
	}
	pvectorstr.clear();
	const char * pstart = pstr;
	for (const char * pchr = pstr;; pchr++)
	{
		if ('\0' == *pchr)
		{
			if (pchr != pstart)
			{
				pvectorstr.push_back(std::string(pstart, pchr - pstart));
			}
			return;
		}
		else
		{
			for (const char* ch = chEx; *ch != '\0'; ch++)
			{
				if (*ch == (*pchr))
				{
					if (pchr != pstart)
					{
						pvectorstr.push_back(std::string(pstart, pchr - pstart));
					}
					pstart = pchr + 1;
					break;
				}
			}

		}
	}
}

//��ż����
bool parityCal(std::vector<bool> left, std::vector<bool> right)
{
	assert(left.size());
	assert(right.size());
	return  !(left.size() % 2 == right.size() % 2);
}

//�������ı�����ͬ���
position FindString(string str)
{
	position p;
	p.i = -1;
	p.j = -1;
	int i, j;
	for (i = 0; i < dictwords.size(); i++)
	{
		for (j = 0; j < dictwords[i].size(); j++)
		{
			if (dictwords[i][j].str.compare(str) == 0)
			{
				p.i = i;
				p.j = j;
				return p;
			}
		}
	}
	return p;
}

//����Ȼ��ͨ��ͬ��ʱ���ɶ�����
void codeParagraph()
{
	c1_words.clear();
	str_split_ex(c1_words, c1.c_str(), " ");
	int lastFlag = -1;
	for (int i = 0; i < c1_words.size(); ++i)
	{
		position pr = FindString(c1_words[i]);
		if (pr.i != -1)
		{
			Pos pos;
			pos.di = pr.i;
			pos.dj = pr.j;
			pos.i = i;
			c1Map.push_back(pos);

			X.push_back((bool)dictwords[pr.i][pr.j].code);
			if (dictwords[pr.i][pr.j].code == lastFlag)
			{
				Xx[Xx.size() - 1].push_back(dictwords[pr.i][pr.j].code);
			}
			else
			{
				std::vector<bool> tempBool;
				tempBool.push_back(dictwords[pr.i][pr.j].code);
				lastFlag = (int)dictwords[pr.i][pr.j].code;
				Xx.push_back(tempBool);
			}
		}
	}

	lastFlag = -1;
	c2_words.clear();
	str_split_ex(c2_words, c2.c_str(), " ");
	for (int i = 0; i < c2_words.size(); ++i)
	{
		position pr = FindString(c2_words[i]);
		if (pr.i != -1)
		{
			Pos pos;
			pos.di = pr.i;
			pos.dj = pr.j;
			pos.i = i;
			c2Map.push_back(pos);

			Y.push_back((bool)dictwords[pr.i][pr.j].code);
			if (dictwords[pr.i][pr.j].code == lastFlag)
			{
				Yy[Yy.size() - 1].push_back(dictwords[pr.i][pr.j].code);
			}
			else
			{
				std::vector<bool> tempBool;
				tempBool.push_back(dictwords[pr.i][pr.j].code);
				lastFlag = (int)dictwords[pr.i][pr.j].code;
				Yy.push_back(tempBool);
			}
		}
	}
}

size_t getXxInX(int a, int b)
{
	int index = 0;
	for (int i = 0; i < a - 1; ++i)
	{
		index += Xx[i].size();
	}
	return size_t(index + b);
}

size_t getYyInY(int a, int b)
{
	int index = 0;
	for (int i = 0; i < a - 1; ++i)
	{
		index += Yy[i].size();
	}
	return size_t(index + b);
}

void reverse(int& P, int& Q, int i)
{
	if (P > 0)
	{//��תx

		if (Q > 0)
		{
			//0->1
			if (Xx[i + 1][0])
			{
				// 0011
				X[getXxInX(i, int(Xx[i].size() - 1))] = true;
				Xx[i].pop_back();
				Xx[i + 1].push_back(true);
			}
			else
			{
				//1100
				X[getXxInX(i + 1, 0)] = true;
				Xx[i].push_back(true);
				Xx[i + 1].pop_back();
			}
			--Q;
		}
		else
		{
			//1 ->0
			if (Xx[i + 1][0])
			{
				// 0011
				X[getXxInX(i + 1, 0)] = false;
				Xx[i].push_back(false);
				Xx[i + 1].pop_back();

			}
			else
			{
				//1100
				X[getXxInX(i, Xx[i].size() - 1)] = false;
				Xx[i].pop_back();
				Xx[i + 1].push_back(false);
			}
			++Q;
		}
		--P;
	}
	else
	{
		//��תY
		if (Q > 0)
		{
			//0->1
			if (Yy[i + 1][0])
			{
				// 0011
				Y[getYyInY(i, Yy[i].size() - 1)] = true;
				Yy[i].pop_back();
				Yy[i + 1].push_back(true);
			}
			else
			{
				//1100
				Y[getYyInY(i + 1, 0)] = true;
				Yy[i].push_back(true);
				Yy[i + 1].pop_back();
			}
			--Q;
		}
		else
		{
			//1 ->0
			if (Yy[i + 1][0])
			{
				// 0011
				Y[getYyInY(i + 1, 0)] = false;
				Yy[i].push_back(false);
				Yy[i + 1].pop_back();

			}
			else
			{
				//1100
				Y[getYyInY(i, (Yy[i].size() - 1))] = false;
				Yy[i].pop_back();
				Yy[i + 1].push_back(false);
			}
			++Q;
		}
		++P;
	}
}

//д���
void writeResult()
{
	ofstream f1("o_1.txt");
	f1 << o1;
	ofstream f2("o_2.txt");
	f2 << o2;
}

//����
void process()
{
	// X.size > Y.size
	assert(Xx.size() >= Yy.size());
	assert(Yy.size() >= source_binary.size());

	int P = 1;
	int Qx = 1;
	int Qy = 1;
	for (int i = 0; i < source_binary.size() - 1; ++i)
	{
		if (parityCal(Xx[i], Yy[i]) == source_binary[i])
		{
			//ͬ��ż
			continue;
		}

		int K = 0;
		int kx = 0;
		int ky = 0;

		//�ж�Kֵ
		if (Xx[i].size() > 1)  ++kx;
		if (Xx[i + 1].size() > 1) ++kx;
		if (Yy[i].size() > 1) ++ky;
		if (Yy[i + 1].size() > 1)  ++ky;

		K = kx + ky;

		switch (K)
		{
		case 4:
		{
				  if (P > 0)
				  {
					  reverse(P, Qx, i);
				  }
				  else
				  {
					  reverse(P, Qy, i);
				  }
		}
			break;
		case 3:
		{
				  //ѡ��k=2��һ��
				  if (kx == 2)
				  {
					  P = 1;
					  reverse(P, Qx, i);
				  }
				  else
				  {
					  P = 0;
					  reverse(P, Qy, i);
				  }
		}
			break;
		case 2:
		{
				  //��������
		}
			break;
		case 1:
		{
				  if (kx == 1)
				  {
					  P = 1;
					  reverse(P, Qx, i);
				  }
				  else
				  {
					  P = 0;
					  reverse(P, Qy, i);
				  }
		}
			break;
		case 0:
		{
				  X[getXxInX(i + 1, 0)] = !X[getXxInX(i + 1, 0)];
				  Y[getYyInY(i + 1, 0)] = !Y[getXxInX(i + 1, 0)];
				  Xx[i].push_back(Xx[i + 1][0]);
				  Yy[i].push_back(Yy[i + 1][0]);
				  mergeSet.push_back(i + 1);
				  if (i != source_binary.size() - 2)
				  {
					  Xx[i].insert(Xx[i].end(), Xx[i + 2].begin(), Xx[i + 2].end());
					  Yy[i].insert(Yy[i].end(), Yy[i + 2].begin(), Yy[i + 2].end());
					  mergeSet.push_back(i + 2);
					  i += 2;
				  }
		}
			break;
		}
	}
}

void restoreWord()
{
	for (int i = 0; i < X.size(); ++i)
	{
		c1_words[c1Map[i].i] = dictwords[c1Map[i].di][X[i]].str;
	}

	for (int ii = 0; ii < c1_words.size(); ++ii)
	{
		o1 += c1_words[ii] + " ";
	}

	for (int j = 0; j < Y.size(); ++j)
	{
		c2_words[c2Map[j].i] = dictwords[c2Map[j].di][Y[j]].str ;
	}

	for (int jj = 0; jj < c2_words.size(); ++jj)
	{
		o2 += c2_words[jj] + " ";
	}
}

void writeCode(std::vector<bool>& x)
{
	for (int i = 0; i < x.size(); ++i)
	{
		std::cout << x[i];
	}
	std::cout << std::endl << std::endl;
}

void goodbye()
{
	cout << endl << endl << endl;
	cout << "\t\t..............................................." << endl;
	cout << endl;
	cout << "\t\t\t  ��л����ʹ���´��ټ�!" << endl;
	cout << endl;
	cout << "\t\t..............................................." << endl;
	system("pause");
}
void Init()
{
	cout << endl << endl << endl;
	cout << "\t\t************************************************" << endl;
	cout << endl;
	cout << "\t\t\t\tϵͳ��ʼ����" << endl;
	cout << endl;
	cout << "\t\t************************************************" << endl;
}


int main()
{
	int t;
	int k;
	/////////////////////////////////////////////////////////////////////////////////////////////////

	Init();
	//����ʵ�
	LoadWordLib();
	LoadData();
	codeParagraph();
	std::cout << "X����:" << endl;
	writeCode(X);
	std::cout << "Y����:" << endl;
	writeCode(Y);
	std::cout << "Ҫ���صĶ�����:" << endl;
	writeCode(source_binary);

	process();

	std::cout << "X���뷭ת��:" << endl;
	writeCode(X);
	std::cout << "Y���뷭ת��:" << endl;
	writeCode(Y);

	restoreWord();
	std::cout << "o1:" << endl;
	std::cout << o1 << endl;
	std::cout << "Yo2:" << endl;
	std::cout << o2 << endl;
	writeResult();
	goodbye(); 
	return 0;
}