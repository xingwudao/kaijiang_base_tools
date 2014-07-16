/**
 * AC�㷨����ģʽƥ���Զ���
 *
 * ���ߣ�����
 *
 * ���ڣ�2012 06 05
 *
 * �汾��0.3
 * 
 * �޶���Ϣ��
 *			���ӣ����ƹ��캯�������ظ�ֵ������,	�ڵ�ṹ������λ���Ƶ�ģʽƥ����ڲ����ⲿ��ʹ�øýṹ��
 *			���ӣ�ƥ����Ϣ�������trie���洢����ģʽ���Ƚ϶�����Σ�������ͳ��ƥ����Ϣʱ�����ַ����Ƚϴ���
 *			���ӣ�ƥ����Ϣ�������vector�洢
 *			�޸ģ��ڲ��ṹʵ��ת����cpp�ļ���
 */

#ifndef CPATTMATCHMACHINE_H_
#define CPATTMATCHMACHINE_H_
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include "./data-structure/trie-tree.hpp"

using namespace std;

namespace kaijiang
{
	/****ģʽ����ƥ����Ϣ�ṹ��****/
	typedef struct MATCH_INFO
	{
		int num;//ƥ�����
		vector<unsigned int> pos;//ƥ��λ��(ģʽ���յ��λ�ã������Ҫ���λ�����ø�ֵ��ȥģʽ������)
		// ���캯��
		MATCH_INFO();
	}MATCH_INFO;


	//ģʽ���ڵ������ӽڵ���
	#define MAX_CELL_SIZE 256


	/*****��ģʽ�Զ���������*****/
	class CPattMatchMachine
	{
		public:
			// ���캯��
			CPattMatchMachine();
			// ���ļ�����ģʽ������
			CPattMatchMachine(const char* cPatternFile);
			// ���ƹ��캯��
			CPattMatchMachine(const CPattMatchMachine& other);
			// ��ֵ������
			CPattMatchMachine& operator=(const CPattMatchMachine& other);
			// ��������
			~CPattMatchMachine();

			/***********************���з���*************************/
		public:
			// ���һ��ģʽ��
			// ������
			//		strPattern -- ģʽ��
			// ���أ�
			//		��ӳɹ� -- true
			//		���ʧ�� -- false
			bool AddPattern(string& strPattern);

			// ׷��һ��ģʽ���ļ�
			// ������
			//		cPatternFile -- ģʽ���ļ�
			// ���أ�
			//		׷�ӵ�ģʽ������
			int AddPattern(const char* cPatternFile);

			// ������ģʽ�Զ�����һ���¼���ģʽ�����Զ���֮����Ҫִ�б�����������Ч.
			// ������
			//		
			// ���أ�
			//
			void PrepareMachine();

			// ƥ�������ı�
			// ������
			//		strText -- �����ı�
			//		mapMatchInfo[OUT] -- ƥ����Ϣ
			//		pTrieTreeMatchInfo[OUT] -- ƥ����Ϣ(��trie������)
			//		vecMatchInfo[OUT] -- ÿ��ģʽ����ƥ����Ϣ��û��ƥ���ϵ�����Ŀ����0��ƥ����Ϣ��λ�ú�����ģʽ����ID�ţ�����ͨ��GetPattern�ӿڶ�ȡģʽ��
			// ���أ�
			//		ƥ���ģʽ����Ŀ
			int MatchText(string& strText, map<string, MATCH_INFO>& mapMatchInfo);
			int MatchText(string& strText, CTrieTree<MATCH_INFO>* pTrieTreeMatchInfo);
			int MatchText(string& strText, vector<MATCH_INFO>& vecMatchInfo);
			int MatchText(string& strText, map<string, unsigned int>& mapMatchInfo);

			// ��ȡģʽ����Ŀ
			// ������
			//		��
			// ���أ�
			//		ģʽ����Ŀ
			//
			inline int GetPatternCount() const {return m_vecPattern.size();};

			// ��ȡģʽ��
			// ������
			//		index -- ������
			// ���أ�
			//		ģʽ��
			string GetPattern(int index) const ;

			/**********************˽�з���****************************/
		private:

			/*****��ģʽ�Զ�����ģʽ���ڵ㶨��****/
			class  PATTERN_TREE_NODE
			{
				public:
					PATTERN_TREE_NODE* pFailNode;//ʧ��ָ��
					int nPatternId;//ģʽ��ID������˽ڵ㲻��һ��ģʽ���Ľ����㣬��Ϊ-1
					PATTERN_TREE_NODE** pChildrenNode;//�ӽڵ�

					int nChildrenCount;
					// ���캯��
					PATTERN_TREE_NODE();
					// ��������
					~PATTERN_TREE_NODE();
			};

			// ����һ��ģʽ��,����ģʽ��ID
			int InsertPattern(string& strPattern);

			// Ϊָ���ڵ��ָ���ӽڵ�Ѱ��ʧ��ָ��
			PATTERN_TREE_NODE* FindFailPtr(PATTERN_TREE_NODE* pCurrentNode, unsigned char chPos);

			// ��ӡ�ڵ㣬������
			// ������
			//		pNode -- �ڵ�
			//		l -- �㼶
			void PrintNode(PATTERN_TREE_NODE* pNode, int l);
			/***************************˽������************************/
		private:
			vector<string> m_vecPattern;//���е�ģʽ��
			PATTERN_TREE_NODE* m_pPatternTreeRoot;//ģʽ�����ڵ�
			bool m_bPrepared;//��ʶģʽƥ����Ƿ�׼����
	};
};
#endif
