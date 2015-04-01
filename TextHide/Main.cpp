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
	string str;			//同义词字符串
	bool code;			//同义词的编码
	int  index;
};

//记录查找的返回位置
struct position
{
	int i;
	int j;
};			

struct Pos
{
	int index;				//在文章中的位置
	int di;
	int dj;					//载体文在在同义词字典中的位置
};

vector< vector<Syword> > dictwords;								//长度为2的同义词组	
map< string, Syword>	dictMap;
std::vector<bool> source_binary;								//要加密的二进制
int transx0;
int transx1;
int transy0;
int transy1;

//打印用的
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

//获取文件夹内所有的文件名
vector<string> getfilename(string filepath)
{  
	vector<string> filename;
	_finddata_t file;  
    long lf;  
    //输入文件夹路径  
	filepath=filepath+"*.*";
    if((lf = _findfirst(filepath.data(), &file))==-1)  
        cout<<"Not Found!"<<endl;  
    else{  
        //输出文件名  
  
        while(_findnext( lf, &file)==0){ 
			filename.push_back(file.name);
        }  
    }	
    _findclose(lf);	
	return filename;
} 

//载入同义词库并且找出个数为2的同义词
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

//加载要加密的二进制
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

//字符分割 不用去理解  就是把 文章一个单词一个单词分开存数组用的
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

//奇偶运算
bool parityCal(std::vector<bool> left, std::vector<bool> right)
{
	assert(left.size());
	assert(right.size());
	return  !(left.size() % 2 == right.size() % 2);
}

//在载体文本中找同义词
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

//加自然段通过同义词编码成二进制
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

//通过Xx的坐标找在X的索引
size_t getxyInXY(int a, int b, std::vector< std::vector<bool> >& xxyy)
{
	int index = 0;
	for (int i = 0; i < a; ++i)
	{
		index += xxyy[i].size();
	}
	return size_t(index + b);
}

// flag: true 根据 Q值判断翻转  false:根据 flag2 判断
// flag2 :true 0 -> 1 false:1->0
void reverseX(std::vector<bool>& X,
			  std::vector< std::vector<bool> >& Xx,
			  int& Q, int& P, int i, bool flag, bool flag2)
{
	std::cout << " 翻转X" << std::endl;
	std::cout << " QX :" << Q <<  std::endl;
	std::cout << "翻转前X编码"<<endl;
	writeCode(X);
	if(flag)
	{
		if (Q > 0)
		{
			std::cout << "  进行 0->1 翻转" << std::endl;
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
			std::cout << "  进行 1->0 翻转" << std::endl;
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
			std::cout << "  进行 0->1 翻转" << std::endl;
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
			std::cout << "  进行 1->0 翻转" << std::endl;
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
	std::cout << " QX 之后 :" << Q <<  std::endl;
	std::cout << "翻转之后X编码:" << endl;
	writeCode(X);
}

void reverseY(std::vector<bool>& Y,
			  std::vector< std::vector<bool> >& Yy,
			  int& Q, int& P, int i, bool flag, bool flag2)
{
	std::cout << " 翻转Y" << std::endl;
	std::cout << " QY :" << Q <<  std::endl;
	std::cout << "翻转前Y编码"<<endl;
	writeCode(Y);
	if(flag)
	{
		if (Q > 0)
		{
			//0->1
			std::cout << "  进行 0->1 翻转" << std::endl;
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
			std::cout << "  进行 1->0 翻转" << std::endl;
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
			std::cout << "  进行 0->1 翻转" << std::endl;
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
			std::cout << "  进行 1->0 翻转" << std::endl;
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
	std::cout << " QY 之后 :" << Q <<  std::endl;
	std::cout << "翻转后Y编码:" << endl;
	writeCode(Y);
}

//处理
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
			//同奇偶
			std::cout << "第" << (i + 1) << "个编码：同奇偶，跳过" << std::endl;
			continue;
		}

		int K = 0;			//游程 > 1的个数
		int kx = 0;			//X游程 > 1的个数
		int ky = 0;			//Y游程 > 1的个数

		//判断K值
		if (Xx[i].size() > 1)  ++kx; //i=1 处理第2编码  判断 x1 x2  y1 y2
		if (Xx[i + 1].size() > 1) ++kx;
		if (Yy[i].size() > 1) ++ky;
		if (Yy[i + 1].size() > 1)  ++ky;

		K = kx + ky;
		std::cout << "第" << (i + 1) << "个编码：处理K = " << K << std::endl;
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
				//选择k=2的一方
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
					//落在X序列
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
						//翻转 Y
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
					//没有到尾部 合并x+2  y+2 到 x y中
					Xx[i].insert(Xx[i].end(), Xx[i + 2].begin(), Xx[i + 2].end());
					Yy[i].insert(Yy[i].end(), Yy[i + 2].begin(), Yy[i + 2].end());
					i += 2; //跳过两个
				}
			}
				break;
		} //end switch
		std::cout << "P 之后:" << P << std::endl;
	}//end for
}

//根据转换后的编码恢复文本
void restoreWord(std::vector<bool>& old, 
				 std::vector<bool>& news,
				 std::vector<Pos>& wordmap,
				 int zu,
				 std::string fileName,
				 std::vector<std::string>& words)
{

	for (int i = 0; i < news.size(); ++i)
	{
		if(news[i] != old[i]) std::cout << fileName << "的" << dictwords[wordmap[i].di][old[i]].str << " 替换成 " << dictwords[wordmap[i].di][news[i]].str << std::endl;
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
	strname = "结果//after" + ts + "_" + fileName;
	ofstream outfile(strname.c_str());
	outfile << o ;
	outfile.close();
	
}

int main()
{

	/////////////////////////////////////////////////////////////////////////////////////////////////

	std::cout << "载入同义词词典...." << std::endl;
	LoadWordLib();
	std::cout << "载入同义词词典结束" << std::endl;

	//载入要加密的二进制
	std::cout << "请输入二进制位数" << std::endl;
	int sourceSize;
	std::cin >> sourceSize;
	randomBinary(sourceSize);
	storeBindary();

	//LoadBindary("source_binary.txt");
	//std::cout << "载入要加密的文本结束" << std::endl;

	vector<string> files;
	files=getfilename("原始\\");
	ofstream errorfile("failedTxt.txt");
	errorfile << std::endl;
	errorfile.close();
	int zu = 0;

		ofstream res1("统计结果//翻转前低频词占两个文本总数比.txt");
		ofstream res2("统计结果//翻转前高频词占两个文本总数比.txt");
		ofstream res3("统计结果//翻转前低频词占文本1总数比.txt");
		ofstream res4("统计结果//翻转前低频词占文本2总数比.txt");
		ofstream res5("统计结果//翻转前高频词占文本1总数比.txt");
		ofstream res6("统计结果//翻转前高频词占文本2总数比.txt");
		ofstream res7("统计结果//翻转后低频词占两个文本总数比.txt");
		ofstream res8("统计结果//翻转后高频词占两个文本总数比.txt");
		ofstream res9("统计结果//翻转后低频词占文本1总数比.txt");
		ofstream res10("统计结果//翻转后低频词占文本2总数比.txt");
		ofstream res11("统计结果//翻转后高频词占文本1总数比.txt");
		ofstream res12("统计结果//翻转后高频词占文本2总数比.txt");
		ofstream res13("统计结果//嵌入效率.txt");
		ofstream res14("统计结果//嵌入率.txt");

	for(int i = 0; i < files.size(); ++i)
	{
		ofstream log(("中间结果//"+files[i] + ".log").c_str());
		std::cout << "读第" << i + 1 << "个文本："<< files[i] << std::endl;
		std::vector<bool> X;
		std::vector< std::vector<bool> > Xx;
		std::vector< std::string > X_words;	
		std::vector<Pos> X_wordmap;
		codeParagraph("原始\\" + files[i], X_words, X, Xx, X_wordmap);
		
		

		std::vector<bool> oldX = X;

		//判断是否合格
		if(Xx.size() <= source_binary.size())
		{
			ofstream errorfile("failedTxt.txt", ios::app);
			errorfile << files[i] << endl;
			errorfile.close();
			std::cout << "隐藏空间为："<< Xx.size() << endl;
			std::cout << "隐藏空间不够不合格" << endl;
			continue;
		}
		log<< "秘密信息："<< transforCode(source_binary)<<endl;
		log << "X编码为:"<< transforCode(X) << endl;
		log << "Xx编码:"<< transforXxYyCode(Xx) << endl;
		log <<"1的个数"<< getcodenum(X, true)<<endl;
		log << "0的个数"<< getcodenum(X,false)<<endl;
		log<<"X总数"<< getcodenum(X, true) + getcodenum(X, false)<< endl;

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
			//读第二个
			std::cout << "读第" << j + 1 << "个文本："<< files[j] << std::endl;
			codeParagraph("原始\\" + files[i], Y_words, Y, Yy, Y_wordmap);

			 oldY = Y;

			//判断是否合格
			if(Yy.size()   <= source_binary.size())
			{
				ofstream errorfile("failedTxt.txt", ios::app);
				errorfile << files[j] << endl;
				errorfile.close();
				std::cout << "隐藏空间为："<< Yy.size() << endl;
				std::cout << "隐藏空间不够不合格" << endl;
				continue;
			}

			//与之对应的文件
			log<<endl;
			log << "对应的文件" << files[j] <<endl;
			log << "Y编码为:"<< transforCode(Y) << endl;
			log << "Yy编码:"<< transforXxYyCode(Yy) << endl;
			log<<"1的个数"<< getcodenum(Y, true)<<endl;
			log << "0的个数"<< getcodenum(Y,false)<<endl;
			log<<"Y总数"<< getcodenum(Y, true) + getcodenum(Y, false)<< endl;

			//合格就跳出来
			break;
		}

		if(Yy.size() == 0)
		{
			//不够
			break;
		}
		
		//判断xy是否对应
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

		
		log << "翻转后X编码为:"<< transforCode(X) << endl;
		log << "翻转后Xx编码:"<< transforXxYyCode(Xx) << endl;
		log<< "1的个数"<< getcodenum(X, true)<<endl;
		log << "0的个数"<< getcodenum(X,false)<<endl;
		log<< "X中0翻转"<<transx0 << "次" <<endl;
		log<< "X中1翻转"<<transx1 << "次" <<endl;
		log<<"X中共翻转" << transx0 + transx1 << "次"<<endl;

		log<< endl;

		log << "翻转后Y编码为:"<< transforCode(Y) << endl;
		log << "翻转后Yy编码:"<< transforXxYyCode(Yy) << endl;
		log<< "1的个数"<< getcodenum(Y, true)<<endl;
		log << "0的个数"<< getcodenum(Y,false)<<endl;
		log<< "Y中0翻转"<<transy0 << "次" <<endl;
		log<< "Y中1翻转"<<transy1 << "次" <<endl;
		log<<"Y中共翻转" << transy0 + transy1 << "次"<<endl;
		log<< endl;

		log<< "翻转前X中1的个数占X的比列 :" << (double)getcodenum(oldX,true) / X.size()<<endl;
		log<< "翻转前X中0的个数占X的比列 :" << (double)getcodenum(oldX,false) / X.size()<< endl;
		log<< "翻转前Y中1的个数占X的比列 :" << (double)getcodenum(oldY,true) / Y.size()<<endl;
		log<< "翻转前Y中0的个数占X的比列 :" << (double)getcodenum(oldY,false) / Y.size()<<endl;

			log<< endl;
		
		log << "翻转前X与Y中1的个数合计:" << getcodenum(oldX, true) + getcodenum(oldY, true) << endl;
		log << "翻转前X与Y中0的个数合计:" << getcodenum(oldX, false) + getcodenum(oldY, false) << endl;
		log << "翻转前X与Y中1占总数的比列:" << double(getcodenum(oldX, true) + getcodenum(oldY, true))/ (X.size() + Y.size()) << endl;
		log << "翻转前X与Y中0占总数的比列:" << double(getcodenum(oldX, false) + getcodenum(oldY, false))/ (X.size() + Y.size()) << endl;
			log<< endl;
		
		log << "翻转后X中1的个数占X的比列:"<< double(getcodenum(X, true))/X.size()<<endl;
		log<< "翻转后X中0的个数占X的比列 :" << (double)getcodenum(X,false) / X.size()<< endl;
		log<< "翻转后Y中1的个数占X的比列 :" << (double)getcodenum(Y,true) / Y.size()<<endl;
		log<< "翻转后Y中0的个数占X的比列 :" << (double)getcodenum(Y,false) / Y.size()<<endl;
		log<< endl;

		log << "翻转后X与Y中1的个数合计:" << getcodenum(X, true) + getcodenum(Y, true) << endl;
		log << "翻转后X与Y中0的个数合计:" << getcodenum(X, false) + getcodenum(Y, false) << endl;
		log << "翻转后X与Y中1占总数的比列:" << double(getcodenum(X, true) + getcodenum(Y, true))/ (X.size() + Y.size()) << endl;
		log << "翻转后X与Y中0占总数的比列:" << double(getcodenum(X, false) + getcodenum(Y, false))/ (X.size() + Y.size()) << endl;
			log<< endl;
		
		log << "X总共翻转" << transx0 +transx1 << "次"<<endl;
		log << "Y总共翻转" << transy0 +transy1 << "次"<<endl;
		log << "X与Y合计翻转了"<< transx0 +transx1 + transy0 +transy1 << endl;
		log << "嵌入效率：" << double(source_binary.size())/(transx0+ transx1 + transy0 +transy1)<<endl;
		log << "嵌入率："<< double(source_binary.size())/(X.size() + Y.size())<<endl;

		//统计信息
		//下面注释的在对应文本中显示不显示是哪两个文本时一组的
		/*  
		     res1 << "文件X：" << files[xi] << endl;
		     res1 << "文件Y：" << files[j] << endl;
		*/


		/*
		res1 << "翻转前X与Y中1占总数的比例:"<<double(getcodenum(oldX, true) + getcodenum(oldY, true))/ (X.size() + Y.size()) << endl;
		res2 << "翻转前X与Y中0占总数的比列:" << double(getcodenum(oldX, false) + getcodenum(oldY, false))/ (X.size() + Y.size()) << endl;
		res3 << "翻转前X中1的个数占X的比列 :" << (double)getcodenum(oldX,true) / X.size()<<endl;
		res4 <<	"翻转前Y中1的个数占Y的比列 :" << (double)getcodenum(oldY,true) / Y.size()<<endl;
		res5 <<	"翻转前X中0的个数占X的比列 :" << (double)getcodenum(oldX,false) / X.size()<< endl;
		res6 <<	"翻转前Y中0的个数占Y的比列 :" << (double)getcodenum(oldY,false) / Y.size()<<endl;
		res7 <<	"翻转后X与Y中1占总数的比列:" << double(getcodenum(X, true) + getcodenum(Y, true))/ (X.size() + Y.size()) << endl;
		res8 << "翻转后X与Y中0占总数的比列:" << double(getcodenum(X, false) + getcodenum(Y, false))/ (X.size() + Y.size()) << endl;
		res9 << "翻转后X中1的个数占X的比列 :" << (double)getcodenum(X,true) / X.size()<<endl;
		res10 << "翻转后Y中1的个数占Y的比列 :" << (double)getcodenum(Y,true) / Y.size()<<endl;
		res11 << "翻转后X中0的个数占X的比列 :" << (double)getcodenum(X,false) / X.size()<< endl;
		res12 << "翻转后Y中0的个数占Y的比列 :" << (double)getcodenum(Y,false) / Y.size()<<endl;
		res13<<  "嵌入效率：" << double(source_binary.size())/(transx0+ transx1 + transy0 +transy1)<<endl;
		res14<<  "嵌入率："<< double(source_binary.size())/(X.size() + Y.size())<<endl;
	
		说明：此处于下面不同的仅仅是在文本中不输出‘翻转前X与Y中1占总数的比例’之类的汉字，而是直接输出数据；
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