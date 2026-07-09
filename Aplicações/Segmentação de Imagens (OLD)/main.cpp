// ============================================================================
// Segmentação de Imagens via Max-Flow / Min-Cut
// ============================================================================
//
// Aplicação prática de fluxos em redes para separar o foreground (objeto
// principal) do background (fundo) em fotografias. Utiliza a modelagem de
// corte mínimo em grafos onde:
//
//   - Cada pixel é um vértice
//   - N-links conectam pixels vizinhos (4-conectividade)
//   - T-links conectam cada pixel à super-fonte (foreground) e ao
//     super-sumidouro (background)
//
// O corte mínimo (Min-Cut) na rede identifica a partição ótima dos pixels
// em dois conjuntos disjuntos: foreground e background.
//
// Formato de entrada:
//   - Imagem: PPM (P3 ou P6) - formato sem dependências externas
//   - Seeds: arquivo texto com marcações do usuário (pixel_x pixel_y label)
//     onde label é 'F' para foreground ou 'B' para background
//
// Formato de saída:
//   - Imagem PPM com o foreground preservado e o background escurecido
//
// Plataformas: Windows, MSYS2, Linux, FreeBSD
// ============================================================================

#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

#include "../../Implementações/Dinic.hpp"
#include "../../Implementações/EdmondsKarp.hpp"
#include "../../Implementações/FordFulkerson.hpp"
#include "../../Implementações/PushRelabel.hpp"
#include "../../Implementações/PushRelabelImproved.hpp"

using FlowSolver = PushRelabelImproved;

// ============================================================================
// Estruturas de Dados
// ============================================================================

struct Pixel
{
	unsigned char r, g, b;
};

struct Image
{
	Size width, height;
	std::vector<Pixel> data;

	[[nodiscard]] Size index(const Size x, const Size y) const
	{
		return y * width + x;
	}

	[[nodiscard]] const Pixel &at(const Size x, const Size y) const
	{
		return data[index(x, y)];
	}
};

enum class SeedLabel
{
	NONE,
	FOREGROUND,
	BACKGROUND
};

struct Seed
{
	Size x, y;
	SeedLabel label;
};

// ============================================================================
// Leitor e Escritor de Imagens PPM
// ============================================================================

Image read_ppm(const std::string &filename)
{
	std::ifstream file(filename, std::ios::binary);
	if (!file.is_open())
	{
		std::cerr << "Erro: Não foi possível abrir o arquivo '" << filename << "'"
		          << std::endl;
		std::exit(1);
	}

	std::string magic;
	file >> magic;

	if (magic != "P3" && magic != "P6")
	{
		std::cerr << "Erro: Formato PPM inválido (esperado P3 ou P6, obteve '"
		          << magic << "')" << std::endl;
		std::exit(1);
	}

	Image img;
	Size max_val;

	// Pular comentários
	char c;
	file.get(c);
	while (file.peek() == '#')
	{
		std::string comment;
		std::getline(file, comment);
	}

	file >> img.width >> img.height >> max_val;
	img.data.resize(img.width * img.height);

	if (magic == "P3")
	{
		// Formato ASCII
		for (Size i = 0; i < img.width * img.height; i++)
		{
			int r, g, b;
			file >> r >> g >> b;
			img.data[i] = {
			    static_cast<unsigned char>(r),
			    static_cast<unsigned char>(g),
			    static_cast<unsigned char>(b)
			};
		}
	}
	else
	{
		// Formato binário P6
		file.get(c); // pular whitespace após max_val
		for (Size i = 0; i < img.width * img.height; i++)
		{
			file.read(reinterpret_cast<char *>(&img.data[i]), 3);
		}
	}

	std::cout << "Imagem carregada: " << img.width << "x" << img.height << " ("
	          << img.width * img.height << " pixels)" << std::endl;

	return img;
}

void write_ppm(const std::string &filename, const Image &img)
{
	std::ofstream file(filename, std::ios::binary);
	if (!file.is_open())
	{
		std::cerr << "Erro: Não foi possível criar o arquivo '" << filename << "'"
		          << std::endl;
		std::exit(1);
	}

	file << "P6\n" << img.width << " " << img.height << "\n255\n";
	for (Size i = 0; i < img.width * img.height; i++)
	{
		file.write(reinterpret_cast<const char *>(&img.data[i]), 3);
	}

	std::cout << "Resultado salvo em: " << filename << std::endl;
}

// ============================================================================
// Leitor de Seeds (Marcações)
// ============================================================================

std::vector<Seed> read_seeds(const std::string &filename)
{
	std::ifstream file(filename);
	if (!file.is_open())
	{
		std::cerr << "Erro: Não foi possível abrir o arquivo de seeds '" << filename
		          << "'" << std::endl;
		std::exit(1);
	}

	std::vector<Seed> seeds;
	std::string line;

	while (std::getline(file, line))
	{
		if (line.empty() || line[0] == '#')
			continue;

		std::istringstream iss(line);
		Size x, y;
		char label_char;

		if (!(iss >> x >> y >> label_char))
			continue;

		SeedLabel label = SeedLabel::NONE;
		if (label_char == 'F' || label_char == 'f')
			label = SeedLabel::FOREGROUND;
		else if (label_char == 'B' || label_char == 'b')
			label = SeedLabel::BACKGROUND;

		if (label != SeedLabel::NONE)
		{
			seeds.push_back({x, y, label});
		}
	}

	Size fg_count = 0, bg_count = 0;
	for (const auto &s : seeds)
	{
		if (s.label == SeedLabel::FOREGROUND)
			fg_count++;
		else
			bg_count++;
	}

	std::cout << "Seeds carregados: " << seeds.size() << " (" << fg_count
	          << " foreground, " << bg_count << " background)" << std::endl;

	return seeds;
}

// ============================================================================
// Construção do Grafo de Segmentação
// ============================================================================

// Diferença quadrática entre dois pixels (distância euclidiana no espaço RGB)
double pixel_distance(const Pixel &a, const Pixel &b)
{
	const double dr = static_cast<double>(a.r) - static_cast<double>(b.r);
	const double dg = static_cast<double>(a.g) - static_cast<double>(b.g);
	const double db = static_cast<double>(a.b) - static_cast<double>(b.b);
	return std::sqrt(dr * dr + dg * dg + db * db);
}

struct SegmentationGraph
{
	Size source; // super-fonte (foreground)
	Size sink;   // super-sumidouro (background)
	std::unique_ptr<FlowNetwork> network;
};

SegmentationGraph build_graph(
    const Image &img, const std::vector<Seed> &seeds, const double sigma = 30.0
)
{
	const Size num_pixels = img.width * img.height;
	const Size source = num_pixels;   // super-fonte
	const Size sink = num_pixels + 1; // super-sumidouro
	const Size total_nodes = num_pixels + 2;

	auto fn = FlowSolver::create(total_nodes);

	// Mapa de seeds para acesso rápido
	std::vector<SeedLabel> seed_map(num_pixels, SeedLabel::NONE);
	for (const auto &seed : seeds)
	{
		if (seed.x < img.width && seed.y < img.height)
		{
			seed_map[img.index(seed.x, seed.y)] = seed.label;
		}
	}

	// ================================================================
	// N-links: Arestas entre pixels vizinhos (4-conectividade)
	// Peso baseado na similaridade de cor: w(p,q) = K * exp(-||Ip-Iq||² / 2σ²)
	// ================================================================
	const Long K = 1000; // fator de escala para inteiros
	const double two_sigma_sq = 2.0 * sigma * sigma;

	// Direções: direita, baixo (evita duplicar arestas bidirecionais)
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

				fn->add_edge(p, q, std::max(weight, static_cast<Long>(1)));
			}
		}
	}

	// ================================================================
	// T-links: Arestas de/para os terminais (source e sink)
	// Seeds marcados recebem peso infinito (forçando a partição)
	// Pixels não marcados recebem peso baseado em modelo simples
	// ================================================================

	// Calcular cor média do foreground e background a partir dos seeds
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

	Pixel fg_mean = {0, 0, 0};
	Pixel bg_mean = {0, 0, 0};

	if (fg_count > 0)
	{
		fg_mean = {
		    static_cast<unsigned char>(fg_r / static_cast<double>(fg_count)),
		    static_cast<unsigned char>(fg_g / static_cast<double>(fg_count)),
		    static_cast<unsigned char>(fg_b / static_cast<double>(fg_count))
		};
	}

	if (bg_count > 0)
	{
		bg_mean = {
		    static_cast<unsigned char>(bg_r / static_cast<double>(bg_count)),
		    static_cast<unsigned char>(bg_g / static_cast<double>(bg_count)),
		    static_cast<unsigned char>(bg_b / static_cast<double>(bg_count))
		};
	}

	for (Size i = 0; i < num_pixels; i++)
	{
		if (seed_map[i] == SeedLabel::FOREGROUND)
		{
			// Seed de foreground: conexão forte com source
			fn->add_edge(source, i, INF);
			fn->add_edge(i, sink, 0);
		}
		else if (seed_map[i] == SeedLabel::BACKGROUND)
		{
			// Seed de background: conexão forte com sink
			fn->add_edge(source, i, 0);
			fn->add_edge(i, sink, INF);
		}
		else
		{
			// Pixel não marcado: peso baseado na distância ao modelo de cor
			const Pixel &px = img.data[i];
			const double dist_fg = pixel_distance(px, fg_mean);
			const double dist_bg = pixel_distance(px, bg_mean);

			// Probabilidade proporcional à distância inversa da respectiva cor média
			const Long w_source = static_cast<Long>(
			    K * std::exp(-dist_fg * dist_fg / two_sigma_sq)
			);
			const Long w_sink = static_cast<Long>(
			    K * std::exp(-dist_bg * dist_bg / two_sigma_sq)
			);

			fn->add_edge(source, i, std::max(w_source, static_cast<Long>(1)));
			fn->add_edge(i, sink, std::max(w_sink, static_cast<Long>(1)));
		}
	}

	return {source, sink, std::move(fn)};
}

// ============================================================================
// Extração do Corte e Geração da Imagem de Saída
// ============================================================================

std::vector<bool> extract_foreground_mask(
    const SegmentationGraph &graph, const Size num_pixels
)
{
	// BFS na rede residual a partir da source
	std::vector<bool> reachable(num_pixels + 2, false);
	std::queue<Size> q;

	q.push(graph.source);
	reachable[graph.source] = true;

	const auto &edges = graph.network->get_edges();
	const auto &adj = graph.network->get_adj();

	while (!q.empty())
	{
		const Size u = q.front();
		q.pop();

		for (const Size edge_id : adj[u])
		{
			const auto &edge = edges[edge_id];
			if (edge.capacity - edge.flow <= 0 || reachable[edge.to])
				continue;

			reachable[edge.to] = true;
			q.push(edge.to);
		}
	}

	// Pixels alcançáveis desde source = foreground
	std::vector<bool> mask(num_pixels);
	for (Size i = 0; i < num_pixels; i++)
	{
		mask[i] = reachable[i];
	}

	return mask;
}

Image apply_mask(const Image &original, const std::vector<bool> &fg_mask)
{
	Image result = original;

	Size fg_pixels = 0, bg_pixels = 0;
	for (Size i = 0; i < original.width * original.height; i++)
	{
		if (fg_mask[i])
		{
			fg_pixels++;
		}
		else
		{
			// Fundo com cor chamativa (Verde "Chroma Key") para deixar nítido o
			// corte
			result.data[i].r = 0;
			result.data[i].g = 255;
			result.data[i].b = 0;
			bg_pixels++;
		}
	}

	std::cout << "Segmentação: " << fg_pixels << " pixels foreground, " << bg_pixels
	          << " pixels background" << std::endl;

	return result;
}

// ============================================================================
// Interface CLI
// ============================================================================

void print_usage(const char *program_name)
{
	std::cout << "========================================================\n"
	          << "  Segmentação de Imagens via Max-Flow / Min-Cut\n"
	          << "  IC: Problemas de Fluxos em Redes\n"
	          << "========================================================\n\n"
	          << "Uso:\n"
	          << "  " << program_name
	          << " --input <imagem.ppm> --seeds <seeds.txt> --output <saida.ppm>\n\n"
	          << "Parâmetros:\n"
	          << "  --input   Imagem de entrada no formato PPM (P3 ou P6)\n"
	          << "  --seeds   Arquivo de seeds com marcações do usuário\n"
	          << "  --output  Imagem de saída segmentada (PPM P6)\n"
	          << "  --sigma   Parâmetro sigma para a função de peso (padrão: 30.0)\n"
	          << "  --help    Exibir esta mensagem de ajuda\n\n"
	          << "Formato do arquivo de seeds:\n"
	          << "  Cada linha: x y label\n"
	          << "  onde label é 'F' (foreground) ou 'B' (background)\n"
	          << "  Linhas começando com '#' são comentários\n\n"
	          << "Exemplo:\n"
	          << "  " << program_name
	          << " --input foto.ppm --seeds marcacoes.txt --output resultado.ppm\n"
	          << std::endl;
}

int main(int argc, char *argv[])
{
	std::string input_file;
	std::string seeds_file;
	std::string output_file = "output_segmented.ppm";
	double sigma = 30.0;

	if (argc == 1)
	{
		std::cout << "========================================================\n"
		          << "  Segmentação de Imagens (Modo Interativo)\n"
		          << "========================================================\n";

		std::cout << "Digite o caminho da imagem de entrada (.ppm): ";
		std::getline(std::cin, input_file);

		std::cout << "Digite o caminho do arquivo de seeds (.txt): ";
		std::getline(std::cin, seeds_file);

		std::string tmp_output;
		std::cout << "Digite o caminho da imagem de saída [padrão: "
		             "output_segmented.ppm]: ";
		std::getline(std::cin, tmp_output);
		if (!tmp_output.empty())
			output_file = tmp_output;

		std::string tmp_sigma;
		std::cout << "Digite o valor de sigma [padrão: 30.0]: ";
		std::getline(std::cin, tmp_sigma);
		if (!tmp_sigma.empty())
			sigma = std::stod(tmp_sigma);
	}
	else
	{
		for (int i = 1; i < argc; i++)
		{
			if (std::strcmp(argv[i], "--help") == 0 ||
			    std::strcmp(argv[i], "-h") == 0)
			{
				print_usage(argv[0]);
				return 0;
			}
			else if (std::strcmp(argv[i], "--input") == 0 && i + 1 < argc)
			{
				input_file = argv[++i];
			}
			else if (std::strcmp(argv[i], "--seeds") == 0 && i + 1 < argc)
			{
				seeds_file = argv[++i];
			}
			else if (std::strcmp(argv[i], "--output") == 0 && i + 1 < argc)
			{
				output_file = argv[++i];
			}
			else if (std::strcmp(argv[i], "--sigma") == 0 && i + 1 < argc)
			{
				sigma = std::atof(argv[++i]);
			}
		}
	}

	if (input_file.empty() || seeds_file.empty())
	{
		std::cerr << "Erro: --input e --seeds são obrigatórios.\n" << std::endl;
		print_usage(argv[0]);
		return 1;
	}

	// 1. Carregar imagem e seeds
	std::cout << "\n[1/4] Carregando imagem e seeds..." << std::endl;
	const Image img = read_ppm(input_file);
	const auto seeds = read_seeds(seeds_file);

	// 2. Construir grafo de segmentação
	std::cout << "\n[2/4] Construindo grafo de segmentação..." << std::endl;
	auto graph = build_graph(img, seeds, sigma);
	const Size num_pixels = img.width * img.height;
	const Size num_edges = graph.network->get_edges().size();
	std::cout << "Grafo construído: " << (num_pixels + 2) << " vértices, "
	          << num_edges / 2 << " arestas" << std::endl;

	// 3. Computar Min-Cut via Max-Flow (Push-Relabel Improved)
	std::cout << "\n[3/4] Computando Min-Cut (Push-Relabel com Gap Heuristic)..."
	          << std::endl;
	const Long max_flow = graph.network->compute_max_flow(graph.source, graph.sink);
	std::cout << "Fluxo Máximo / Corte Mínimo: " << max_flow << std::endl;

	// 4. Extrair máscara e gerar imagem de saída
	std::cout << "\n[4/4] Gerando imagem segmentada..." << std::endl;
	const auto fg_mask = extract_foreground_mask(graph, num_pixels);
	const Image result = apply_mask(img, fg_mask);
	write_ppm(output_file, result);

	std::cout << "\nSegmentação concluída com sucesso!" << std::endl;
	return 0;
}
