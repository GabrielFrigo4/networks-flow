// Gerador de imagens de teste PPM para o segmentador
// Cria imagens simples com regiões bem definidas de foreground/background

#include <fstream>
#include <iostream>
#include <string>

void generate_10x10()
{
	// Imagem 10x10: metade esquerda branca (foreground), metade direita preta
	// (background)
	std::ofstream img("simple_10x10.ppm");
	img << "P3\n10 10\n255\n";
	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 10; x++)
		{
			if (x < 5)
				img << "255 255 255 "; // branco (foreground)
			else
				img << "30 30 30 "; // quase preto (background)
		}
		img << "\n";
	}
	img.close();

	// Seeds correspondentes
	std::ofstream seeds("simple_10x10_seeds.txt");
	seeds << "# Seeds para simple_10x10.ppm\n";
	seeds << "# Formato: x y label (F=foreground, B=background)\n";
	// Marcar coluna 1 como foreground
	for (int y = 0; y < 10; y++)
		seeds << "1 " << y << " F\n";
	// Marcar coluna 8 como background
	for (int y = 0; y < 10; y++)
		seeds << "8 " << y << " B\n";
	seeds.close();

	std::cout << "Gerado: simple_10x10.ppm + simple_10x10_seeds.txt" << std::endl;
}

void generate_20x20()
{
	// Imagem 20x20: círculo vermelho central (foreground) em fundo azul (background)
	std::ofstream img("circle_20x20.ppm");
	img << "P3\n20 20\n255\n";
	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			int dx = x - 10;
			int dy = y - 10;
			if (dx * dx + dy * dy <= 36) // raio 6
				img << "220 50 50 ";     // vermelho (foreground)
			else
				img << "50 50 200 "; // azul (background)
		}
		img << "\n";
	}
	img.close();

	// Seeds correspondentes
	std::ofstream seeds("circle_20x20_seeds.txt");
	seeds << "# Seeds para circle_20x20.ppm\n";
	seeds << "# Centro como foreground\n";
	seeds << "10 10 F\n";
	seeds << "9 10 F\n";
	seeds << "11 10 F\n";
	seeds << "10 9 F\n";
	seeds << "10 11 F\n";
	seeds << "# Cantos como background\n";
	seeds << "0 0 B\n";
	seeds << "19 0 B\n";
	seeds << "0 19 B\n";
	seeds << "19 19 B\n";
	seeds << "0 10 B\n";
	seeds << "19 10 B\n";
	seeds << "10 0 B\n";
	seeds << "10 19 B\n";
	seeds.close();

	std::cout << "Gerado: circle_20x20.ppm + circle_20x20_seeds.txt" << std::endl;
}

int main()
{
	generate_10x10();
	generate_20x20();
	std::cout << "Todas as imagens de teste foram geradas!" << std::endl;
	return 0;
}
