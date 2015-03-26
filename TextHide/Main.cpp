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

typedef struct
{
	string str;			//ͬ����ַ���
	bool code;		//ͬ��ʵı���
}Syword;

vector<vector<Syword>> dictwords;				//����Ϊ2��ͬ�����
std::string c1;													//��Ȼ����1
std::vector<bool> X;											//��Ȼ���侭��ͬ��ʱ����
std::vector<std::vector<bool>> Xx;					
std::string c2;													//��Ȼ����2
std::vector<bool> Y;											//��Ȼ����2����ͬ��ʱ����
std::vector<std::vector<bool>> Yy;					
std::string source_word;										//Ҫ���ܵĶ�����
std::vector<bool> source_binary;
std::string o1;													//�������1
std::string o2;													//�������2

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
	return left.size() % 2 == right.size() % 2;
}


//�������ı�����ͬ���
Syword FindString(string str)
{
	Syword d;
	d.str = "";
	d.code = -1;
	for (int i = 0; i < dictwords.size(); i++)
	{
		for (int j = 0; j < dictwords[i].size(); j++)
		{
			if (dictwords[i][j].str.compare(str) == 0)
			{
				return dictwords[i][j];
			}
		}
	}
	return d;
}

//����Ȼ��ͨ��ͬ��ʱ���ɶ�����
void codeParagraph()
{
	std::vector<std::string> c1_words;
	str_split_ex(c1_words, c1.c_str(), " ");
	int lastFlag = -1;
	for (std::vector<std::string>::const_iterator it = c1_words.begin(); it != c1_words.end(); ++it)
	{
		Syword pr = FindString(*it);
		if (pr.str != "")
		{
			X.push_back((bool)pr.code);
			if (pr.code == lastFlag)
			{
				Xx[Xx.size() - 1].push_back(pr.code);
			}
			else
			{
				std::vector<bool> tempBool;
				tempBool.push_back(pr.code);
				lastFlag = (int)pr.code;
				Xx.push_back(tempBool);
			}
		}
	}

	lastFlag = -1;
	std::vector<std::string> c2_words;
	str_split_ex(c2_words, c2.c_str(), " ");
	for (std::vector<std::string>::const_iterator it = c2_words.begin(); it != c2_words.end(); ++it)
	{
		Syword pr = FindString(*it);
		if (pr.str != "")
		{
			Y.push_back((bool)pr.code);
			if (pr.code == lastFlag)
			{
				Yy[Yy.size() - 1].push_back(pr.code);
			}
			else
			{
				std::vector<bool> tempBool;
				tempBool.push_back(pr.code);
				lastFlag = (int)pr.code;
				Yy.push_back(tempBool);
			}
		}
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
	for (int i = 0; i < source_binary.size(); ++i)
	{

	}
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
	process();
	writeResult();
	goodbye(); 
	return 0;
}