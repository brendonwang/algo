#!/usr/bin/env python3

from __future__ import annotations

import os
import shutil
import subprocess
import sys
import time
from pathlib import Path
from typing import Iterable, List, Optional

DEFAULT_LIB_ROOT = Path.home() / "workspace" / "brendon" / "algo" / "library"

TOP_LEVEL_PREVIEW = r"""
P="$LIB_ROOT"/{}
case "$P" in
  "$LIB_ROOT/.. EXIT")
    echo "Exit"
    ;;
  "$LIB_ROOT/.. YOINK STAGED ("*)
    echo "Staged files:"
    if [ -n "$YOINK_STAGED_DISPLAY" ]; then
      printf '%s\n' "$YOINK_STAGED_DISPLAY"
    else
      echo "(none)"
    fi
    ;;
  *)
    if [ -d "$P" ]; then
      (cd "$P" && (fd . -t f 2>/dev/null || find . -type f | sed "s|^\./||")) | head -200
    else
      bat --style=numbers --color=always --line-range=:200 "$P" 2>/dev/null || file "$P"
    fi
    ;;
esac
""".strip()

INNER_PREVIEW = r"""
P="$CURRENT_DIR"/{}
if [ "{}" = ".. BACK" ]; then
  ls -la "$CURRENT_DIR"
else
  bat --style=numbers --color=always --line-range=:200 "$P" 2>/dev/null || file "$P"
fi
""".strip()


def ensure_command(name: str) -> None:
    if shutil.which(name):
        return
    print(f"Error: Required command '{name}' is not available in PATH.")
    sys.exit(1)


def list_top_level(lib_root: Path) -> List[str]:
    try:
        entries = [entry.name for entry in lib_root.iterdir()]
    except OSError as exc:
        print(f"Error reading library directory '{lib_root}': {exc}")
        sys.exit(1)
    return sorted(entries, key=str.lower)


def list_files(directory: Path) -> List[str]:
    files: List[str] = []
    for root, _, filenames in os.walk(directory):
        for filename in filenames:
            rel = Path(root, filename).relative_to(directory)
            files.append(rel.as_posix())
    return sorted(files, key=str.lower)


def run_fzf(
        lines: Iterable[str],
        prompt: str,
        preview_cmd: str,
        multi: bool = False,
        extra_env: Optional[dict[str, str]] = None,
) -> Optional[List[str]]:
    env = os.environ.copy()
    if extra_env:
        env.update(extra_env)

    args = [
        "fzf",
        "--layout=reverse",
        "--border",
        f"--prompt={prompt}",
        "--preview",
        preview_cmd,
        "--preview-window",
        "right:60%",
    ]
    if multi:
        args.append("--multi")

    input_text = "\n".join(lines)
    try:
        proc = subprocess.run(
            args,
            input=input_text,
            text=True,
            capture_output=True,
            check=False,
            env=env,
        )
    except FileNotFoundError:
        print("Error: fzf command not found in PATH.")
        sys.exit(1)

    if proc.returncode != 0:
        return None

    output = proc.stdout.rstrip("\n")
    if not output:
        return []
    return output.split("\n")


def run_yoink(script_path: Path, files: List[str], lib_root: Path) -> None:
    args: List[str] = []
    for entry in files:
        if os.path.isabs(entry):
            args.append(entry)
        else:
            args.append(str((lib_root / entry).resolve()))

    cmd = [sys.executable, str(script_path), *args]
    try:
        subprocess.run(cmd, check=True)
    except subprocess.CalledProcessError as exc:
        print(f"yoink.py failed with exit code {exc.returncode}.")
        sys.exit(exc.returncode)


def stage_files(
        staged: List[str],
        selected_files: List[str],
        selected_dir: str,
) -> None:
    dir_rel = selected_dir.rstrip("/")
    for rel in selected_files:
        rel_clean = rel.lstrip("./")
        entry = rel_clean if not dir_rel else f"{dir_rel}/{rel_clean}"
        staged.append(entry)


def main() -> None:
    ensure_command("fzf")

    lib_root = Path(os.environ.get("YOINK_PROMPT_LIB_ROOT", DEFAULT_LIB_ROOT)).expanduser()
    if not lib_root.is_dir():
        print(f"Error: LIB_ROOT '{lib_root}' does not exist or is not a directory.")
        sys.exit(1)

    yoink_path = Path(os.environ.get("YOINK_PROMPT_YOINK", Path(__file__).resolve().parent / "yoink.py"))
    if not yoink_path.exists():
        print(f"Error: Could not locate yoink.py at '{yoink_path}'.")
        sys.exit(1)

    staged_files: List[str] = []

    while True:
        top_entries = [".. YOINK STAGED ({})".format(len(staged_files)), ".. EXIT"]
        top_entries.extend(list_top_level(lib_root))

        preview_env = {
            "LIB_ROOT": str(lib_root),
            "YOINK_STAGED_DISPLAY": "\n".join(staged_files),
        }
        selection = run_fzf(
            top_entries,
            prompt="folder/file> ",
            preview_cmd=TOP_LEVEL_PREVIEW,
            multi=False,
            extra_env=preview_env,
        )

        if not selection:
            sys.exit(0)

        choice = selection[0]

        if choice == ".. EXIT":
            sys.exit(0)

        if choice.startswith(".. YOINK STAGED"):
            if not staged_files:
                print("No staged files.")
                time.sleep(1)
                continue
            run_yoink(yoink_path, staged_files, lib_root)
            time.sleep(2)
            sys.exit(0)

        target_path = lib_root / choice

        if target_path.is_file():
            run_yoink(yoink_path, [str(target_path)], lib_root)
            time.sleep(2)
            sys.exit(0)

        if not target_path.is_dir():
            continue

        selected_dir = choice

        while True:
            inner_entries = [".. BACK"]
            inner_entries.extend(list_files(target_path))

            inner_env = {
                "CURRENT_DIR": str(target_path),
            }

            inner_selection = run_fzf(
                inner_entries,
                prompt=f"file in {selected_dir}> ",
                preview_cmd=INNER_PREVIEW,
                multi=True,
                extra_env=inner_env,
            )

            if inner_selection is None:
                break

            cleaned = [item for item in inner_selection if item != ".. BACK"]
            if not cleaned:
                break

            stage_files(staged_files, cleaned, selected_dir)
            break


if __name__ == "__main__":
    main()
