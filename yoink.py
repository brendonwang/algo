#!/usr/bin/env python3
"""
yoink: a CLI tool that takes code from a file, removes redundant lines
       (such as the "#pragma once" line) before and after the actual code,
       then pastes the processed code into the clipboard.

By default, it will look for files relative to ~/workspace/brendon/algo.
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

def process_file(filepath: str) -> str:
    with open(filepath, 'r') as file:
        lines = file.readlines()

    # Remove redundant lines, e.g., any line exactly matching "#pragma once"
    processed_lines = []
    for line in lines:
        if re.match(r'^\s*#pragma\s+once\s*$', line):
            continue
        processed_lines.append(line)

    # Strip leading and trailing blank lines
    while processed_lines and processed_lines[0].strip() == "":
        processed_lines.pop(0)
    while processed_lines and processed_lines[-1].strip() == "":
        processed_lines.pop()

    return ''.join(processed_lines)

def main():
    parser = argparse.ArgumentParser(
        description="yoink: Remove redundant lines and copy code to clipboard."
    )
    parser.add_argument("file", metavar="FILE", type=str,
                        help="The file path relative to the workspace directory or an absolute path.")
    parser.add_argument("--workspace", type=str, default=os.path.expanduser("~/workspace/brendon/algo"),
                        help="The workspace directory for relative file paths. Default: %(default)s")

    args = parser.parse_args()

    # Interpret the file path relative to the workspace if not absolute.
    if not os.path.isabs(args.file):
        filepath = os.path.join(args.workspace, args.file)
    else:
        filepath = args.file

    if not os.path.exists(filepath):
        print(f"Error: The file does not exist: {filepath}")
        sys.exit(1)

    try:
        processed_code = process_file(filepath)
    except Exception as e:
        print(f"Error processing file: {e}")
        sys.exit(1)

    # Copy the processed code to the clipboard
    try:
        pyperclip.copy(processed_code)
        print("Code has been processed and copied to the clipboard!")
    except Exception as e:
        print(f"Error copying to clipboard: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()