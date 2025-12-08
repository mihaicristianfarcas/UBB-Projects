#!/usr/bin/env python3
"""Entry point for LR(0) shift-reduce parser."""

import sys
from pathlib import Path

# Add src directory to path
sys.path.insert(0, str(Path(__file__).parent))

from lr0.cli import main

if __name__ == "__main__":
    main()
