#ifndef CRANDOM_SAMPLER_H
#define CRANDOM_SAMPLER_H

#include <vector>
#include <iostream>
#include <fstream>
#include <time.h>
#include "CTypeTool.h"

using namespace std;

// �����������ʵ����������������㷨��
// 1��������֪���ݴ�С�ĵȸ��ʲ���
// 2������δ֪���ݴ�С�ĵȸ��ʲ�������ˮ���㷨��
//
namespace kaijiang
{
	// ˵����[0,1]���ȷֲ�������������[0,1]֮����ȷֲ��������
	// ������
	//	��
	// ���أ�
	//  [0,1]֮����ȷֲ��������
	float frand(uint32_t seed)
	{
		return ((float)rand_r(&seed)) / (float)RAND_MAX;
	}

	// ������ʵ�֣�ģ����
	template <class T>
	class CRandomSampler
	{
		public:
		// ˵��������С������������ȫ�������ڴ�ģ��ȸ��ʲ���m������
		// ������
		//  [IN] vecData -- ��������
		//  [IN] m -- Ҫ�����ĸ���
		//  [OUT] vecRand -- ����������
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

		// ˵����������ʽ���ݣ�δ֪��С�����ݣ����ȸ��ʲ���m������ˮ���㷨��
		// ������
		//	[IN] isDataStream -- ����������
		//	[IN] m -- ��������
		//	[OUT] vecRand -- ����������
		// ���أ�
		//  �����ɹ�
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
