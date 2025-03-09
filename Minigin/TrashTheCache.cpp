#include "TrashTheCache.h"
#include "GameObject.h"
#include "imgui.h"
#include "imgui_plot.h"
#include <iostream>

dae::TrashTheCache::TrashTheCache(GameObject* owner, int size)
	: BaseComponent(owner)
{
	size_t vectorSize = size;
	std::cout << "TrashTheCache: Initializing large vectors, this may take a while\n";
	for (int i{}; i < vectorSize; ++i)
	{
		m_largeIntVector.push_back(i);
		m_largeObjectVector.push_back(GameObject3D{});
		m_largeObjectAltVector.push_back(GameObject3DAlt{});
	}

}

void dae::TrashTheCache::Update()
{
}

void dae::TrashTheCache::Render() const
{

	// Exercise 1 Window
	ImGui::Begin("Exercise 1");

	static int sampleSize = 10;
	ImGui::InputInt("# samples", &sampleSize);
	
	static std::vector<float> averageTimings{};
	static bool loading = false;
	if (loading)
	{
		ImGui::Text("Wait for it...");
		averageTimings = RunTest<int>(m_largeIntVector, sampleSize, [](int& number) {number *= 2; });
		loading = false;
	}
	else
	{
		if (ImGui::Button("Trash The Cache"))
			loading = true;
	}

	ImGui::PlotConfig conf;
	const float stepsizes[11] = { 1,2,4,8,16,32,64,128,256,512,1024 };
	conf.values.xs = stepsizes;
	conf.scale.min = -1;
	conf.tooltip.show = true;
	conf.tooltip.format = "x=%.2f, y=%.2f";
	conf.grid_x.show = false;
	conf.grid_y.show = true;
	conf.frame_size = ImVec2(200, 100);
	conf.line_thickness = 2.f;


	if (averageTimings.size() > 0)
	{
		conf.values.ys = averageTimings.data();
		conf.values.count = static_cast<int>(averageTimings.size());
		conf.values.color = ImColor{ 255,0,0 };
		conf.scale.max = averageTimings[0];
		ImGui::Plot("Graph 1", conf);
	}

	ImGui::End();

	//Exercise 2 Window
	ImGui::Begin("Exercise 2");

	static int objectSampleSize = 100;
	ImGui::InputInt("# samples", &objectSampleSize);

	// GameObject3D Section
	static std::vector<float> gameObject3DAverageTimings{};

	static bool loadingGameObject3DResults = false;
	if (loadingGameObject3DResults)
	{
		ImGui::Text("Wait for it...");
		gameObject3DAverageTimings = RunTest<GameObject3D>(m_largeObjectVector, objectSampleSize, [](GameObject3D& object) {object.id *= 2; });
		loadingGameObject3DResults = false;
	}
	else
	{
		if (ImGui::Button("Trash the cache With GameObject3D"))
			loadingGameObject3DResults = true;
	}

	if (gameObject3DAverageTimings.size() > 0)
	{
		conf.values.ys = gameObject3DAverageTimings.data();
		conf.values.count = static_cast<int>(gameObject3DAverageTimings.size());
		conf.values.color = ImColor{ 0,255,0 };
		conf.scale.max = gameObject3DAverageTimings[0];
		ImGui::Plot("Graph 2", conf);
	}

	// GameObject3DAlt Section
	static std::vector<float> gameObject3DAltAverageTimings{};

	static bool loadingGameObject3DAltResults = false;
	if (loadingGameObject3DAltResults)
	{
		ImGui::Text("Wait for it...");
		gameObject3DAltAverageTimings = RunTest<GameObject3DAlt>(m_largeObjectAltVector, objectSampleSize, [](GameObject3DAlt& object) {object.id *= 2; });
		loadingGameObject3DAltResults = false;
	}
	else
	{
		if (ImGui::Button("Trash the cache With GameObject3DAlt"))
			loadingGameObject3DAltResults = true;
	}

	if (gameObject3DAltAverageTimings.size() > 0)
	{
		conf.values.ys = gameObject3DAltAverageTimings.data();
		conf.values.count = static_cast<int>(gameObject3DAltAverageTimings.size());
		conf.values.color = ImColor{ 0,255,255 };
		conf.scale.max = gameObject3DAltAverageTimings[0];
		ImGui::Plot("Graph 3", conf);
	}

	if (gameObject3DAverageTimings.size() > 0 && gameObject3DAltAverageTimings.size() > 0)
	{
		ImGui::Text("Combined:");
		const float* y_data[] = { gameObject3DAverageTimings.data(), gameObject3DAltAverageTimings.data() };
		static ImU32 colors[3] = { ImColor(0, 255, 0), ImColor(0, 255, 255)};
		conf.values.ys = 0;
		conf.values.count = static_cast<int>(gameObject3DAverageTimings.size());
		conf.values.ys_list = y_data;
		conf.values.ys_count = 2;
		conf.values.colors = colors;
		conf.scale.max = gameObject3DAverageTimings[0];
		ImGui::Plot("Graph 4", conf);
	}

	ImGui::End();
}
