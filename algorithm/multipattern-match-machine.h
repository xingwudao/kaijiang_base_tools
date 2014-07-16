/**
 * AC算法，多模式匹配自动机
 *
 * 作者：开江
 *
 * 日期：2012 06 05
 *
 * 版本：0.3
 * 
 * 修订信息：
 *			增加：复制构造函数和重载赋值操作符,	节点结构体声明位置移到模式匹配机内部，外部不使用该结构体
 *			增加：匹配信息输出采用trie树存储，对模式串比较多的情形，可以在统计匹配信息时减少字符串比较次数
 *			增加：匹配信息输出采用vector存储
 *			修改：内部结构实现转移至cpp文件中
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
	/****模式串的匹配信息结构体****/
	typedef struct MATCH_INFO
	{
		int num;//匹配次数
		vector<unsigned int> pos;//匹配位置(模式串终点的位置，如果需要起点位置则用该值减去模式串长度)
		// 构造函数
		MATCH_INFO();
	}MATCH_INFO;


	//模式树节点的最大子节点数
	#define MAX_CELL_SIZE 256


	/*****多模式自动机类声明*****/
	class CPattMatchMachine
	{
		public:
			// 构造函数
			CPattMatchMachine();
			// 从文件读入模式串构造
			CPattMatchMachine(const char* cPatternFile);
			// 复制构造函数
			CPattMatchMachine(const CPattMatchMachine& other);
			// 赋值操作符
			CPattMatchMachine& operator=(const CPattMatchMachine& other);
			// 析构函数
			~CPattMatchMachine();

			/***********************公有方法*************************/
		public:
			// 添加一个模式串
			// 参数：
			//		strPattern -- 模式串
			// 返回：
			//		添加成功 -- true
			//		添加失败 -- false
			bool AddPattern(string& strPattern);

			// 追加一个模式串文件
			// 参数：
			//		cPatternFile -- 模式串文件
			// 返回：
			//		追加的模式串数量
			int AddPattern(const char* cPatternFile);

			// 启动多模式自动机，一旦新加了模式串到自动机之后都需要执行本方法才能生效.
			// 参数：
			//		
			// 返回：
			//
			void PrepareMachine();

			// 匹配输入文本
			// 参数：
			//		strText -- 输入文本
			//		mapMatchInfo[OUT] -- 匹配信息
			//		pTrieTreeMatchInfo[OUT] -- 匹配信息(用trie树保存)
			//		vecMatchInfo[OUT] -- 每个模式串的匹配信息，没有匹配上的其数目则是0，匹配信息的位置号则是模式串的ID号，可以通过GetPattern接口读取模式串
			// 返回：
			//		匹配的模式串数目
			int MatchText(string& strText, map<string, MATCH_INFO>& mapMatchInfo);
			int MatchText(string& strText, CTrieTree<MATCH_INFO>* pTrieTreeMatchInfo);
			int MatchText(string& strText, vector<MATCH_INFO>& vecMatchInfo);
			int MatchText(string& strText, map<string, unsigned int>& mapMatchInfo);

			// 获取模式串数目
			// 参数：
			//		无
			// 返回：
			//		模式串数目
			//
			inline int GetPatternCount() const {return m_vecPattern.size();};

			// 获取模式串
			// 参数：
			//		index -- 索引号
			// 返回：
			//		模式串
			string GetPattern(int index) const ;

			/**********************私有方法****************************/
		private:

			/*****多模式自动机的模式树节点定义****/
			class  PATTERN_TREE_NODE
			{
				public:
					PATTERN_TREE_NODE* pFailNode;//失败指针
					int nPatternId;//模式串ID，如果此节点不是一个模式串的结束点，则为-1
					PATTERN_TREE_NODE** pChildrenNode;//子节点

					int nChildrenCount;
					// 构造函数
					PATTERN_TREE_NODE();
					// 析构函数
					~PATTERN_TREE_NODE();
			};

			// 插入一个模式串,返回模式串ID
			int InsertPattern(string& strPattern);

			// 为指定节点的指定子节点寻找失败指针
			PATTERN_TREE_NODE* FindFailPtr(PATTERN_TREE_NODE* pCurrentNode, unsigned char chPos);

			// 打印节点，调试用
			// 参数：
			//		pNode -- 节点
			//		l -- 层级
			void PrintNode(PATTERN_TREE_NODE* pNode, int l);
			/***************************私有属性************************/
		private:
			vector<string> m_vecPattern;//所有的模式串
			PATTERN_TREE_NODE* m_pPatternTreeRoot;//模式树根节点
			bool m_bPrepared;//标识模式匹配机是否准备好
	};
};
#endif
