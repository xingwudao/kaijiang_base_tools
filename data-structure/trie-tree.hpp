/**
 * trie树存储通用实现
 *
 * 作者：开江
 *
 * 日期：20120515
 *
 * 修改说明：从词典到code的映射树修改为关键词分数存储结构，去掉code
 *			将节点头文件内容合并到本文件，以后只需要引用一个头文件即可
 *
 */

#ifndef CTRIETREE_H
#define CTRIETREE_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

namespace kaijiang
{
		#define MAX_CELL_SIZE 256

		template <class VALUE>
		class TrieNode
		{
			public:
				TrieNode(VALUE v):m_value(v)
			{
				m_nCode = -1;
				m_nLength = 0;
				m_pCells = new TrieNode* [MAX_CELL_SIZE];
				for(int i = 0; i < MAX_CELL_SIZE; i++)
				{
					m_pCells[i] = NULL;
				}
				m_pKeyStr = NULL;
			}
				TrieNode()
				{
					m_nCode = -1;
					m_nLength = 0;
					m_pCells = new TrieNode* [MAX_CELL_SIZE];
					for(int i = 0; i < MAX_CELL_SIZE; i++)
					{
						m_pCells[i] = NULL;
					}
					m_pKeyStr = NULL;
				}
				//拷贝构造函数
				TrieNode(const TrieNode& other)
				{
					this->m_nCode = other.m_nCode;
					this->m_nLength = other.m_nLength;
					this->m_pCells = new TrieNode* [MAX_CELL_SIZE];
					for(int i = 0; i < MAX_CELL_SIZE; i++)
					{
						if(other.m_pCells[i] == NULL)
						{
							m_pCells[i] = NULL;
							continue;
						}
						m_pCells[i] = new TrieNode(*(other.m_pCells[i]));
					}
					if(other.m_pKeyStr != NULL)
					{
						m_pKeyStr = new char [strlen(other.m_pKeyStr) + 1];
						strcpy(m_pKeyStr, other.m_pKeyStr);
					}
					else 
					{
						m_pKeyStr = NULL;
					}
				}
				TrieNode& operator=(const TrieNode& other)
				{
					if(&other == this)
					  return *this;
					this->m_nCode = other.m_nCode;
					this->m_nLength = other.m_nLength;
					for(int i = 0; i < MAX_CELL_SIZE; i++)
					{
						if(m_pCells[i] != NULL)
						{
							delete m_pCells[i];
							m_pCells[i] = NULL;
						}
						if(other.m_pCells[i] != NULL)
						{
							m_pCells[i] = new TrieNode(*(other.m_pCells[i]));
						}
					}
					if(other.m_pKeyStr != NULL)
					{
						this->m_pKeyStr = new char [strlen(other.m_pKeyStr) + 1];
						strcpy(this->m_pKeyStr, other.m_pKeyStr);
					}
					else 
					{
						this->m_pKeyStr = NULL;
					}
					return *this;
				}
				~TrieNode()
				{
					for(int i = 0; i < MAX_CELL_SIZE; i++)
					{
						if(m_pCells[i] != NULL)
						{
							delete m_pCells[i];
							m_pCells[i] = NULL;
						}
					}
					delete [] m_pCells;
					m_pCells = NULL;
					if(m_pKeyStr != NULL)
					  delete [] m_pKeyStr;
				}

				// 获取指定分支
				TrieNode* GetCell(int pos) const
				{
					if(pos < 0 || pos > 255)
					  return NULL;
					return m_pCells[pos]; 
				}
				void SetCell(TrieNode* pBranch, int pos)
				{
					if(pos < 0 || pos > 255 || pBranch == NULL)
					{
						return ;
					}
					m_pCells[pos] = pBranch;
				}
				// 获取当前节点的值
				VALUE& GetValue() {return m_value; }
				VALUE GetConstValue() const {return m_value; }
				const char* GetKey()const {return m_pKeyStr;}

				// 获取当前节点上词语的编号
				int& TheCode(){return m_nCode;}

				// 判断当前节点是否是词
				inline bool IsWord() const {return m_nCode!= -1;}

				// 操作字符串长度
				int & GetKeyLength(){return m_nLength;}

				void SetKey(const char* strKey)
				{
					if(m_pKeyStr == NULL)
					{
						m_pKeyStr = new char [strlen(strKey) + 1];
						strcpy(m_pKeyStr, strKey);
					}
				}
			private:
				// 该节点的分支
				TrieNode** m_pCells;//

				//  以下字段只有当当前节点是一个key结束处才有效
				int m_nCode;//对应的词的代码，如果不存在结束在此节点的词，则此值为-1
				VALUE m_value;//词索引的数据
				char *m_pKeyStr;
				int m_nLength;//词语的长度（从第一个字节到当前所经历的字节数，如果当前节点存在词语的话）
		};

	template <class DATA>
		class CTrieTree
		{
			public:
				CTrieTree()
				{
					m_pRoot = new TrieNode<DATA>;
				}
				CTrieTree(const CTrieTree& other)
				{
					m_pRoot = new TrieNode<DATA>;
					for(unsigned int i = 0; i < other.m_vecWords.size(); i++)
					{
						DATA data = other.m_vecWords[i]->GetConstValue();
						Add(other.GetKey(i+1).c_str(), data);		
					}
				}
				CTrieTree& operator=(const CTrieTree& other)
				{
					if(&other == this)
					  return *this;
					if(this->m_pRoot != NULL)
					  delete this->m_pRoot;
					this->m_pRoot = new TrieNode<DATA>;
					m_vecWords.clear();
					for(unsigned int i = 0; i < other.m_vecWords.size(); i++)
					{
						DATA data = other.m_vecWords[i]->GetConstValue();
						Add(other.GetKey(i+1).c_str(), data);		
					}
					return *this;
				}
				~CTrieTree()
				{
					if(m_pRoot != NULL)
					  delete m_pRoot;
				}
			public:
				// 添加一个词
				// 参数：
				//		word -- 要插入的字符串
				//		data -- 词所要索引的数据
				//
				// 原来该词不存在且插入成功则返回true，否则返回false
				//
				TrieNode<DATA>* Add(const char* word, DATA& data)
				{
					int code = -1;
					int len = strlen(word);
					if(len <= 0)
					  return NULL;
					TrieNode<DATA>* pNode;
					TrieNode<DATA>* pCurrentNode;
					pCurrentNode = m_pRoot;
					for(int i = 0; i < len; i++)
					{
						unsigned char pos = (unsigned char)word[i];
						pNode = pCurrentNode->GetCell(pos);
						if(pNode == NULL)
						{
							pNode = new TrieNode<DATA>;
							pNode->GetKeyLength() = i + 1;
							pCurrentNode->SetCell(pNode, pos);
						}
						pCurrentNode = pNode;
					}
					if(pNode->TheCode() > 0)
					{
						return pNode;
					}
					m_vecWords.push_back(pNode);
					pNode->TheCode() = m_vecWords.size();
					pNode->SetKey(word);
					pNode->GetValue() = data; 
					return pNode;
				}

				// 判断是否存在指定的key
				// 参数：
				//		key -- 作为key的字符串
				// 存在则返回true
				bool HaveKey(const char* key) const 
				{
					return SearchKey(key) != NULL;
				}

				// 根据key返回code
				// 不推荐使用
				//
				int GetKeyCode(const char* key) const 
				{
					TrieNode<DATA>* pNode = SearchKey(key);
					if(pNode == NULL)
					  return -1;
					return pNode->TheCode();
				}

				// 根据词的ID获取词
				// 参数：
				//		code -- 词语的代码
				// 词语
				string GetKey(int code) const 
				{
					if(code <= 0 || code > (int)m_vecWords.size())
					  return "";
					return m_vecWords[code - 1]->GetKey();
				}

				// 根据key的代码返回其索引的值
				DATA& GetValue(int code)
				{
					if(code < 1 || code > (int)m_vecWords.size())
					{
						return m_ErrorIndex;
					}
					return m_vecWords[code - 1]->GetValue();
				}

				DATA& GetValue(const char* key)
				{
					TrieNode<DATA>* pNode = SearchKey(key);
					if(pNode == NULL)
					  return m_ErrorIndex;
					return pNode->GetValue();
				}
				// 返回key的个数
				int GetKeyCount()  const 
				{
					return m_vecWords.size();
				}

				// 打印
				ostream& Print(ostream& os)
				{
					PrintTree(os, m_pRoot);	
					return os;
				}

				// 根据key搜索对应的节点
				TrieNode<DATA>* SearchKey(const char* key) const
				{
					int len = strlen(key);
					if(len <= 0)
					  return NULL;
					TrieNode<DATA>* pNode;
					TrieNode<DATA>* pCurrentNode = m_pRoot;
					for(int i = 0; i < len; i++)
					{
						if(pCurrentNode ==  NULL)
						  return NULL;
						unsigned char pos = (unsigned char)key[i];
						pNode = pCurrentNode->GetCell(pos);
						if(pNode == NULL)
						{
							return NULL;
						}
						pCurrentNode = pNode;
					}
					if(pNode->GetKey() == NULL || strlen(pNode->GetKey()) == 0)
					{
						return NULL;
					}
					if(pNode->TheCode() <= 0)
					  return NULL;
					return pNode;
				}
				ostream& PrintTree(ostream& os, TrieNode<DATA>* pTree)
				{
					if(pTree->TheCode() > 0)
					{
						os<<pTree->TheCode()<<":"<<pTree->GetKey()<<"\t"<<pTree->GetValue()<<endl;
					}
					for(int i=0; i < MAX_CELL_SIZE; i++)
					{
						if(pTree->GetCell(i) != NULL)
						  PrintTree(os, pTree->GetCell(i));
					}
					return os;
				}
			private:
				TrieNode<DATA>* m_pRoot;
				vector<TrieNode<DATA>*> m_vecWords;
				DATA m_ErrorIndex;//作为索引错误时返回的值
		};
};
#endif
