// Problema: School Dance (CSES 1696)
// https://cses.fi/problemset/task/1696
//
// Emparelhamento Bipartido Máximo via super-fonte e super-sumidouro.

#include "../../Dinic.hpp"
#include "../../EdmondsKarp.hpp"
#include "../../FordFulkerson.hpp"
#include "../../PushRelabel.hpp"
#include "../../PushRelabelImproved.hpp"
#include <iostream>

using FlowSolver = PushRelabelImproved;

void print_matches(
    const std::unique_ptr<FlowNetwork> &fn, const Size num_boys, const Size num_girls
)
{
	for (const auto &edge : fn->get_edges())
	{
		const Size num_total = num_boys + num_girls;
		const bool from_boy = (edge.from >= 1 && edge.from <= num_boys);
		const bool to_girl = (edge.to > num_boys && edge.to <= num_total);

		if (!from_boy || !to_girl || edge.flow != 1)
		{
			continue;
		}

		std::cout << edge.from << " " << edge.to - num_boys << std::endl;
	}
}

void task()
{
	Size num_boys, num_girls, num_potential_pairs;
	if (!(std::cin >> num_boys >> num_girls >> num_potential_pairs))
		return;

	const Size total_nodes = num_boys + num_girls + 2;
	const Size source = 0;
	const Size sink = total_nodes - 1;

	const auto fn = FlowSolver::create(total_nodes);

	for (Size i = 1; i <= num_boys; i++)
	{
		fn->add_edge(source, i, 1);
	}

	for (Size i = 1; i <= num_girls; i++)
	{
		fn->add_edge(num_boys + i, sink, 1);
	}

	for (Size i = 0; i < num_potential_pairs; i++)
	{
		Size boy, girl;
		std::cin >> boy >> girl;
		fn->add_edge(boy, num_boys + girl, 1);
	}

	std::cout << fn->compute_max_flow(source, sink) << std::endl;
	print_matches(fn, num_boys, num_girls);
}

int main(void)
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	task();
	return 0;
}
