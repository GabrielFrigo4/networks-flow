#pragma once

#include <cmath>
#include <memory>
#include <queue>
#include <vector>

#include "../../../Implementações/Dinic.hpp"
#include "../../../Implementações/EdmondsKarp.hpp"
#include "../../../Implementações/FlowNetwork.hpp"
#include "../../../Implementações/FordFulkerson.hpp"
#include "../../../Implementações/PushRelabel.hpp"
#include "../../../Implementações/PushRelabelImproved.hpp"

#include "image.hpp"
#include "seeds.hpp"

using FlowSolver = PushRelabelImproved;

struct SegmentationGraph
{
	Size source;
	Size sink;
	std::unique_ptr<FlowNetwork> network;
};

inline double pixel_distance(const Pixel &a, const Pixel &b)
{
	const double dr = static_cast<double>(a.r) - static_cast<double>(b.r);
	const double dg = static_cast<double>(a.g) - static_cast<double>(b.g);
	const double db = static_cast<double>(a.b) - static_cast<double>(b.b);
	return std::sqrt(dr * dr + dg * dg + db * db);
}

inline SegmentationGraph build_graph(
    const Image &img, const std::vector<Seed> &seeds, const double sigma = 30.0
)
{
	const Size num_pixels = img.width * img.height;
	const Size source = num_pixels;
	const Size sink = num_pixels + 1;
	const Size total = num_pixels + 2;

	auto network = FlowSolver::create(total);

	std::vector<SeedLabel> seed_map(num_pixels, SeedLabel::NONE);
	for (const auto &seed : seeds)
	{
		if (seed.x < img.width && seed.y < img.height)
			seed_map[img.index(seed.x, seed.y)] = seed.label;
	}

	const Long K = 1000;
	const double two_sigma_sq = 2.0 * sigma * sigma;
	const int dx[] = {1, 0, -1, 0};
	const int dy[] = {0, 1, 0, -1};

	for (Size y = 0; y < img.height; y++)
	{
		for (Size x = 0; x < img.width; x++)
		{
			const Size p = img.index(x, y);

			for (int d = 0; d < 4; d++)
			{
				const int nx = static_cast<int>(x) + dx[d];
				const int ny = static_cast<int>(y) + dy[d];

				if (nx < 0 || nx >= static_cast<int>(img.width) || ny < 0 ||
				    ny >= static_cast<int>(img.height))
					continue;

				const Size q = img.index(
				    static_cast<Size>(nx), static_cast<Size>(ny)
				);
				const double dist = pixel_distance(
				    img.at(x, y),
				    img.at(static_cast<Size>(nx), static_cast<Size>(ny))
				);
				const Long weight = static_cast<Long>(
				    K * std::exp(-dist * dist / two_sigma_sq)
				);

				network->add_edge(p, q, std::max(weight, static_cast<Long>(1)));
			}
		}
	}

	double fg_r = 0, fg_g = 0, fg_b = 0;
	double bg_r = 0, bg_g = 0, bg_b = 0;
	Size fg_count = 0, bg_count = 0;

	for (const auto &seed : seeds)
	{
		if (seed.x >= img.width || seed.y >= img.height)
			continue;

		const Pixel &px = img.at(seed.x, seed.y);
		if (seed.label == SeedLabel::FOREGROUND)
		{
			fg_r += px.r;
			fg_g += px.g;
			fg_b += px.b;
			fg_count++;
		}
		else
		{
			bg_r += px.r;
			bg_g += px.g;
			bg_b += px.b;
			bg_count++;
		}
	}

	const Pixel fg_mean =
	    fg_count > 0
	        ? Pixel{static_cast<unsigned char>(fg_r / fg_count), static_cast<unsigned char>(fg_g / fg_count), static_cast<unsigned char>(fg_b / fg_count)}
	        : Pixel{0, 0, 0};

	const Pixel bg_mean =
	    bg_count > 0
	        ? Pixel{static_cast<unsigned char>(bg_r / bg_count), static_cast<unsigned char>(bg_g / bg_count), static_cast<unsigned char>(bg_b / bg_count)}
	        : Pixel{0, 0, 0};

	for (Size i = 0; i < num_pixels; i++)
	{
		if (seed_map[i] == SeedLabel::FOREGROUND)
		{
			network->add_edge(source, i, INF);
			network->add_edge(i, sink, 0);
		}
		else if (seed_map[i] == SeedLabel::BACKGROUND)
		{
			network->add_edge(source, i, 0);
			network->add_edge(i, sink, INF);
		}
		else
		{
			const Pixel &px = img.data[i];
			const double dist_fg = pixel_distance(px, fg_mean);
			const double dist_bg = pixel_distance(px, bg_mean);

			const Long w_source = static_cast<Long>(
			    K * std::exp(-dist_fg * dist_fg / two_sigma_sq)
			);
			const Long w_sink = static_cast<Long>(
			    K * std::exp(-dist_bg * dist_bg / two_sigma_sq)
			);

			network->add_edge(source, i, std::max(w_source, static_cast<Long>(1)));
			network->add_edge(i, sink, std::max(w_sink, static_cast<Long>(1)));
		}
	}

	return {source, sink, std::move(network)};
}

[[nodiscard]] inline std::vector<bool> extract_foreground_mask(
    const SegmentationGraph &graph, const Size num_pixels
)
{
	std::vector<bool> reachable(num_pixels + 2, false);
	std::queue<Size> q;

	q.push(graph.source);
	reachable[graph.source] = true;

	const auto &edges = graph.network->get_edges();
	const auto &adjacency = graph.network->get_adjacency();

	while (!q.empty())
	{
		const Size u = q.front();
		q.pop();

		for (const Size edge_id : adjacency[u])
		{
			const auto &edge = edges[edge_id];
			if (edge.capacity - edge.flow <= 0 || reachable[edge.to])
				continue;

			reachable[edge.to] = true;
			q.push(edge.to);
		}
	}

	std::vector<bool> mask(num_pixels);
	for (Size i = 0; i < num_pixels; i++)
		mask[i] = reachable[i];

	return mask;
}

inline Image apply_mask(const Image &original, const std::vector<bool> &fg_mask)
{
	Image result = original;

	for (Size i = 0; i < original.width * original.height; i++)
	{
		if (!fg_mask[i])
		{
			result.data[i].r = 0;
			result.data[i].g = 255;
			result.data[i].b = 0;
		}
	}

	return result;
}
