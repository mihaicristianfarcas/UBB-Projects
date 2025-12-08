#!/usr/bin/env python3
"""Entry point for LL(1) predictive parser."""

import sys
from pathlib import Path

# Add src directory to path
sys.path.insert(0, str(Path(__file__).parent))

from ll1.cli import main

if __name__ == "__main__":
    main()
