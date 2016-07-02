"""
A test suite for Greyhawk.

tests to see if the AST and LLVM output are identical. if not, it'll
start diffing and asking if you the changes are acceptable.
"""
import os
import shlex
import subprocess
import sys

BASE = os.path.dirname(os.path.abspath(__file__))
# this is the parent directory
REPO_ROOT = os.path.dirname(BASE)
EXECUTABLE = os.path.join(REPO_ROOT, 'target', 'debug', 'greyhawk')
SOURCE_DIRECTORY = os.path.join(REPO_ROOT, "examples")
TEST_TYPES = {
    # "ast": {
    #     "extra_params": "--ast",
    #     "dir": os.path.join(REPO_ROOT, "tests", "ast")
    # },
    "stdout": {
        "extra_params": "",
        "dir": os.path.join(REPO_ROOT, "tests", "stdout")
    }
}
FAILED, SKIPPED, PASSED = "failed", "skipped", "passed"


def main(args=sys.argv[1:]):
    # options = _parse_arguments(args)
    total, passed, failed, skipped = 0, 0, 0, 0
    for name in os.listdir(SOURCE_DIRECTORY):
        if not name.endswith(".gh"):
            continue
        if name.startswith("_"):
            continue
        total += 1
        result = _test_file(name)
        if result == FAILED:
            failed += 1
        elif result == SKIPPED:
            skipped += 1
        else:
            passed += 1
    if failed:
        print(red(
            "failed : {0:02d} / {0:02d}".format(failed, total)
        ))
    if skipped:
        print(yellow(
            "skipped: {0:02d} / {0:02d}".format(skipped, total)
        ))
    if passed:
        print(green(
            "passed : {0:02d} / {0:02d}".format(passed, total)
        ))


def _parse_arguments(args):
    import argparse
    parser = argparse.ArgumentParser()
    return parser.parse_args()


def _test_file(name):
    print("testing {}...".format(name))
    skipped = True
    failed = False
    for type_name, configuration in TEST_TYPES.items():
        target_file = os.path.join(configuration["dir"], name)
        if not os.path.exists(target_file):
            continue
        skipped = False
        succeeded = _run_test(
            os.path.join(SOURCE_DIRECTORY, name),
            target_file,
            configuration["extra_params"]
        )
        print("  {}: {}".format(type_name, succeeded))
        failed = failed or not succeeded
    if failed:
        return FAILED
    if skipped:
        return SKIPPED
    return PASSED


def _run_test(source_file, target_file, extra_params):
    command = shlex.split("{} {} {}".format(
        EXECUTABLE, source_file, extra_params
    ))
    child = subprocess.Popen(command, stdout=subprocess.PIPE)
    stdout, stderr = child.communicate()
    if child.returncode != 0:
        return False
    with open(target_file) as fh:
        output = fh.read().strip()
        stdout = stdout.decode("UTF-8").strip()
        return stdout == output

CSI = "\x1B["


def red(body):
    return CSI + "31;31m" + body + CSI + "0m"

def yellow(body):
    return CSI + "31;33m" + body + CSI + "0m"

def green(body):
    return CSI + "31;32m" + body + CSI + "0m"

main()
