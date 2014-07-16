/**
 * trie���洢ͨ��ʵ��
 *
 * ���ߣ�����
 *
 * ���ڣ�20120515
 *
 * �޸�˵�����Ӵʵ䵽code��ӳ�����޸�Ϊ�ؼ��ʷ����洢�ṹ��ȥ��code
 *			���ڵ�ͷ�ļ����ݺϲ������ļ����Ժ�ֻ��Ҫ����һ��ͷ�ļ�����
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
				//�������캯��
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

				// ��ȡָ����֧
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
				// ��ȡ��ǰ�ڵ��ֵ
				VALUE& GetValue() {return m_value; }
				VALUE GetConstValue() const {return m_value; }
				const char* GetKey()const {return m_pKeyStr;}

				// ��ȡ��ǰ�ڵ��ϴ���ı��
				int& TheCode(){return m_nCode;}

				// �жϵ�ǰ�ڵ��Ƿ��Ǵ�
				inline bool IsWord() const {return m_nCode!= -1;}

				// �����ַ�������
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
				// �ýڵ�ķ�֧
				TrieNode** m_pCells;//

				//  �����ֶ�ֻ�е���ǰ�ڵ���һ��key����������Ч
				int m_nCode;//��Ӧ�ĴʵĴ��룬��������ڽ����ڴ˽ڵ�Ĵʣ����ֵΪ-1
				VALUE m_value;//������������
				char *m_pKeyStr;
				int m_nLength;//����ĳ��ȣ��ӵ�һ���ֽڵ���ǰ���������ֽ����������ǰ�ڵ���ڴ���Ļ���
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
				// ���һ����
				// ������
				//		word -- Ҫ������ַ���
				//		data -- ����Ҫ����������
				//
				// ԭ���ôʲ������Ҳ���ɹ��򷵻�true�����򷵻�false
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

				// �ж��Ƿ����ָ����key
				// ������
				//		key -- ��Ϊkey���ַ���
				// �����򷵻�true
				bool HaveKey(const char* key) const 
				{
					return SearchKey(key) != NULL;
				}

				// ����key����code
				// ���Ƽ�ʹ��
				//
				int GetKeyCode(const char* key) const 
				{
					TrieNode<DATA>* pNode = SearchKey(key);
					if(pNode == NULL)
					  return -1;
					return pNode->TheCode();
				}

				// ���ݴʵ�ID��ȡ��
				// ������
				//		code -- ����Ĵ���
				// ����
				string GetKey(int code) const 
				{
					if(code <= 0 || code > (int)m_vecWords.size())
					  return "";
					return m_vecWords[code - 1]->GetKey();
				}

				// ����key�Ĵ��뷵����������ֵ
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
				// ����key�ĸ���
				int GetKeyCount()  const 
				{
					return m_vecWords.size();
				}

				// ��ӡ
				ostream& Print(ostream& os)
				{
					PrintTree(os, m_pRoot);	
					return os;
				}

				// ����key������Ӧ�Ľڵ�
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
				DATA m_ErrorIndex;//��Ϊ��������ʱ���ص�ֵ
		};
};
#endif
