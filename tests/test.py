"""
A test suite for Greyhawk.

tests to see if the AST and LLVM output are identical. if not, it'll
start diffing and asking if you the changes are acceptable.
"""
import argparse
import os
import shlex
import subprocess
import difflib

BASE = os.path.dirname(os.path.abspath(__file__))
# this is the parent directory
REPO_ROOT = os.path.dirname(BASE)

EXECUTABLE = os.path.join(REPO_ROOT, 'bin', 'greyhawk')
EXAMPLES_DIRECTORY = os.path.join(os.curdir, 'examples')
# specifically test-related
TEST_DIR = os.path.join(os.curdir, 'tests')
AST_OUTPUT_DIR = os.path.join(TEST_DIR, 'ast')
RESULT_OUTPUT_DIR = os.path.join(TEST_DIR, 'result')

# string templates
OUTPUT_DOESNT_EXIST = "Output for {type} {file} doesn't exist! do you want to create it? (default no) "
OUTPUT_CHANGED = "Output changed! accept changes? (default no) "

TEST_FILE_TYPES = {
    "ast": {
        "extra_params": "--ast",
        "name": "ast",
        "test_directory": AST_OUTPUT_DIR
    },
    "result": {
        "extra_params": "",
        "name": "result",
        "test_directory": RESULT_OUTPUT_DIR
    }
}


def _test_file(target_file, extra_params):
    command = shlex.split('{0} {1} {2}'.format(
        EXECUTABLE, target_file, extra_params
    ))
    child = subprocess.Popen(command, stdout=subprocess.PIPE)
    stdout, stderr = child.communicate()
    if child.returncode != 0:
        print("  executing {0} with {1} had exit code {2}".format(
            target_file, extra_params, child.returncode
        ))
        return None
    return stdout


def main():

    parser = argparse.ArgumentParser()
    parser.add_argument("-p", "--prompt", help="Instead of failing, prompt to see if content should be overwritten", action="store_true")
    args = parser.parse_args()

    tests = 0
    successes = 0
    for test_type, test_file_type in TEST_FILE_TYPES.items():
        print(test_type + ":")

        for name in os.listdir(EXAMPLES_DIRECTORY):
            if not name.endswith('.gh'):
                continue

            tests += 1
            target_file_path = os.path.join(EXAMPLES_DIRECTORY, name)

            output = _test_file(target_file_path, test_file_type['extra_params'])

            if not output:
                continue

            output = output.decode('utf-8')

            output_file = os.path.join(test_file_type['test_directory'], name)

            if not os.path.exists(output_file):
                print("  output file not found for {0}. creating...".format(
                    name
                ))
                with open(output_file, 'w+') as fh:
                    fh.write(output)

            else:
                with open(output_file, 'r') as fh:
                    desired_output = fh.read()
                    if output.strip() != desired_output.strip():
                        print("  output file {0} has modified output:".format(name))
                        for line in difflib.context_diff(
                                output.split('\n'), desired_output.split('\n'),
                                "Current Output", "Desired Output"
                        ):
                            print(line)
                        continue

            successes += 1

    print("{0}/{1} files passed!".format(successes, tests))


main()
