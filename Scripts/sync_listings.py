#!/usr/bin/env python3

import sys
from pathlib import Path

LISTING_DEFINITIONS = [
    {
        "source": "Implementações/FlowNetwork/FlowNetwork.hpp",
        "output": "LaTeX/apendices/01_flownetwork.tex",
        "title": "Implementação da Interface Base ``Flow-Network''",
        "label_appendix": "ap:flownetwork",
        "caption": "Implementação em C++ da interface base \\lstinline{FlowNetwork} para a modelagem abstrata de redes de fluxo.",
        "label_listing": "lst:flownetwork",
    },
    {
        "source": "Implementações/FlowNetwork/FordFulkerson.hpp",
        "output": "LaTeX/apendices/02_ford_fulkerson.tex",
        "title": "Implementação da Classe ``Ford-Fulkerson''",
        "label_appendix": "ap:ford_fulkerson",
        "caption": "Implementação em C++ da classe \\lstinline{FordFulkerson}, método genérico de caminhos aumentantes via DFS.",
        "label_listing": "lst:ford_fulkerson",
    },
    {
        "source": "Implementações/FlowNetwork/EdmondsKarp.hpp",
        "output": "LaTeX/apendices/03_edmonds_karp.tex",
        "title": "Implementação da Classe ``Edmonds-Karp''",
        "label_appendix": "ap:edmonds_karp",
        "caption": "Implementação em C++ da classe \\lstinline{EdmondsKarp}, baseada na seleção iterativa de caminhos mais curtos via Busca em Largura (BFS).",
        "label_listing": "lst:edmonds_karp",
    },
    {
        "source": "Implementações/FlowNetwork/Dinic.hpp",
        "output": "LaTeX/apendices/04_dinic.tex",
        "title": "Implementação da Classe ``Dinic''",
        "label_appendix": "ap:dinic",
        "caption": "Implementação em C++ da classe \\lstinline{Dinic}, estruturada com Digrafo de Níveis e otimização de ponteiros mortos no cálculo do fluxo bloqueador.",
        "label_listing": "lst:dinic",
    },
    {
        "source": "Implementações/FlowNetwork/PushRelabel.hpp",
        "output": "LaTeX/apendices/05_push_relabel.tex",
        "title": "Implementação da Classe ``Push-Relabel''",
        "label_appendix": "ap:push_relabel",
        "caption": "Implementação em C++ da classe \\lstinline{PushRelabel}, baseada em pré-fluxos topológicos e política de processamento em fila (FIFO).",
        "label_listing": "lst:push_relabel",
    },
    {
        "source": "Implementações/FlowNetwork/PushRelabelImproved.hpp",
        "output": "LaTeX/apendices/06_push_relabel_improved.tex",
        "title": "Implementação da Classe ``Push-Relabel Improved''",
        "label_appendix": "ap:push_relabel_improved",
        "caption": "Implementação em C++ da classe \\lstinline{PushRelabelImproved}, otimizada empiricamente com a Heurística de Lacuna (\\textit{Gap Heuristic}).",
        "label_listing": "lst:push_relabel_improved",
    },
    {
        "source": "Implementações/CostNetwork/CostNetwork.hpp",
        "output": "LaTeX/apendices/07_costnetwork.tex",
        "title": "Implementação da Interface Base ``Cost-Network''",
        "label_appendix": "ap:costnetwork",
        "caption": "Implementação em C++ da interface base \\lstinline{CostNetwork} para a modelagem abstrata de redes de fluxo com custos.",
        "label_listing": "lst:costnetwork",
    },
    {
        "source": "Implementações/CostNetwork/CycleCanceling.hpp",
        "output": "LaTeX/apendices/08_cycle_canceling.tex",
        "title": "Implementação da Classe ``Cycle-Canceling''",
        "label_appendix": "ap:cycle_canceling",
        "caption": "Implementação em C++ do algoritmo \\textit{Cycle Canceling}.",
        "label_listing": "lst:cycle_canceling",
    },
    {
        "source": "Implementações/CostNetwork/SuccessiveShortest.hpp",
        "output": "LaTeX/apendices/09_ssp_spfa.tex",
        "title": "Implementação da Classe ``Successive-Shortest-Path''",
        "label_appendix": "ap:successive_shortest",
        "caption": "Implementação em C++ do algoritmo \\textit{Successive Shortest Path} com SPFA.",
        "label_listing": "lst:successive_shortest",
    },
    {
        "source": "Implementações/CostNetwork/SuccessiveShortestDijkstra.hpp",
        "output": "LaTeX/apendices/10_ssp_dijkstra.tex",
        "title": "Implementação da Classe ``Successive-Shortest-Path'' com Dijkstra e Potenciais",
        "label_appendix": "ap:successive_shortest_dijkstra",
        "caption": "Implementação em C++ do algoritmo \\textit{Successive Shortest Path} com Potenciais Nodais e Dijkstra.",
        "label_listing": "lst:successive_shortest_dijkstra",
    },
    {
        "source": "Implementações/CostNetwork/NetworkSimplex.hpp",
        "output": "LaTeX/apendices/11_network_simplex.tex",
        "title": "Implementação da Classe ``Network-Simplex''",
        "label_appendix": "ap:network_simplex",
        "caption": "Implementação em C++ do algoritmo \\textit{Network Simplex}.",
        "label_listing": "lst:network_simplex",
    },
]


def generate_appendix_content(title: str, label_appendix: str, caption: str, label_listing: str, source_code: str) -> str:
    separator = "% " + "=" * 73
    return (
        f"{separator}\n"
        f"% {title.upper()}\n"
        f"{separator}\n"
        f"\\apendice{{{title}}}\n"
        f"\\label{{{label_appendix}}}\n\n"
        f"\\begin{{lstlisting}}[caption={{{caption}}}, label={{{label_listing}}}]\n"
        f"{source_code.strip()}\n"
        f"\\end{{lstlisting}}\n"
    )


def find_repo_root() -> Path:
    current = Path.cwd().resolve()
    for directory in [current, *current.parents]:
        if (directory / ".git").exists() or (directory / "Implementações").exists():
            return directory
    return current


def sync_listings(check_only: bool = False) -> bool:
    repo_root = find_repo_root()
    is_in_sync = True

    for definition in LISTING_DEFINITIONS:
        source_path = repo_root / definition["source"]
        output_path = repo_root / definition["output"]

        if not source_path.exists():
            sys.stderr.write(f"Source file not found: {source_path}\n")
            return False

        source_code = source_path.read_text(encoding="utf-8")
        expected_content = generate_appendix_content(
            definition["title"],
            definition["label_appendix"],
            definition["caption"],
            definition["label_listing"],
            source_code,
        )

        if check_only:
            if not output_path.exists():
                sys.stderr.write(f"Missing appendix file: {output_path}\n")
                is_in_sync = False
            else:
                current_content = output_path.read_text(encoding="utf-8")
                if current_content != expected_content:
                    sys.stderr.write(f"Listing out of sync: {output_path} with {source_path}\n")
                    is_in_sync = False
        else:
            output_path.parent.mkdir(parents=True, exist_ok=True)
            output_path.write_text(expected_content, encoding="utf-8")

    return is_in_sync


def main() -> None:
    check_mode = "--check" in sys.argv
    success = sync_listings(check_only=check_mode)

    if not success:
        sys.exit(1)


if __name__ == "__main__":
    main()
