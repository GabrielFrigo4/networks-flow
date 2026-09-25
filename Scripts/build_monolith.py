#!/usr/bin/env python3

import re
import sys
from pathlib import Path


def find_repo_root() -> Path:
    current = Path.cwd().resolve()
    for directory in [current, *current.parents]:
        if (directory / ".git").exists() or (directory / "LaTeX").exists():
            return directory
    return current


def resolve_inputs(content: str, latex_dir: Path) -> str:
    input_pattern = re.compile(r"^\s*\\input\{([^}]+)\}", re.MULTILINE)

    def replacer(match: re.Match) -> str:
        input_file_str = match.group(1).strip()
        if not input_file_str.endswith(".tex"):
            input_file_str += ".tex"

        target_path = (latex_dir / input_file_str).resolve()

        if not target_path.exists():
            sys.stderr.write(f"Aviso: arquivo referenciado em \\input não encontrado: {input_file_str} em {target_path}\n")
            return match.group(0)

        child_content = target_path.read_text(encoding="utf-8")
        return resolve_inputs(child_content, latex_dir)

    return input_pattern.sub(replacer, content)


def generate_monolith(check_only: bool = False) -> bool:
    repo_root = find_repo_root()
    latex_dir = repo_root / "LaTeX"
    entry_point = latex_dir / "relatorio.tex"
    output_path = latex_dir / "monolito.tex"

    if not entry_point.exists():
        sys.stderr.write(f"Arquivo de entrada não encontrado: {entry_point}\n")
        return False

    initial_content = entry_point.read_text(encoding="utf-8")
    monolith_content = resolve_inputs(initial_content, latex_dir)

    # Cabeçalho padronizado para o monolito
    header_banner = (
        "% =========================================================================\n"
        "% RELATÓRIO DE INICIAÇÃO CIENTÍFICA (COM CAPA INSTITUCIONAL UFABC)\n"
        "% =========================================================================\n\n"
    )

    clean_monolith = monolith_content.strip()

    # Se o arquivo .bbl existir, embute a bibliografia compilada no monólito (100% autocontido)
    bbl_path = latex_dir / "build" / "relatorio.bbl"
    if bbl_path.exists():
        bbl_content = bbl_path.read_text(encoding="utf-8").strip()
        bib_pattern = re.compile(
            r"\\bibliographystyle\{[^}]+\}\s*\\bibliography\{[^}]+\}",
            re.MULTILINE,
        )
        clean_monolith = bib_pattern.sub(lambda _: bbl_content, clean_monolith)

    # Normaliza quebras de linha excessivas e garante encerramento limpo antes de \end{document}
    clean_monolith = re.sub(r"\n{3,}", "\n\n", clean_monolith)
    clean_monolith = re.sub(r"\n+\\end\{document\}", r"\n\n\\end{document}", clean_monolith)

    if clean_monolith.startswith("% ========================================================================="):
        final_content = clean_monolith + "\n"
    else:
        final_content = header_banner + clean_monolith + "\n"

    if check_only:
        if not output_path.exists():
            sys.stderr.write(f"Arquivo monolito não encontrado: {output_path}\n")
            return False
        current_content = output_path.read_text(encoding="utf-8")
        if current_content != final_content:
            sys.stderr.write(f"Monólito desatualizado em relação às fontes modulares!\n")
            return False
        return True

    output_path.write_text(final_content, encoding="utf-8")
    return True


def main() -> None:
    check_mode = "--check" in sys.argv
    success = generate_monolith(check_only=check_mode)
    if not success:
        sys.exit(1)


if __name__ == "__main__":
    main()
