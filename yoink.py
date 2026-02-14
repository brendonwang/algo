#!/usr/bin/env python3
"""
yoink: a CLI tool that takes one or more C++ source/header files, removes
       redundant lines (such as "#pragma once"), optionally inlines #include
       files (ensuring each file is only included once), and copies the
       processed code into the clipboard.

By default, it will look for files relative to
~/workspace/brendon/algo/library, but you can specify a custom workspace path
with the '--workspace' argument.

Examples:
    yoink graph/dijkstra.cpp
    yoink graph/dijkstra.cpp graph/utils.hpp
"""

import argparse
import os
import sys
import re

try:
    import pyperclip
except ImportError:
    print("Error: This script requires the 'pyperclip' package. "
          "Install it by running 'pip install pyperclip'.")
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

            # First, assume it's relative to the current file
            abs_inc_path = os.path.join(os.path.dirname(filepath), include_path)
            if not os.path.isabs(abs_inc_path):
                # If still not absolute, resolve relative to the workspace
                abs_inc_path = os.path.abspath(os.path.join(workspace, include_path))

            # Inline the included file if not visited
            if abs_inc_path not in visited_includes:
                visited_includes.add(abs_inc_path)
                if os.path.exists(abs_inc_path):
                    included_lines = read_and_inline(abs_inc_path, visited_includes, workspace)
                    processed_lines.extend(included_lines)
                else:
                    # If the file doesn't exist, keep the #include line as-is
                    processed_lines.append(line.rstrip('\n'))
            # If we've already seen this include, skip it
        else:
            processed_lines.append(line.rstrip('\n'))

    return processed_lines


def main():
    parser = argparse.ArgumentParser(
        description=(
            "yoink: Remove redundant lines, inline C++ includes, "
            "and copy processed code from one or more files to the clipboard."
        )
    )
    parser.add_argument(
        "files", metavar="FILE", type=str, nargs="+",
        help="Path(s) to C++ file(s) (absolute or relative to the workspace) to process."
    )
    parser.add_argument(
        "--workspace", type=str,
        default=os.path.expanduser("~/workspace/brendon/algo/library"),
        help="The workspace directory for relative paths. Default: %(default)s"
    )

    args = parser.parse_args()

    # Resolve all file paths to absolute paths
    resolved_files = []
    for file_arg in args.files:
        if os.path.isabs(file_arg):
            filepath = file_arg
        else:
            filepath = os.path.abspath(os.path.join(args.workspace, file_arg))

        if not os.path.exists(filepath):
            print(f"Error: The file does not exist: {filepath}")
            sys.exit(1)

        resolved_files.append(filepath)

    visited_includes: set = set()
    all_lines: list[str] = []

    # Process each file in order, sharing visited_includes
    for i, filepath in enumerate(resolved_files):
        visited_includes.add(filepath)
        try:
            lines = read_and_inline(filepath, visited_includes, args.workspace)
        except Exception as e:
            print(f"Error processing file '{filepath}': {e}")
            sys.exit(1)

        all_lines.extend(lines)

        # Add a blank line between different top-level files for readability
        if i != len(resolved_files) - 1:
            all_lines.append("")

    if not all_lines:
        print("No content processed; nothing copied to clipboard.")
        sys.exit(0)

    # Copy to clipboard
    try:
        pyperclip.copy('\n'.join(all_lines))
    except Exception as e:
        print(f"Error copying to clipboard: {e}")
        sys.exit(1)

    # Print out which files were copied
    print("Done! Copied processed code from the following file(s):")
    for path in resolved_files:
        print(f"  - {path}")


if __name__ == "__main__":
    main()
