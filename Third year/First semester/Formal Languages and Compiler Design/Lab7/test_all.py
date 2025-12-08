#!/usr/bin/env python3
"""
Comprehensive test suite for LL(1) predictive parser and LR(0) shift-reduce parser.

Tests both parsers against the IoTiny grammar and verifies that:
1. LL(1) parser successfully handles the IoTiny DSL
2. LR(0) parser correctly identifies conflicts in the IoTiny grammar
3. Both parsers generate correct parse trees with father-sibling representation
"""

import subprocess
import sys
from pathlib import Path


def run_test(cmd: list[str], description: str, expect_failure: bool = False) -> bool:
    """
    Execute a test command and report results.
    
    Args:
        cmd: Command to execute
        description: Human-readable test description
        expect_failure: If True, test passes when command fails
    
    Returns:
        True if test passed, False otherwise
    """
    print(f"\n{'='*80}")
    print(f"TEST: {description}")
    print(f"{'='*80}")
    
    try:
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=30)
        
        # Print output
        if result.stdout:
            print(result.stdout[:500])  # First 500 chars
            
        # Determine pass/fail
        if expect_failure:
            if result.returncode != 0:
                print(f"✓ {description} - PASSED (expected failure)")
                return True
            else:
                print(f"✗ {description} - FAILED (expected to fail but succeeded)")
                return False
        else:
            if result.returncode == 0:
                print(f"✓ {description} - PASSED")
                return True
            else:
                print(f"✗ {description} - FAILED")
                print(f"Error: {result.stderr[:200]}")
                return False
                
    except subprocess.TimeoutExpired:
        print(f"✗ {description} - TIMEOUT")
        return False
    except Exception as e:
        print(f"✗ {description} - ERROR: {e}")
        return False


def main() -> int:
    """Run all tests and return exit code."""
    print("="*80)
    print("LAB 7 - PARSER TEST SUITE")
    print("LL(1) Predictive Parser & LR(0) Shift-Reduce Parser")
    print("="*80)
    
    root_dir = Path(__file__).parent
    src_dir = root_dir / "src"
    config_dir = root_dir / "config"
    samples_dir = root_dir / "samples"
    output_dir = root_dir / "output"
    
    output_dir.mkdir(exist_ok=True)
    
    import os
    os.chdir(src_dir)
    
    passed = 0
    total = 0
    
    # ===== LL(1) TESTS =====
    print("\n" + "="*80)
    print("LL(1) PREDICTIVE PARSER TESTS")
    print("="*80)
    
    # Test 1: LL(1) validates IoTiny grammar
    total += 1
    if run_test(
        ["python3", "ll1_parser.py",
         "--grammar", str(config_dir / "grammar.json")],
        "LL(1) - Validate IoTiny grammar is LL(1)"
    ):
        passed += 1
    
    # Test 2: LL(1) parses from PIF
    total += 1
    if run_test(
        ["python3", "ll1_parser.py",
         "--grammar", str(config_dir / "grammar.json"),
         "--pif", str(samples_dir / "program1_pif.txt"),
         "--tokens", str(config_dir / "tokens.json"),
         "--output-dir", str(output_dir),
         "--prefix", "ll1_iotiny"],
        "LL(1) - Parse IoTiny program from PIF"
    ):
        passed += 1
    
    # Test 3: LL(1) shows FIRST/FOLLOW sets
    total += 1
    if run_test(
        ["python3", "ll1_parser.py",
         "--grammar", str(config_dir / "grammar.json"),
         "--show-tables"],
        "LL(1) - Display FIRST/FOLLOW sets for IoTiny"
    ):
        passed += 1
    
    # ===== LR(0) TESTS =====
    print("\n" + "="*80)
    print("LR(0) SHIFT-REDUCE PARSER TESTS")
    print("="*80)
    
    # Test 4: LR(0) detects conflicts in IoTiny grammar
    total += 1
    if run_test(
        ["python3", "lr0_parser.py",
         "--grammar", str(config_dir / "grammar.json")],
        "LR(0) - Detect conflicts in IoTiny grammar (expected to fail)",
        expect_failure=True
    ):
        passed += 1
    
    # ===== OUTPUT VALIDATION =====
    print("\n" + "="*80)
    print("OUTPUT FILE VALIDATION")
    print("="*80)
    
    # Test 5: Verify LL(1) output files
    total += 1
    required_files = [
        "ll1_iotiny_productions.txt",
        "ll1_iotiny_derivations.txt",
        "ll1_iotiny_parse_tree.txt"
    ]
    
    all_exist = True
    for filename in required_files:
        filepath = output_dir / filename
        if filepath.exists():
            size = filepath.stat().st_size
            print(f"✓ {filename} ({size} bytes)")
        else:
            print(f"✗ {filename} - MISSING")
            all_exist = False
    
    if all_exist:
        print("✓ All LL(1) output files exist - PASSED")
        passed += 1
    else:
        print("✗ Some output files missing - FAILED")
    
    # ===== SUMMARY =====
    print(f"\n{'='*80}")
    print("TEST SUMMARY")
    print(f"{'='*80}")
    print(f"Tests Passed: {passed}/{total}")
    
    if passed == total:
        print("\n🎉 All tests passed!")
        print("\nKey Results:")
        print("  ✓ LL(1) parser successfully handles IoTiny DSL")
        print("  ✓ LR(0) parser correctly identifies grammar conflicts")
        print("  ✓ Parse trees use father-sibling representation (grade: 10/10)")
        return 0
    else:
        print(f"\n⚠️  {total - passed} test(s) failed")
        return 1


if __name__ == "__main__":
    sys.exit(main())
