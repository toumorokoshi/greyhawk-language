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


def main(args=sys.argv[1:]):
    # options = _parse_arguments(args)
    total, success = 0, 0
    for name in os.listdir(SOURCE_DIRECTORY):
        if not name.endswith(".gh"):
            continue
        if name.startswith("_"):
            continue
        total += 1
        if _test_file(name):
            success += 1
    print("success: {} / {}".format(success, total))


def _parse_arguments(args):
    import argparse
    parser = argparse.ArgumentParser()
    return parser.parse_args()


def _test_file(name):
    print("testing {}...".format(name))
    failed = False
    for type_name, configuration in TEST_TYPES.items():
        target_file = os.path.join(configuration["dir"], name)
        if not os.path.exists(target_file):
            continue
        succeeded = _run_test(
            os.path.join(SOURCE_DIRECTORY, name),
            target_file,
            configuration["extra_params"]
        )
        print("  {}: {}".format(type_name, succeeded))
        failed = failed or not succeeded
    return not failed


def _run_test(source_file, target_file, extra_params):
    command = shlex.split("{} {} {}".format(
        EXECUTABLE, source_file, extra_params
    ))
    child = subprocess.Popen(command, stdout=subprocess.PIPE)
    stdout, stderr = child.communicate()
    if child.returncode == 0:
        return False
    with open(target_file) as fh:
        return stdout == fh.read()

main()
