#ifndef CRANDOM_SAMPLER_H
#define CRANDOM_SAMPLER_H

#include <vector>
#include <iostream>
#include <fstream>
#include <time.h>
#include "CTypeTool.h"

using namespace std;

// 随机采样器，实现了两种随机采样算法：
// 1、对于已知数据大小的等概率采样
// 2、对于未知数据大小的等概率采样（蓄水池算法）
//
namespace kaijiang
{
	// 说明：[0,1]均匀分布函数，产生在[0,1]之间均匀分布的随机数
	// 参数：
	//	无
	// 返回：
	//  [0,1]之间均匀分布的随机数
	float frand(uint32_t seed)
	{
		return ((float)rand_r(&seed)) / (float)RAND_MAX;
	}

	// 采样器实现，模板类
	template <class T>
	class CRandomSampler
	{
		public:
		// 说明：对于小数据量，可以全部读入内存的，等概率采样m个数据
		// 参数：
		//  [IN] vecData -- 总体数据
		//  [IN] m -- 要采样的个数
		//  [OUT] vecRand -- 随机采样结果
		static bool DoSampling(const vector<T> vecData, int m, vector<T>& vecRand)
		{
			int32_t nSize = vecData.size();
			if(nSize  < m || m < 0)
			  return false;
			vecRand.clear();
			vecRand.reserve(m);
			for(int32_t i = 0, isize = nSize; i < isize ; i++)
			{
				float fRand = frand(time(0));
				if(fRand <=(float)(m)/nSize)
				{
					vecRand.push_back(vecData[i]);
					m--;
				}
				nSize --;
			}
			return true;
		}

		// 说明：对于流式数据（未知大小的数据），等概率采样m个（蓄水池算法）
		// 参数：
		//	[IN] isDataStream -- 输入数据流
		//	[IN] m -- 采样个数
		//	[OUT] vecRand -- 随机采样结果
		// 返回：
		//  采样成功
		static bool DoSampling(istream& isDataStream, int m, vector<string>& vecRand)
		{
			if(m <= 0)
			  return false;
			vecRand.clear();
			vecRand.reserve(m);
			uint32_t count = 1;
			string line;
			while(getline(isDataStream, line))
			{
				if(count <= (uint32_t)m)
				{
					vecRand.push_back(line);
				}
				else 
				{
					float fRand = frand(time(0));
					if(fRand < (float)m/(float)count)
					{
						vecRand[(size_t)(frand() * m)] = line;
					}
				}
				count++;
			}

			return true;
		}
	};
};
#endif
