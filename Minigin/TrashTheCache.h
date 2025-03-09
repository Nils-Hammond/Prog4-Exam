#pragma once
#include "BaseComponent.h"
#include <vector>
#include <set>
#include <chrono>

namespace dae
{
	class TrashTheCache : public BaseComponent
	{
	public:
		TrashTheCache(GameObject* owner, int size);
		~TrashTheCache() = default;
		TrashTheCache(const TrashTheCache& other) = delete;
		TrashTheCache(TrashTheCache&& other) = delete;
		TrashTheCache& operator=(const TrashTheCache& other) = delete;
		TrashTheCache& operator=(TrashTheCache&& other) = delete;

		void Update() override;
		void Render() const override;
	private:
		struct Transform
		{
			float matrix[16] = {
				1,0,0,0,
				0,1,0,0,
				0,0,1,0,
				0,0,0,1
			};
		};

		class GameObject3D
		{
		public:
			Transform local;
			int id;
		};

		class GameObject3DAlt
		{
		public:
			Transform* local;
			int id;
		};

		std::vector<int> m_largeIntVector{};
		std::vector<GameObject3D> m_largeObjectVector{};
		std::vector<GameObject3DAlt> m_largeObjectAltVector{};

		template <typename T, typename CallableObject>
		std::vector<float> RunTest(std::vector<T> largeArray, int sampleSize, CallableObject operation) const
		{
			std::vector<float> averageTimings;
			for (int startingStepSize{ 1 }; startingStepSize <= 1024; startingStepSize *= 2)
			{
				std::set<float> timings;
				for (int counter{}; counter < sampleSize; ++counter)
				{
					auto start = std::chrono::high_resolution_clock::now();
					for (int stepsize = startingStepSize; stepsize <= 1024; stepsize *= 2)
					{
						for (int idx{}; idx < largeArray.size(); idx += stepsize)
						{
							operation(largeArray[idx]);
						}
					}
					auto end = std::chrono::high_resolution_clock::now();
					auto duration = std::chrono::duration<float, std::milli>(end - start).count();
					timings.insert(duration);
				}
				if (timings.size() > 2)
				{
					timings.erase(timings.begin());
					timings.erase(std::prev(timings.end()));
				}
				float averageTime{};
				for (auto timing : timings)
				{
					averageTime += timing;
				}
				averageTime /= timings.size();
				averageTimings.push_back(averageTime);
			}
			return averageTimings;
		}
	};
}
