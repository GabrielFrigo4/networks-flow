// Template de Problema de Fluxo em Redes
// Inclui todos os algoritmos disponíveis na biblioteca.
// Para trocar o algoritmo, basta alterar o 'using' abaixo.

#include "../../Dinic.hpp"
#include "../../EdmondsKarp.hpp"
#include "../../FordFulkerson.hpp"
#include "../../PushRelabel.hpp"
#include "../../PushRelabelImproved.hpp"
#include <iostream>

// ============================================================
// Selecione o algoritmo de fluxo desejado aqui:
using FlowSolver = PushRelabelImproved;
// Opções: FordFulkerson, EdmondsKarp, Dinic, PushRelabel, PushRelabelImproved
// ============================================================

void task()
{
	Size num_nodes, num_edges;
	if (!(std::cin >> num_nodes >> num_edges))
		return;

	const auto fn = FlowSolver::create(num_nodes);
	for (Size k = 0; k < num_edges; k++)
	{
		Size from_node, to_node;
		Long capacity;
		std::cin >> from_node >> to_node >> capacity;
		fn->add_edge(from_node - 1, to_node - 1, capacity);
	}

	std::cout << fn->compute_max_flow(0, num_nodes - 1) << std::endl;
}

int main(void)
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	task();
	return 0;
}
