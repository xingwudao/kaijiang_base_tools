#include "CPattMatchMachine.h"
#include <iostream>
#include <queue>
using namespace std;
using namespace kaijiang;

// 匹配信息的构造函数
kaijiang::MATCH_INFO::MATCH_INFO()
{
	num = 0;
}

//调试用的函数
void kaijiang::CPattMatchMachine::PrintNode(PATTERN_TREE_NODE* pNode, int l)
{
	cout<<"leval:"<<l<<endl;
	cout<<"children count:"<<pNode->nChildrenCount<<endl;
	for(int i = 0; i < MAX_CELL_SIZE; i++)
	{
		if(pNode->pChildrenNode[i] != NULL)
		{
			cout<<(char)i<<":"<<endl;
			PrintNode(pNode->pChildrenNode[i], l+1);
		}
	}
}

// 默认构造函数
kaijiang::CPattMatchMachine::CPattMatchMachine()
{
	m_pPatternTreeRoot = new PATTERN_TREE_NODE;
	m_bPrepared = false;
}

// 从文件读入模式串构造
kaijiang::CPattMatchMachine::CPattMatchMachine(const char* cPatternFile)
{
	m_pPatternTreeRoot = new PATTERN_TREE_NODE;
	m_bPrepared = false;
	AddPattern(cPatternFile);
}

// 复制构造函数
kaijiang::CPattMatchMachine::CPattMatchMachine(const kaijiang::CPattMatchMachine& other)
{
	for(unsigned int i = 0; i < other.m_vecPattern.size(); i++)
	{
		string p = other.m_vecPattern[i];
		AddPattern(p);
	}
	if(other.m_bPrepared)
	  PrepareMachine();
}

// 赋值操作符
kaijiang::CPattMatchMachine& kaijiang::CPattMatchMachine::operator=(const kaijiang::CPattMatchMachine& other)
{
	if(&other == this)
	  return *this;
	delete m_pPatternTreeRoot;
	m_pPatternTreeRoot = new PATTERN_TREE_NODE; 
	m_vecPattern.clear();
	m_bPrepared = false;
	for(unsigned int i = 0; i < other.m_vecPattern.size(); i++)
	{
		string p = other.m_vecPattern[i];
		this->AddPattern(p);
	}
	if(other.m_bPrepared)
	  this->PrepareMachine();

	return *this;
}

// 析构函数
kaijiang::CPattMatchMachine::~CPattMatchMachine()
{
	delete m_pPatternTreeRoot;
}

// 添加一个模式串
bool kaijiang::CPattMatchMachine::AddPattern(string& strPattern)
{
	if(strPattern.size() == 0)
	  return false;
	PATTERN_TREE_NODE* pCurrent = m_pPatternTreeRoot;
	PATTERN_TREE_NODE* pNode;
	for(unsigned int index = 0; index != strPattern.size(); index++)
	{
		unsigned char pos = (unsigned char)strPattern.at(index);
		pNode = pCurrent->pChildrenNode[pos];
		if(pNode == NULL)
		{
			pNode = new PATTERN_TREE_NODE;
			pCurrent->pChildrenNode[pos] = pNode;
			pCurrent->nChildrenCount++;
		}
		pCurrent = pNode;
	}
	if(pCurrent->nPatternId == -1)
	{
		pCurrent->nPatternId = m_vecPattern.size();
		m_vecPattern.push_back(strPattern);
	}
	m_bPrepared = false;
	return true;
}

// 从文件中追加模式串
int kaijiang::CPattMatchMachine::AddPattern(const char* cPatternFile)
{
	ifstream fin(cPatternFile);
	if(!fin.is_open())
	{
		cerr<<cPatternFile<<"can not be opened."<<endl;
		return 0;
	}
	int count = GetPatternCount();
	string line;
	while(!fin.eof())
	{
		getline(fin, line);
		if(line.size() == 0)
		  continue;
		AddPattern(line);
	}
	fin.close();
	PrepareMachine();

	return GetPatternCount() - count;
}

// 更新每个节点的失败指针
void kaijiang::CPattMatchMachine::PrepareMachine()
{
	if(m_vecPattern.size() == 0)
	  return;
	// 根节点下面的子节点失败指针都是根节点,同时将它加入到失败指针的更新队列中
	vector<PATTERN_TREE_NODE*> q;//动态数组作为队列使用
	for(int i = 0; i < MAX_CELL_SIZE; i++)
	{
		if(m_pPatternTreeRoot->pChildrenNode[i] != NULL)
		{
			m_pPatternTreeRoot->pChildrenNode[i]->pFailNode = m_pPatternTreeRoot;
			q.push_back(m_pPatternTreeRoot->pChildrenNode[i]);
		}
	}
	// 处理队列中的节点的子节点
	unsigned int index = 0;//index 指向队首的元素
	PATTERN_TREE_NODE* pCurrentNode;//当前正在处理的节点
	while(q.size() > index)
	{
		pCurrentNode = q[index];
		// 对于当前队首的节点，挨个找其子节点的失败指针
		if(pCurrentNode->nChildrenCount == 0)
		{
			index++;
			continue;
		}
		for(int i = 0; i < MAX_CELL_SIZE; i++)
		{
			if(pCurrentNode->pChildrenNode[i] != NULL)
			{
				// 寻找当前子节点的失败指针
				pCurrentNode->pChildrenNode[i]->pFailNode = FindFailPtr(pCurrentNode->pFailNode, (unsigned char)i);
				// 然后把当前子节点加入到队列中
				if(pCurrentNode->pChildrenNode[i]->nChildrenCount != 0)
				  q.push_back(pCurrentNode->pChildrenNode[i]);
			}
		}
		index++;
	}
	m_bPrepared = true;
	cerr<<"AC multi_pattern match machine is ready[pattern number = "<<GetPatternCount()<<"]"<<endl;
}

// 匹配接口之一
int kaijiang::CPattMatchMachine::MatchText(string& strText, map<string, MATCH_INFO>&mapMatchInfo)
{
	vector<MATCH_INFO> vecMatchInfo;//匹配的模式串信息

	// 调用底层匹配接口
	int count = MatchText(strText, vecMatchInfo);

	// 用指定给的结构输出
	for(unsigned int i = 0; i < vecMatchInfo.size(); i++)
	{
		if(vecMatchInfo[i].num == 0)
		  continue;
		mapMatchInfo[m_vecPattern[i]] = vecMatchInfo[i];
#ifdef DEBUG
		cerr<<m_vecPattern[i]<<"\tnum:"<<vecMatchInfo[i].num<<endl;
		for(unsigned int k = 0; k < vecMatchInfo[i].pos.size(); k++)
		{
			cerr<<vecMatchInfo[i].pos[k]<<"\t";
		}
		cerr<<endl;
#endif
	}
	return count;
}

// 匹配接口之二
int kaijiang::CPattMatchMachine::MatchText(string& strText, CTrieTree<MATCH_INFO>* pTrieTreeMatchInfo)
{
	if (pTrieTreeMatchInfo == NULL)
	  return 0;
	vector<MATCH_INFO> vecMatchInfo;//匹配的模式串信息
	int count = MatchText(strText, vecMatchInfo);
	// 把匹配的模式串输出
	for(unsigned int i = 0; i < vecMatchInfo.size(); i++)
	{
		if(vecMatchInfo[i].num == 0)
		  continue;
		pTrieTreeMatchInfo->Add(m_vecPattern[i].c_str(), vecMatchInfo[i]);
#ifdef DEBUG
		cerr<<m_vecPattern[i]<<"\tnum:"<<vecMatchInfo[i].num<<endl;
		for(unsigned int k = 0; k < vecMatchInfo[i].pos.size(); k++)
		{
			cerr<<vecMatchInfo[i].pos[k]<<"\t";
		}
		cerr<<endl;
#endif
	}
	return count;
}

// 匹配接口之三
int kaijiang::CPattMatchMachine::MatchText(string& strText, vector<MATCH_INFO>& vecMatchInfo)
{
	if(!m_bPrepared)
	{
		PrepareMachine();
	}
	if(strText.size() == 0)
	  return 0;

	vector<MATCH_INFO> vecTmp;
	vecTmp.reserve(m_vecPattern.size());
	vecTmp.resize(m_vecPattern.size());
	unsigned int index = 0;
	PATTERN_TREE_NODE* pCurrentNode = m_pPatternTreeRoot;
	int count = 0;
	// 逐个字符匹配
	for(; index != strText.size(); index++)
	{
		unsigned char pos = (unsigned char)strText.at(index);
		while(pCurrentNode->pChildrenNode[pos] == NULL && pCurrentNode != m_pPatternTreeRoot)
		{
			pCurrentNode = pCurrentNode->pFailNode;
		}
		pCurrentNode = pCurrentNode->pChildrenNode[pos];
		pCurrentNode = pCurrentNode == NULL ? m_pPatternTreeRoot : pCurrentNode;
		PATTERN_TREE_NODE* pNode = pCurrentNode;
		while(pNode != m_pPatternTreeRoot)
		{
			if(pNode->nPatternId != -1)
			{
#ifdef DEBUG
				cout<<"pattern:"<<m_vecPattern[pNode->nPatternId]<<endl;
#endif
				vecTmp[pNode->nPatternId].num++;
				count++;
				vecTmp[pNode->nPatternId].pos.push_back(index);
			}
			pNode = pNode->pFailNode;	
		}
	}
	vecMatchInfo.swap(vecTmp);
	return count;
}

// 匹配接口之四
int kaijiang::CPattMatchMachine::MatchText(string& strText, map<string, unsigned int>& mapMatchInfo)
{
	if(!m_bPrepared)
	{
		PrepareMachine();
	}
	if(strText.size() == 0)
		return 0;

	unsigned int index = 0;
	mapMatchInfo.clear();
	PATTERN_TREE_NODE* pCurrentNode = m_pPatternTreeRoot;
	int count = 0;
	// 逐个字符匹配
	for(; index != strText.size(); index++)
	{
		unsigned char pos = (unsigned char)strText.at(index);
		while(pCurrentNode->pChildrenNode[pos] == NULL && pCurrentNode != m_pPatternTreeRoot)
		{
			pCurrentNode = pCurrentNode->pFailNode;
		}
		pCurrentNode = pCurrentNode->pChildrenNode[pos];
		pCurrentNode = pCurrentNode == NULL ? m_pPatternTreeRoot : pCurrentNode;
		PATTERN_TREE_NODE* pNode = pCurrentNode;
		while(pNode != m_pPatternTreeRoot)
		{
			if(pNode->nPatternId != -1)
			{
#ifdef DEBUG
				cout<<"pattern:"<<m_vecPattern[pNode->nPatternId]<<endl;
#endif
				mapMatchInfo[m_vecPattern[pNode->nPatternId]]++;
				count++;
			}
			pNode = pNode->pFailNode;	
		}
	}
	return count;
}

// 获取模式串
string kaijiang::CPattMatchMachine::GetPattern(int index) const
{
	if(index < 0 || index >= (int)m_vecPattern.size())
		return "";
	return m_vecPattern[index];
}

/****************************私有方法实现*******************************/
kaijiang::CPattMatchMachine::PATTERN_TREE_NODE* kaijiang::CPattMatchMachine::FindFailPtr(PATTERN_TREE_NODE* pCurrentNode, unsigned char chPos)
{
	if(pCurrentNode == NULL)
		return NULL;

	if(pCurrentNode->pChildrenNode[chPos] != NULL)
		return pCurrentNode->pChildrenNode[chPos];

	// 如果父亲节点的失败指针中不含有以这字符开始的子节点，则沿着失败指针走下去
	PATTERN_TREE_NODE* pFailNode = pCurrentNode->pFailNode;
	if(pFailNode == NULL)
		return pCurrentNode;
	return FindFailPtr(pFailNode, chPos);
}

/****************************私有类实现*******************************/
// 构造函数
kaijiang::CPattMatchMachine::PATTERN_TREE_NODE::PATTERN_TREE_NODE()
{
	pFailNode = NULL;
	pChildrenNode = new PATTERN_TREE_NODE* [MAX_CELL_SIZE];
	for(int i=0; i < MAX_CELL_SIZE; i++)
	{
		pChildrenNode[i] = NULL;
	}
	nPatternId = -1;
	nChildrenCount = 0;
}
// 析构函数
kaijiang::CPattMatchMachine::PATTERN_TREE_NODE::~PATTERN_TREE_NODE()
{
	for(int i=0; i < MAX_CELL_SIZE; i++)
	{
		if(pChildrenNode[i] != NULL)
			delete pChildrenNode[i];
		pChildrenNode[i] = NULL;
	}
	delete [] pChildrenNode;
	pChildrenNode = NULL;
}
