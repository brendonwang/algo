#!/usr/bin/env python3
"""
yoink: a CLI tool that takes C++ code from a file, removes redundant lines
       (such as "#pragma once"), optionally inlines #include files
       (ensuring each file is only included once), and processes struct
       definitions so their contents are on one line while keeping braces
       separate. The processed code is then copied into the clipboard.

By default, it will look for files relative to ~/workspace/brendon/algo/library,
but you can specify a custom workspace path with the '--workspace' argument.
"""

import argparse
import os
import sys
import re

try:
    import pyperclip
except ImportError:
    print("Error: This script requires the 'pyperclip' package. Install it by running 'pip install pyperclip'.")
    sys.exit(1)


def read_and_inline(filepath: str, visited_includes: set, workspace: str) -> list:
    """
    Reads the file at 'filepath' and returns a list of lines with:
      - #pragma once stripped out.
      - #include "relative_path" files inlined, if not visited before.
    """
    try:
        with open(filepath, 'r') as f:
            original_lines = f.readlines()
    except OSError as e:
        print(f"Error reading file '{filepath}': {e}")
        return []

    processed_lines = []
    for line in original_lines:
        # Skip #pragma once lines
        if re.match(r'^\s*#pragma\s+once\s*$', line):
            continue

        # Check for #include "relative/path"
        match = re.match(r'^\s*#\s*include\s+"([^"]+)"', line)
        if match:
            include_path = match.group(1)
            # Convert to absolute path if it isn't
            abs_inc_path = os.path.join(os.path.dirname(filepath), include_path)
            if not os.path.isabs(abs_inc_path):
                abs_inc_path = os.path.abspath(os.path.join(workspace, include_path))

            # Inline the included file if not visited
            if abs_inc_path not in visited_includes:
                visited_includes.add(abs_inc_path)
                if os.path.exists(abs_inc_path):
                    # Recursively inline
                    included_lines = read_and_inline(abs_inc_path, visited_includes, workspace)
                    processed_lines.extend(included_lines)
                else:
                    # If the file doesn't exist, keep the #include line
                    processed_lines.append(line.rstrip('\n'))  # Else, already visited this include, so skip
        else:
            processed_lines.append(line.rstrip('\n'))

    return processed_lines


def process_struct(lines: list[str]) -> str:
    """
    Processes struct definitions to make their contents one line each
    while keeping braces separate, turning them into minimal no-format blocks
    if template<> is found.
    """
    in_struct = False
    struct_lines = []
    output_lines = []

    for line in lines:
        if (('struct' in line and '{' in line) or ('template<' in line)) and not in_struct:
            if 'template<' in line:
                output_lines.append('// @formatter:off')
            in_struct = True
            output_lines.append(line.strip())
        elif in_struct and '};' in line:
            in_struct = False
            struct_content = ' '.join(struct_lines).strip()
            output_lines.append(f"{struct_content}")
            output_lines.append(line.strip())
            output_lines.append('// @formatter:on')
            struct_lines = []
        elif in_struct:
            struct_lines.append(line.strip())
        else:
            output_lines.append(line.strip())

    return '\n'.join(output_lines)


def main():
    parser = argparse.ArgumentParser(
        description="yoink: Remove redundant lines, inline C++ includes, process struct definitions, and copy code to clipboard.")
    parser.add_argument(
        "file", metavar="FILE", type=str,
        help="Path to the C++ file (absolute or relative to the workspace) to process.")
    parser.add_argument(
        "--workspace", type=str, default=os.path.expanduser("~/workspace/brendon/algo/library"),
        help="The workspace directory for relative paths. Default: %(default)s")

    args = parser.parse_args()

    # Determine absolute path to the target file
    if os.path.isabs(args.file):
        filepath = args.file
    else:
        filepath = os.path.abspath(os.path.join(args.workspace, args.file))

    if not os.path.exists(filepath):
        print(f"Error: The file does not exist: {filepath}")
        sys.exit(1)

    visited_includes = set()
    visited_includes.add(filepath)

    try:
        # Read and inline includes
        lines = read_and_inline(filepath, visited_includes, args.workspace)
        # Process struct formatting
        final_output = process_struct(lines)
    except Exception as e:
        print(f"Error processing file '{filepath}': {e}")
        sys.exit(1)

    # Copy to clipboard
    try:
        pyperclip.copy(final_output)
        print(
            "Done!")
    except Exception as e:
        print(f"Error copying to clipboard: {e}")
        sys.exit(1)


if __name__ == "__main__":
    main()
