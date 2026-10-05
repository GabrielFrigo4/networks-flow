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
    input_pattern = re.compile(r"(?<!%)\\input\{([^}]+)\}")

    def replacer(match: re.Match) -> str:
        input_file_str = match.group(1).strip()
        if not input_file_str.endswith(".tex"):
            input_file_str += ".tex"

        target_path = (latex_dir / input_file_str).resolve()

        if not target_path.exists():
            sys.stderr.write(
                f"Aviso: arquivo referenciado em \\input não encontrado: {input_file_str} em {target_path}\n")
            return match.group(0)

        child_content = target_path.read_text(encoding="utf-8").strip()
        return resolve_inputs(child_content, latex_dir)

    return input_pattern.sub(replacer, content)


def format_monolith_content(text: str) -> str:
    text = re.sub(r"([^\n])\n*\\(newpage|clearpage)", r"\1\n\n\\\2", text)
    text = re.sub(r"(\\(?:newpage|clearpage))\n([^\n])", r"\1\n\n\2", text)
    text = re.sub(r"(\\end\{lstlisting\})\s*\n(?!\n)", r"\1\n\n", text)
    lines = text.split("\n")

    formatted_lines = []
    for i, line in enumerate(lines):
        if re.match(r"^\s*%\s*={10,}\s*$", line):
            prev_idx = i - 1
            while prev_idx >= 0 and not lines[prev_idx].strip():
                prev_idx -= 1
            if prev_idx >= 0 and not lines[prev_idx].strip().startswith("%"):
                if formatted_lines and formatted_lines[-1] != "":
                    formatted_lines.append("")
        formatted_lines.append(line)

    text = "\n".join(formatted_lines)
    text = re.sub(r"\n{3,}", "\n\n", text)
    text = re.sub(r"\n+\\end\{document\}", r"\n\n\\end{document}", text)
    return text.strip()


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

    header_banner = (
        "% =========================================================================\n"
        "% RELATÓRIO DE INICIAÇÃO CIENTÍFICA (COM CAPA INSTITUCIONAL UFABC)\n"
        "% =========================================================================\n\n"
    )

    clean_monolith = format_monolith_content(monolith_content)

    if clean_monolith.startswith("% ========================================================================="):
        final_content = clean_monolith + "\n"
    else:
        final_content = header_banner + clean_monolith + "\n"

    if check_only:
        if not output_path.exists():
            sys.stderr.write(
                f"Arquivo monolito não encontrado: {output_path}\n")
            return False
        current_content = output_path.read_text(encoding="utf-8")
        if current_content != final_content:
            sys.stderr.write(
                f"Monólito desatualizado em relação às fontes modulares!\n")
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
