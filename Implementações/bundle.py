#!/usr/bin/env python3

import os
import re
import sys

SYS_INCLUDE_RE = re.compile(r'^\s*#\s*include\s*<([^>]+)>')
LOCAL_INCLUDE_RE = re.compile(r'^\s*#\s*include\s*"([^"]+)"')
PRAGMA_ONCE_RE = re.compile(r'^\s*#\s*pragma\s+once')
IFNDEF_RE = re.compile(r'^\s*#\s*ifndef\s+([A-Za-z0-9_]+)')
DEFINE_RE = re.compile(r'^\s*#\s*define\s+([A-Za-z0-9_]+)')
ENDIF_RE = re.compile(r'^\s*#\s*endif')


def bundle_file(entry_file: str) -> str:
    system_includes = set()
    visited_files = set()
    output_lines = []

    def process_file(filepath: str, is_root: bool = False):
        abs_path = os.path.abspath(filepath)
        if abs_path in visited_files:
            return
        visited_files.add(abs_path)

        file_dir = os.path.dirname(abs_path)

        try:
            with open(abs_path, 'r', encoding='utf-8') as f:
                content = f.readlines()
        except Exception as e:
            print(f"// Error reading {filepath}: {e}", file=sys.stderr)
            return

        lines_to_add = []
        guard_symbol = None
        guard_active = False

        for i, raw_line in enumerate(content):
            line = raw_line.rstrip('\r\n')
            stripped = line.strip()

            if PRAGMA_ONCE_RE.match(stripped):
                continue

            if not is_root and guard_symbol is None:
                m_if = IFNDEF_RE.match(stripped)
                if m_if and i < 5:
                    guard_symbol = m_if.group(1)
                    guard_active = True
                    continue

            if guard_active and guard_symbol is not None:
                m_def = DEFINE_RE.match(stripped)
                if m_def and m_def.group(1) == guard_symbol:
                    guard_active = False
                    continue

            m_sys = SYS_INCLUDE_RE.match(stripped)
            if m_sys:
                system_includes.add(m_sys.group(1))
                continue

            m_loc = LOCAL_INCLUDE_RE.match(stripped)
            if m_loc:
                rel_inc = m_loc.group(1)
                full_inc_path = os.path.join(file_dir, rel_inc)
                process_file(full_inc_path, is_root=False)
                continue

            lines_to_add.append(raw_line)

        if guard_symbol is not None and lines_to_add:
            for j in range(len(lines_to_add) - 1, -1, -1):
                if ENDIF_RE.match(lines_to_add[j].strip()):
                    lines_to_add.pop(j)
                    break

        if not is_root:
            while lines_to_add and not lines_to_add[0].strip():
                lines_to_add.pop(0)
            while lines_to_add and not lines_to_add[-1].strip():
                lines_to_add.pop()
            if lines_to_add:
                lines_to_add.append('\n\n')

        output_lines.extend(lines_to_add)

    process_file(entry_file, is_root=True)

    header = []
    for inc in sorted(system_includes):
        header.append(f"#include <{inc}>\n")

    full_text = "".join(header) + "\n" + "".join(output_lines)
    full_text = re.sub(r'\n{3,}', '\n\n', full_text)
    full_text = full_text.strip() + "\n"

    return full_text


def main():
    if len(sys.argv) < 2:
        print("Usage: bundle.py <entry_file.cpp> [-o output.cpp]", file=sys.stderr)
        sys.exit(1)

    entry_file = sys.argv[1]
    output_file = None

    if "-o" in sys.argv:
        idx = sys.argv.index("-o")
        if idx + 1 < len(sys.argv):
            output_file = sys.argv[idx + 1]

    bundled_code = bundle_file(entry_file)

    if output_file:
        with open(output_file, 'w', encoding='utf-8') as f:
            f.write(bundled_code)
    else:
        sys.stdout.write(bundled_code)


if __name__ == '__main__':
    main()
