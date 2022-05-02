#pragma once
#include <functional>
#include <thread>

namespace dae
{
	struct TransformTTC
	{
		float matrix[16] = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
	};

	class GameObject3D
	{
	public:
		TransformTTC transform;
		int          ID = 1;
	};

	class GameObject3DAlt
	{
	public:
		TransformTTC* transform;
		int           ID = 1;
	};


	inline void plot(const float*  xData, std::vector<float> yData, const size_t count,
	          const ImColor color)
	{
		ImGui::PlotConfig conf;
		conf.values.xs      = xData;
		conf.values.ys      = yData.data();
		conf.values.count   = int(count);
		conf.scale.min      = 0.f;
		conf.scale.max      = yData[0];
		conf.tooltip.show   = true;
		conf.tooltip.format = "x=%.2f, y=%.2f";
		conf.grid_x.show    = true;
		conf.grid_y.show    = true;
		conf.grid_y.size    = 5000;
		conf.frame_size     = ImVec2(200, 100);
		conf.line_thickness = 2.f;
		conf.values.color   = color;

		Plot("plot", conf);
	}

	inline void plotMulti(const float* xData, const float** yData,
	               const size_t count, int           ys_count, const ImU32* colors)
	{
		ImGui::PlotConfig conf;
		conf.values.xs       = xData;
		conf.values.ys_list  = yData;
		conf.values.count    = int(count);
		conf.values.ys_count = ys_count;
		conf.scale.min       = 0;
		conf.scale.max       = yData[0][0];
		conf.tooltip.show    = true;
		conf.tooltip.format  = "x=%.2f, y=%.2f";
		conf.grid_x.show     = true;
		conf.grid_y.show     = true;
		conf.grid_y.size     = 5000;
		conf.frame_size      = ImVec2(200, 100);
		conf.line_thickness  = 2.f;
		conf.values.colors   = colors;

		Plot("plot", conf);
	}

	class CacheExercise
	{
	public:
		CacheExercise() = default;

		void Render()
		{
			ImGui::Begin("Exercise 2");
			ImGui::InputInt("# samples", &m_NrSamples);
			if (m_LoadingGraph1)
			{
				ImGui::Text("Loading...");
			}
			else
			{
				if (ImGui::Button("Trash the cache"))
				{
					m_LoadingGraph1 = true;
					m_Graph1Xs.clear();
					m_Graph1Ys.clear();

					std::thread t(&dae::CacheExercise::TTCInt, this);
					t.detach();
				}
			}

			if (!m_Graph1Ys.empty())
			{
				m_LoadingGraph1 = false;
				plot(m_Graph1Xs.data(), m_Graph1Ys, m_Graph1Ys.size(), ImColor(122, 122, 0));
			}

			ImGui::End();

			ImGui::Begin("Exercise 3");
			ImGui::InputInt("# samples", &m_NrSamples2);
			if (m_LoadingGraph2)
			{
				ImGui::Text("Loading...");
			}
			else
			{
				if (ImGui::Button("Trash the cache with GameObject3D"))
				{
					m_LoadingGraph2 = true;
					m_Graph2Xs.clear();
					m_Graph2Ys.clear();

					std::thread t(&dae::CacheExercise::TTC3D, this);
					t.detach();
				}
			}
			if (m_LoadingGraph3)
			{
				ImGui::Text("Loading...");
			}
			else
			{
				if (ImGui::Button("Trash the cache with GameObject3DAlt"))
				{
					m_LoadingGraph3 = true;
					m_Graph3Xs.clear();
					m_Graph3Ys.clear();

					std::thread t(&dae::CacheExercise::TTTC3DAlt, this);
					t.detach();
				}
			}

			if (!m_Graph2Ys.empty())
			{
				m_LoadingGraph2 = false;
				plot(m_Graph2Xs.data(), m_Graph2Ys,
				     m_Graph2Ys.size(), ImColor(0, 122, 0));
			}
			if (!m_Graph3Ys.empty())
			{
				m_LoadingGraph3 = false;
				plot(m_Graph3Xs.data(), m_Graph3Ys,
				     m_Graph3Ys.size(), ImColor(0, 122, 122));
			}

			if (!m_Graph2Ys.empty() && !m_Graph3Ys.empty())
			{
				ImGui::Text("Combined:");
				std::vector ysList    = {m_Graph2Ys.data(), m_Graph3Ys.data()};
				float**     ysListRaw = ysList.data();
				const ImU32 colors[3] = {ImColor(0, 122, 0), ImColor(0, 122, 122)};
				plotMulti(
					m_Graph3Xs.data(),
					const_cast<const float**>(ysListRaw),
					m_Graph3Ys.size(), 2,
					colors);
			}

			ImGui::End();
		}

	private:
		int  m_NrSamples     = 10;
		bool m_LoadingGraph1 = false;

		std::vector<float> m_Graph1Xs{};
		std::vector<float> m_Graph1Ys{};

		int  m_NrSamples2    = 10;
		bool m_LoadingGraph2 = false;

		std::vector<float> m_Graph2Xs{};
		std::vector<float> m_Graph2Ys{};

		bool m_LoadingGraph3 = false;

		std::vector<float> m_Graph3Xs{};
		std::vector<float> m_Graph3Ys{};

		// Trash the cache exercises
		void TTCInt();
		void TTC3D();
		void TTTC3DAlt();

		template <typename T>
		void TrashTheCache(
			int                                       testSize,
			std::function<void(std::vector<T>&, int)> bufferAccessor,
			std::vector<float>&                       xData, std::vector<float>& yData
		);
	};


#include <chrono>


	constexpr auto STEP_COUNT = 11;
	//constexpr auto BUFFER_SIZE   = 67108864;      // 2^26;
	constexpr auto BUFFER_SIZE = 16777216; // 2^24;

	inline void dae::CacheExercise::TTCInt()
	{
		TrashTheCache<int>(
			m_NrSamples,
			[](auto& buffer, int i) { buffer[i] = 0; },
			m_Graph1Xs, m_Graph1Ys);

		/*
		Test results voor TEST_SIZE=1000, BUFFER_SIZE=67108864

		1; 42256
		2; 26026
		4; 20502
		8; 18269
		16; 18784
		32; 16715
		64; 8927
		128; 4735
		256; 2535
		512; 1373
		1024; 713
		*/
	}


	inline void dae::CacheExercise::TTC3D()
	{
		TrashTheCache<dae::GameObject3D>(
			m_NrSamples2,
			[](auto& buffer, int i) { buffer[i].ID = 0; },
			m_Graph2Xs, m_Graph2Ys);
	}

	inline void dae::CacheExercise::TTTC3DAlt()
	{
		TrashTheCache<GameObject3DAlt>(
			m_NrSamples2,
			[](auto& buffer, int i) { buffer[i].ID = 0; },
			m_Graph3Xs, m_Graph3Ys);
	}


	template <typename T>
	inline void dae::CacheExercise::TrashTheCache(
		const int                                 testSize,
		std::function<void(std::vector<T>&, int)> bufferAccessor,
		std::vector<float>&                       xData, std::vector<float>& yData
	)
	{
		std::cout << "Testing with a sample size of " << testSize << "." << std::endl;

		std::vector<int>  testXAxis{};
		std::vector<long> testResult{};
		testResult.assign(STEP_COUNT, 0);

		for (int testIndex = 0; testIndex < testSize; testIndex++)
		{
			// const auto buffer = new dae::GameObject3D[BUFFER_SIZE_2]{};
			std::vector<T> buffer{};
			buffer.resize(BUFFER_SIZE);

			int stepIndex = 0;
			for (int step = 1; step <= 1024; step *= 2)
			{
				const auto start = std::chrono::high_resolution_clock::now();

				for (int i = 0; i < BUFFER_SIZE; i += step)
				{
					bufferAccessor(buffer, i);
					// auto a = buffer[i];
				}

				const auto end   = std::chrono::high_resolution_clock::now();
				const auto total = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
				testResult[stepIndex] += long(total);

				stepIndex++;
			}

			std::cout << "Test nr. " << testIndex << std::endl;
		}

		// Average our results
		int step = 1;
		for (int stepIndex = 0; stepIndex < STEP_COUNT; stepIndex++)
		{
			testResult[stepIndex] /= testSize;
			std::cout << step << "; " << testResult[stepIndex] << std::endl;
			testXAxis.push_back(step);
			step *= 2;
		}

		// Save the data in the class
		for (const auto val : testXAxis)
		{
			xData.push_back(float(val));
		}
		for (const auto val : testResult)
		{
			yData.push_back(float(val));
		}
	}
};
