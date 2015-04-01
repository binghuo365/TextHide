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
#include <map>

using namespace std;

struct Syword
{
	string str;			//ͬ����ַ���
	bool code;			//ͬ��ʵı���
	int  index;
};

//��¼���ҵķ���λ��
struct position
{
	int i;
	int j;
};			

struct Pos
{
	int index;				//�������е�λ��
	int di;
	int dj;					//����������ͬ����ֵ��е�λ��
};

vector< vector<Syword> > dictwords;								//����Ϊ2��ͬ�����	
map< string, Syword>	dictMap;
std::vector<bool> source_binary;								//Ҫ���ܵĶ�����
int transx0;
int transx1;
int transy0;
int transy1;

//��ӡ�õ�
void writeCode(std::vector<bool>& x)
{
	for (int i = 0; i < x.size(); ++i)
	{
		std::cout << x[i];
	}
	std::cout << std::endl << std::endl;
}

std::string transforCode(std::vector<bool>& x)
{
	std::string str;
	for (int i = 0; i < x.size(); ++i)
	{
		str+= x[i] ? "1" : "0";
	}
	return str;
}

std::string transforXxYyCode(std::vector< std::vector<bool> >& xx)
{
	std::string str;
	for (int i = 0; i < xx.size(); ++i)
	{
		std::string temp = xx[i].front() ? "1" : "0";
		str = str + temp + ":";
		std::string st;
		char t[256];
		sprintf(t,"%d",xx[i].size());
		st =t;
		str = str + st + "  ";
	}
	return str;
}

int getcodenum(std::vector<bool>& x, bool type)
{
	int total =0;
	for(int i = 0; i< x.size(); ++i)
	{
		if(type&&x[i]) total++;
		else if(!type && !x[i]) total++;
	}
	return total;
}

//��ȡ�ļ��������е��ļ���
vector<string> getfilename(string filepath)
{  
	vector<string> filename;
	_finddata_t file;  
    long lf;  
    //�����ļ���·��  
	filepath=filepath+"*.*";
    if((lf = _findfirst(filepath.data(), &file))==-1)  
        cout<<"Not Found!"<<endl;  
    else{  
        //����ļ���  
  
        while(_findnext( lf, &file)==0){ 
			filename.push_back(file.name);
        }  
    }	
    _findclose(lf);	
	return filename;
} 

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
			temp[0].index = dictwords.size() - 1;
			temp[1].index = dictwords.size() - 1;
			dictMap[temp[0].str] = temp[0];
			dictMap[temp[1].str] = temp[1];
		}
		temp.clear();
	}
	fin.close();
}

void randomBinary(int _size)
{
	srand(time(0));
	for(int i = 0; i<_size; ++i)
	{
		int a = rand() % 2;
		source_binary.push_back(a);
		std::cout << a ;
	}
	std::cout<<endl;
}

void storeBindary()
{
	ofstream f("source_binary.txt");
	string str;
	for(int i = 0; i< source_binary.size(); ++i)
	{
		str += source_binary[i] ? "1" : "0"; 
	}
	f <<str<<endl;
	f.close();
}

//����Ҫ���ܵĶ�����
void LoadBindary(std::string fileName)
{
	ifstream fin3(fileName.c_str());
	string str;
	std::string source_word;
	while (getline(fin3, str))
	{
		source_word += str;
	}
	for (int i = 0; i < source_word.length(); ++i)
	{
		source_binary.push_back(source_word.at(i) == '1');
	}
	fin3.close();
}

//�ַ��ָ� ����ȥ���  ���ǰ� ����һ������һ�����ʷֿ��������õ�
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
	std::map<std::string, Syword>::iterator it = dictMap.find(str);
	if(it != dictMap.end())
	{
		p.i = it->second.index;
		p.j = it->second.code;
	}
	return p;
}

//����Ȼ��ͨ��ͬ��ʱ���ɶ�����
void codeParagraph(std::string fileName, std::vector< std::string >& words, 
				   std::vector< bool >& xy, std::vector< std::vector<bool> >& xxyy,
				   vector<Pos>& wordmap)
{
	ifstream fin(fileName.c_str());
	string str;
	std::string sourceWord;
	while (getline(fin, str))
	{
		sourceWord += str;
	}
	fin.close();

	words.clear();
	str_split_ex(words, sourceWord.c_str(), " ");
	int lastFlag = -1;
	for (int i = 0; i < words.size(); ++i)
	{
		position pr = FindString(words[i]);
		if (pr.i != -1)
		{
			Pos pos;
			pos.di = pr.i;
			pos.dj = pr.j;
			pos.index = i;
			wordmap.push_back(pos);

			xy.push_back((bool)dictwords[pr.i][pr.j].code);
			if (dictwords[pr.i][pr.j].code == lastFlag)
			{
				xxyy[xxyy.size() - 1].push_back(dictwords[pr.i][pr.j].code);
			}
			else
			{
				std::vector<bool> tempBool;
				tempBool.push_back(dictwords[pr.i][pr.j].code);
				lastFlag = (int)dictwords[pr.i][pr.j].code;
				xxyy.push_back(tempBool);
			}
		}
	}
}

//ͨ��Xx����������X������
size_t getxyInXY(int a, int b, std::vector< std::vector<bool> >& xxyy)
{
	int index = 0;
	for (int i = 0; i < a; ++i)
	{
		index += xxyy[i].size();
	}
	return size_t(index + b);
}

// flag: true ���� Qֵ�жϷ�ת  false:���� flag2 �ж�
// flag2 :true 0 -> 1 false:1->0
void reverseX(std::vector<bool>& X,
			  std::vector< std::vector<bool> >& Xx,
			  int& Q, int& P, int i, bool flag, bool flag2)
{
	std::cout << " ��תX" << std::endl;
	std::cout << " QX :" << Q <<  std::endl;
	std::cout << "��תǰX����"<<endl;
	writeCode(X);
	if(flag)
	{
		if (Q > 0)
		{
			std::cout << "  ���� 0->1 ��ת" << std::endl;
			transx0 ++;
			//0->1
			if (Xx[i + 1].front())
			{
				// 0011
				X[getxyInXY(i, int(Xx[i].size() - 1), Xx)] = true;
				Xx[i].pop_back();
				Xx[i + 1].push_back(true);
			}
			else
			{
				//1100
				X[getxyInXY(i + 1, 0, Xx)] = true;
				Xx[i].push_back(true);
				Xx[i + 1].pop_back();
			}
			--Q;
		}
		else
		{
			std::cout << "  ���� 1->0 ��ת" << std::endl;
			transx1 ++;
			//1 ->0
			if (Xx[i + 1][0])
			{
				// 0011
				X[getxyInXY(i + 1, 0, Xx)] = false;
				Xx[i].push_back(false);
				Xx[i + 1].pop_back();

			}
			else
			{
				//1100
				X[getxyInXY(i, Xx[i].size() - 1 , Xx)] = false;
				Xx[i].pop_back();
				Xx[i + 1].push_back(false);
			}
			++Q;
		}
	}
	else
	{
		if(flag2)
		{
			//0->1
			transx0 ++;
			std::cout << "  ���� 0->1 ��ת" << std::endl;
			//0->1
			if (Xx[i + 1].front())
			{
				// 0011
				X[getxyInXY(i, int(Xx[i].size() - 1), Xx)] = true;
				Xx[i].pop_back();
				Xx[i + 1].push_back(true);
			}
			else
			{
				//1100
				X[getxyInXY(i + 1, 0, Xx)] = true;
				Xx[i].push_back(true);
				Xx[i + 1].pop_back();
			}
			--Q;
		}
		else
		{
			//1->0
			transx1 ++;
			std::cout << "  ���� 1->0 ��ת" << std::endl;
			//1 ->0
			if (Xx[i + 1][0])
			{
				// 0011
				X[getxyInXY(i + 1, 0, Xx)] = false;
				Xx[i].push_back(false);
				Xx[i + 1].pop_back();

			}
			else
			{
				//1100
				X[getxyInXY(i, Xx[i].size() - 1, Xx)] = false;
				Xx[i].pop_back();
				Xx[i + 1].push_back(false);
			}
			++Q;
		}
	}
	--P;
	std::cout << " QX ֮�� :" << Q <<  std::endl;
	std::cout << "��ת֮��X����:" << endl;
	writeCode(X);
}

void reverseY(std::vector<bool>& Y,
			  std::vector< std::vector<bool> >& Yy,
			  int& Q, int& P, int i, bool flag, bool flag2)
{
	std::cout << " ��תY" << std::endl;
	std::cout << " QY :" << Q <<  std::endl;
	std::cout << "��תǰY����"<<endl;
	writeCode(Y);
	if(flag)
	{
		if (Q > 0)
		{
			//0->1
			std::cout << "  ���� 0->1 ��ת" << std::endl;
			transy0 ++;
			if (Yy[i + 1][0])
			{
				// 0011
				Y[getxyInXY(i, Yy[i].size() - 1, Yy)] = true;
				Yy[i].pop_back();
				Yy[i + 1].push_back(true);
			}
			else
			{
				//1100
				Y[getxyInXY(i + 1, 0, Yy)] = true;
				Yy[i].push_back(true);
				Yy[i + 1].pop_back();
			}
			--Q;
		}
		else
		{
			std::cout << "  ���� 1->0 ��ת" << std::endl;
			transy1 ++;
			//1 ->0
			if (Yy[i + 1][0])
			{
				// 0011
				Y[getxyInXY(i + 1, 0, Yy)] = false;
				Yy[i].push_back(false);
				Yy[i + 1].pop_back();

			}
			else
			{
				//1100
				Y[getxyInXY(i, (Yy[i].size() - 1), Yy)] = false;
				Yy[i].pop_back();
				Yy[i + 1].push_back(false);
			}
			++Q;
		}
	}
	else
	{
		if(flag2)
		{
			//0->1
			std::cout << "  ���� 0->1 ��ת" << std::endl;
			transy0 ++;
			if (Yy[i + 1][0])
			{
				// 0011
				Y[getxyInXY(i, Yy[i].size() - 1, Yy)] = true;
				Yy[i].pop_back();
				Yy[i + 1].push_back(true);
			}
			else
			{
				//1100
				Y[getxyInXY(i + 1, 0, Yy)] = true;
				Yy[i].push_back(true);
				Yy[i + 1].pop_back();
			}
			--Q;
		}
		else
		{
			//1 ->0
			std::cout << "  ���� 1->0 ��ת" << std::endl;
			transy1 ++;
			if (Yy[i + 1][0])
			{
				// 0011
				Y[getxyInXY(i + 1, 0,Yy)] = false;
				Yy[i].push_back(false);
				Yy[i + 1].pop_back();

			}
			else
			{
				//1100
				Y[getxyInXY(i, (Yy[i].size() - 1), Yy)] = false;
				Yy[i].pop_back();
				Yy[i + 1].push_back(false);
			}
			++Q;
		}
	}
	++P;
	std::cout << " QY ֮�� :" << Q <<  std::endl;
	std::cout << "��ת��Y����:" << endl;
	writeCode(Y);
}

//����
void process(std::vector< std::vector<bool> >& Xx,
			 std::vector< std::vector<bool> >& Yy,
			 std::vector<bool>& X,
			 std::vector<bool>& Y)
{
	// X.size > Y.size
	assert(Xx.size() >= Yy.size());
	assert(source_binary.size() < Yy.size());
	assert(source_binary.size() != 0);

	int P = 1;
	int Qx = 1;
	int Qy = 1;
	for (int i = 0; i < source_binary.size(); ++i)
	{
		if (parityCal(Xx[i], Yy[i]) == source_binary[i])
		{
			//ͬ��ż
			std::cout << "��" << (i + 1) << "�����룺ͬ��ż������" << std::endl;
			continue;
		}

		int K = 0;			//�γ� > 1�ĸ���
		int kx = 0;			//X�γ� > 1�ĸ���
		int ky = 0;			//Y�γ� > 1�ĸ���

		//�ж�Kֵ
		if (Xx[i].size() > 1)  ++kx; //i=1 �����2����  �ж� x1 x2  y1 y2
		if (Xx[i + 1].size() > 1) ++kx;
		if (Yy[i].size() > 1) ++ky;
		if (Yy[i + 1].size() > 1)  ++ky;

		K = kx + ky;
		std::cout << "��" << (i + 1) << "�����룺����K = " << K << std::endl;
		std::cout << "P:" << P << std::endl;
		switch (K)
		{
			case 4:
			{
				if (P > 0)
				{
					reverseX(X, Xx, Qx, P, i, true, true);
				}
				else
				{
					reverseY(Y, Yy, Qy, P, i, true, true);
				}
			}
				break;
			case 3:
			{
				//ѡ��k=2��һ��
				if (kx == 2)
				{
					reverseX(X, Xx, Qx, P, i, true, true);
				}
				else
				{
					reverseY(Y, Yy, Qy,P, i, true, true);
				}
			}
				break;
			case 2:
			{
				if(kx == 2)
				{
					//����X����
					reverseX(X, Xx, Qx, P, i, true, true);
				}
				else if(ky == 2)
				{
					reverseY(Y, Yy, Qy, P, i, true, true);
				}
				else
				{
					int kymax = Yy[i].size() > Yy[i + 1].size() ? Yy[i].size() : Yy[i+ 1].size();
					int kxmax = Xx[i].size() > Xx[i + 1].size() ? Xx[i].size() : Xx[i+ 1].size();
					
					//if(Yy[i].size() > Xx[i].size() || Yy[i+ 1].size() > Xx[i].size() || Yy[i].size() > Xx[i + 1].size() || Yy[i +1].size() > Xx[ i+ 1].size())
					if(kymax > kxmax)
					{
						//��ת Y
						if(Yy[i].size() > 1 && !Yy[i].front() || Yy[i + 1].size() > 1 && !Yy[i + 1].front())
							// 001  100 0 -> 1
						reverseY(Y, Yy, Qy, P, i, false, true);
						else
							// 1 -> 0
						reverseY(Y, Yy, Qy, P, i, false, false);
					}
					else
					{
						if(Xx[i].size() > 1 && !Xx[i].front() || Xx[i + 1].size() > 1 && !Xx[i + 1].front())
						reverseX(X, Xx, Qx, P, i, false, true);
						else
						reverseX(X, Xx, Qx, P, i, false, false);
					}
					
				}
			}
				break;
			case 1:
			{
				if (kx == 1)
				{
					if(Xx[i].size() > 1 && !Xx[i].front() || Xx[i + 1].size() > 1 && !Xx[i].front()) 
						reverseX(X, Xx, Qx, P, i, false, true);
					else
						reverseX(X, Xx, Qx, P, i, false, false);
					
				}
				else
				{
					if(Yy[i].size() > 1 && !Yy[i].front() || Yy[i + 1].size() > 1 && !Yy[i].front()) 
						reverseY(Y, Yy, Qy, P, i, false, true);
					else
						reverseY(Y, Yy, Qy, P, i, false, false);
					
				}
			}
				break;
			case 0:
			{
				X[getxyInXY(i + 1, 0, Xx)] = !X[getxyInXY(i + 1, 0, Xx)];
				Y[getxyInXY(i + 1, 0, Yy)] = !Y[getxyInXY(i + 1, 0, Yy)];
				Xx[i].push_back(Xx[i + 1][0]);
				Yy[i].push_back(Yy[i + 1][0]);
				if (i != source_binary.size() - 1)
				{
					//û�е�β�� �ϲ�x+2  y+2 �� x y��
					Xx[i].insert(Xx[i].end(), Xx[i + 2].begin(), Xx[i + 2].end());
					Yy[i].insert(Yy[i].end(), Yy[i + 2].begin(), Yy[i + 2].end());
					i += 2; //��������
				}
			}
				break;
		} //end switch
		std::cout << "P ֮��:" << P << std::endl;
	}//end for
}

//����ת����ı���ָ��ı�
void restoreWord(std::vector<bool>& old, 
				 std::vector<bool>& news,
				 std::vector<Pos>& wordmap,
				 int zu,
				 std::string fileName,
				 std::vector<std::string>& words)
{

	for (int i = 0; i < news.size(); ++i)
	{
		if(news[i] != old[i]) std::cout << fileName << "��" << dictwords[wordmap[i].di][old[i]].str << " �滻�� " << dictwords[wordmap[i].di][news[i]].str << std::endl;
		words[wordmap[i].index] = dictwords[wordmap[i].di][news[i]].str;
	}

	std::string o;
	for (int ii = 0; ii < words.size(); ++ii)
	{
		o += words[ii] + " ";
	}
	std::string strname;
	char t[256];
	std::string ts;
	sprintf(t, "%d", zu);
	ts = t;
	strname = "���//after" + ts + "_" + fileName;
	ofstream outfile(strname.c_str());
	outfile << o ;
	outfile.close();
	
}

int main()
{

	/////////////////////////////////////////////////////////////////////////////////////////////////

	std::cout << "����ͬ��ʴʵ�...." << std::endl;
	LoadWordLib();
	std::cout << "����ͬ��ʴʵ����" << std::endl;

	//����Ҫ���ܵĶ�����
	std::cout << "�����������λ��" << std::endl;
	int sourceSize;
	std::cin >> sourceSize;
	randomBinary(sourceSize);
	storeBindary();

	//LoadBindary("source_binary.txt");
	//std::cout << "����Ҫ���ܵ��ı�����" << std::endl;

	vector<string> files;
	files=getfilename("ԭʼ\\");
	ofstream errorfile("failedTxt.txt");
	errorfile << std::endl;
	errorfile.close();
	int zu = 0;

		ofstream res1("ͳ�ƽ��//��תǰ��Ƶ��ռ�����ı�������.txt");
		ofstream res2("ͳ�ƽ��//��תǰ��Ƶ��ռ�����ı�������.txt");
		ofstream res3("ͳ�ƽ��//��תǰ��Ƶ��ռ�ı�1������.txt");
		ofstream res4("ͳ�ƽ��//��תǰ��Ƶ��ռ�ı�2������.txt");
		ofstream res5("ͳ�ƽ��//��תǰ��Ƶ��ռ�ı�1������.txt");
		ofstream res6("ͳ�ƽ��//��תǰ��Ƶ��ռ�ı�2������.txt");
		ofstream res7("ͳ�ƽ��//��ת���Ƶ��ռ�����ı�������.txt");
		ofstream res8("ͳ�ƽ��//��ת���Ƶ��ռ�����ı�������.txt");
		ofstream res9("ͳ�ƽ��//��ת���Ƶ��ռ�ı�1������.txt");
		ofstream res10("ͳ�ƽ��//��ת���Ƶ��ռ�ı�2������.txt");
		ofstream res11("ͳ�ƽ��//��ת���Ƶ��ռ�ı�1������.txt");
		ofstream res12("ͳ�ƽ��//��ת���Ƶ��ռ�ı�2������.txt");
		ofstream res13("ͳ�ƽ��//Ƕ��Ч��.txt");
		ofstream res14("ͳ�ƽ��//Ƕ����.txt");

	for(int i = 0; i < files.size(); ++i)
	{
		ofstream log(("�м���//"+files[i] + ".log").c_str());
		std::cout << "����" << i + 1 << "���ı���"<< files[i] << std::endl;
		std::vector<bool> X;
		std::vector< std::vector<bool> > Xx;
		std::vector< std::string > X_words;	
		std::vector<Pos> X_wordmap;
		codeParagraph("ԭʼ\\" + files[i], X_words, X, Xx, X_wordmap);
		
		

		std::vector<bool> oldX = X;

		//�ж��Ƿ�ϸ�
		if(Xx.size() <= source_binary.size())
		{
			ofstream errorfile("failedTxt.txt", ios::app);
			errorfile << files[i] << endl;
			errorfile.close();
			std::cout << "���ؿռ�Ϊ��"<< Xx.size() << endl;
			std::cout << "���ؿռ䲻�����ϸ�" << endl;
			continue;
		}
		log<< "������Ϣ��"<< transforCode(source_binary)<<endl;
		log << "X����Ϊ:"<< transforCode(X) << endl;
		log << "Xx����:"<< transforXxYyCode(Xx) << endl;
		log <<"1�ĸ���"<< getcodenum(X, true)<<endl;
		log << "0�ĸ���"<< getcodenum(X,false)<<endl;
		log<<"X����"<< getcodenum(X, true) + getcodenum(X, false)<< endl;

		std::vector<bool> Y;
		std::vector< std::vector<bool> > Yy;
		std::vector< std::string > Y_words;	
		std::vector<Pos> Y_wordmap;
		std::vector<bool> oldY;

		int xi = i;
		int j = ++i;
		for( ; j < files.size(); ++j,++i)
		{
			Y.clear();
			Yy.clear();
			Y_words.clear();
			Y_wordmap.clear();
			oldY.clear();
			//���ڶ���
			std::cout << "����" << j + 1 << "���ı���"<< files[j] << std::endl;
			codeParagraph("ԭʼ\\" + files[i], Y_words, Y, Yy, Y_wordmap);

			 oldY = Y;

			//�ж��Ƿ�ϸ�
			if(Yy.size()   <= source_binary.size())
			{
				ofstream errorfile("failedTxt.txt", ios::app);
				errorfile << files[j] << endl;
				errorfile.close();
				std::cout << "���ؿռ�Ϊ��"<< Yy.size() << endl;
				std::cout << "���ؿռ䲻�����ϸ�" << endl;
				continue;
			}

			//��֮��Ӧ���ļ�
			log<<endl;
			log << "��Ӧ���ļ�" << files[j] <<endl;
			log << "Y����Ϊ:"<< transforCode(Y) << endl;
			log << "Yy����:"<< transforXxYyCode(Yy) << endl;
			log<<"1�ĸ���"<< getcodenum(Y, true)<<endl;
			log << "0�ĸ���"<< getcodenum(Y,false)<<endl;
			log<<"Y����"<< getcodenum(Y, true) + getcodenum(Y, false)<< endl;

			//�ϸ��������
			break;
		}

		if(Yy.size() == 0)
		{
			//����
			break;
		}
		
		//�ж�xy�Ƿ��Ӧ
		transx0 = 0;
		transx1 = 0;
		transy0 = 0;
		transy1 = 0;
	
		if(Yy.size() > Xx.size())
		{
			process(Yy, Xx, Y, X);
		}
		else
		{
			process(Xx, Yy, X, Y);
		}
		log<< endl;

		
		log << "��ת��X����Ϊ:"<< transforCode(X) << endl;
		log << "��ת��Xx����:"<< transforXxYyCode(Xx) << endl;
		log<< "1�ĸ���"<< getcodenum(X, true)<<endl;
		log << "0�ĸ���"<< getcodenum(X,false)<<endl;
		log<< "X��0��ת"<<transx0 << "��" <<endl;
		log<< "X��1��ת"<<transx1 << "��" <<endl;
		log<<"X�й���ת" << transx0 + transx1 << "��"<<endl;

		log<< endl;

		log << "��ת��Y����Ϊ:"<< transforCode(Y) << endl;
		log << "��ת��Yy����:"<< transforXxYyCode(Yy) << endl;
		log<< "1�ĸ���"<< getcodenum(Y, true)<<endl;
		log << "0�ĸ���"<< getcodenum(Y,false)<<endl;
		log<< "Y��0��ת"<<transy0 << "��" <<endl;
		log<< "Y��1��ת"<<transy1 << "��" <<endl;
		log<<"Y�й���ת" << transy0 + transy1 << "��"<<endl;
		log<< endl;

		log<< "��תǰX��1�ĸ���ռX�ı��� :" << (double)getcodenum(oldX,true) / X.size()<<endl;
		log<< "��תǰX��0�ĸ���ռX�ı��� :" << (double)getcodenum(oldX,false) / X.size()<< endl;
		log<< "��תǰY��1�ĸ���ռX�ı��� :" << (double)getcodenum(oldY,true) / Y.size()<<endl;
		log<< "��תǰY��0�ĸ���ռX�ı��� :" << (double)getcodenum(oldY,false) / Y.size()<<endl;

			log<< endl;
		
		log << "��תǰX��Y��1�ĸ����ϼ�:" << getcodenum(oldX, true) + getcodenum(oldY, true) << endl;
		log << "��תǰX��Y��0�ĸ����ϼ�:" << getcodenum(oldX, false) + getcodenum(oldY, false) << endl;
		log << "��תǰX��Y��1ռ�����ı���:" << double(getcodenum(oldX, true) + getcodenum(oldY, true))/ (X.size() + Y.size()) << endl;
		log << "��תǰX��Y��0ռ�����ı���:" << double(getcodenum(oldX, false) + getcodenum(oldY, false))/ (X.size() + Y.size()) << endl;
			log<< endl;
		
		log << "��ת��X��1�ĸ���ռX�ı���:"<< double(getcodenum(X, true))/X.size()<<endl;
		log<< "��ת��X��0�ĸ���ռX�ı��� :" << (double)getcodenum(X,false) / X.size()<< endl;
		log<< "��ת��Y��1�ĸ���ռX�ı��� :" << (double)getcodenum(Y,true) / Y.size()<<endl;
		log<< "��ת��Y��0�ĸ���ռX�ı��� :" << (double)getcodenum(Y,false) / Y.size()<<endl;
		log<< endl;

		log << "��ת��X��Y��1�ĸ����ϼ�:" << getcodenum(X, true) + getcodenum(Y, true) << endl;
		log << "��ת��X��Y��0�ĸ����ϼ�:" << getcodenum(X, false) + getcodenum(Y, false) << endl;
		log << "��ת��X��Y��1ռ�����ı���:" << double(getcodenum(X, true) + getcodenum(Y, true))/ (X.size() + Y.size()) << endl;
		log << "��ת��X��Y��0ռ�����ı���:" << double(getcodenum(X, false) + getcodenum(Y, false))/ (X.size() + Y.size()) << endl;
			log<< endl;
		
		log << "X�ܹ���ת" << transx0 +transx1 << "��"<<endl;
		log << "Y�ܹ���ת" << transy0 +transy1 << "��"<<endl;
		log << "X��Y�ϼƷ�ת��"<< transx0 +transx1 + transy0 +transy1 << endl;
		log << "Ƕ��Ч�ʣ�" << double(source_binary.size())/(transx0+ transx1 + transy0 +transy1)<<endl;
		log << "Ƕ���ʣ�"<< double(source_binary.size())/(X.size() + Y.size())<<endl;

		//ͳ����Ϣ
		//����ע�͵��ڶ�Ӧ�ı�����ʾ����ʾ���������ı�ʱһ���
		/*  
		     res1 << "�ļ�X��" << files[xi] << endl;
		     res1 << "�ļ�Y��" << files[j] << endl;
		*/


		/*
		res1 << "��תǰX��Y��1ռ�����ı���:"<<double(getcodenum(oldX, true) + getcodenum(oldY, true))/ (X.size() + Y.size()) << endl;
		res2 << "��תǰX��Y��0ռ�����ı���:" << double(getcodenum(oldX, false) + getcodenum(oldY, false))/ (X.size() + Y.size()) << endl;
		res3 << "��תǰX��1�ĸ���ռX�ı��� :" << (double)getcodenum(oldX,true) / X.size()<<endl;
		res4 <<	"��תǰY��1�ĸ���ռY�ı��� :" << (double)getcodenum(oldY,true) / Y.size()<<endl;
		res5 <<	"��תǰX��0�ĸ���ռX�ı��� :" << (double)getcodenum(oldX,false) / X.size()<< endl;
		res6 <<	"��תǰY��0�ĸ���ռY�ı��� :" << (double)getcodenum(oldY,false) / Y.size()<<endl;
		res7 <<	"��ת��X��Y��1ռ�����ı���:" << double(getcodenum(X, true) + getcodenum(Y, true))/ (X.size() + Y.size()) << endl;
		res8 << "��ת��X��Y��0ռ�����ı���:" << double(getcodenum(X, false) + getcodenum(Y, false))/ (X.size() + Y.size()) << endl;
		res9 << "��ת��X��1�ĸ���ռX�ı��� :" << (double)getcodenum(X,true) / X.size()<<endl;
		res10 << "��ת��Y��1�ĸ���ռY�ı��� :" << (double)getcodenum(Y,true) / Y.size()<<endl;
		res11 << "��ת��X��0�ĸ���ռX�ı��� :" << (double)getcodenum(X,false) / X.size()<< endl;
		res12 << "��ת��Y��0�ĸ���ռY�ı��� :" << (double)getcodenum(Y,false) / Y.size()<<endl;
		res13<<  "Ƕ��Ч�ʣ�" << double(source_binary.size())/(transx0+ transx1 + transy0 +transy1)<<endl;
		res14<<  "Ƕ���ʣ�"<< double(source_binary.size())/(X.size() + Y.size())<<endl;
	
		˵�����˴������治ͬ�Ľ��������ı��в��������תǰX��Y��1ռ�����ı�����֮��ĺ��֣�����ֱ��������ݣ�
		*/
		res1 <<double(getcodenum(oldX, true) + getcodenum(oldY, true))/ (X.size() + Y.size()) << endl;
		res2 << double(getcodenum(oldX, false) + getcodenum(oldY, false))/ (X.size() + Y.size()) << endl;
		res3 << (double)getcodenum(oldX,true) / X.size()<<endl;
		res4  << (double)getcodenum(oldY,true) / Y.size()<<endl;
		res5 << (double)getcodenum(oldX,false) / X.size()<< endl;
		res6  << (double)getcodenum(oldY,false) / Y.size()<<endl;
		res7  << double(getcodenum(X, true) + getcodenum(Y, true))/ (X.size() + Y.size()) << endl;
		res8  << double(getcodenum(X, false) + getcodenum(Y, false))/ (X.size() + Y.size()) << endl;
		res9  << (double)getcodenum(X,true) / X.size()<<endl;
		res10  << (double)getcodenum(Y,true) / Y.size()<<endl;
		res11 << (double)getcodenum(X,false) / X.size()<< endl;
		res12 << (double)getcodenum(Y,false) / Y.size()<<endl;
		res13<< double(source_binary.size())/(transx0+ transx1 + transy0 +transy1)<<endl;
		res14<< double(source_binary.size())/(X.size() + Y.size())<<endl;

		
		log.close();
		++zu;
		restoreWord(oldX, X, X_wordmap, zu, files[xi], X_words);
		restoreWord(oldY, Y, Y_wordmap, zu, files[j], Y_words);
	}

		res1.close();
		res2.close();
		res3.close();
		res4.close();
		res5.close();
		res6.close();
		res7.close();
		res8.close();
		res9.close();
		res10.close();
		res11.close();
		res12.close();
		res13.close();
		res14.close();
		


	system("pause");
	return 0;
}