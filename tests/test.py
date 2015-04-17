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


def get_yes_or_no(prompt):
    """ convert user input to a yes or no """
    response = input(prompt)
    cleaned_response = response.strip().lower()
    return len(cleaned_response) > 0 and cleaned_response[0] in ('y', 't')

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

parser = argparse.ArgumentParser()
parser.add_argument("-p", "--prompt", help="Instead of failing, prompt to see if content should be overwritten", action="store_true")
args = parser.parse_args()

errors = []
tests = 0
for f in filter(lambda x: x.endswith('gh'), os.listdir(EXAMPLES_DIRECTORY)):
    tests += 1
    target_file_path = os.path.join(EXAMPLES_DIRECTORY, f)
    # check if ast matches
    for test_type, test_file_type in TEST_FILE_TYPES.items():
        command = shlex.split('{0} {1} {2}'.format(
            EXECUTABLE, target_file_path, test_file_type['extra_params'])
        )
        output = subprocess.call(command, stderr=subprocess.STDOUT).decode('utf-8')
        output_file = os.path.join(test_file_type['test_directory'], f)
        overwrite = False
        if not os.path.exists(output_file):
            if args.prompt:
                overwrite = get_yes_or_no(OUTPUT_DOESNT_EXIST.format(type=test_type,
                                                                     file=output_file))
        else:
            with open(output_file, 'r') as fh:
                desired_output = fh.read()
                if output.strip() != desired_output.strip():
                    for line in difflib.context_diff(output.split('\n'), desired_output.split('\n'),
                                                     "Current Output", "Desired Output"):
                        print(line)
                    overwrite = get_yes_or_no(OUTPUT_CHANGED) if args.prompt else False
                else:
                    continue
        if overwrite:
            if not os.path.exists(os.path.dirname(output_file)):
                os.makedirs(os.path.dirname(output_file))
            with open(output_file, 'w+') as fh:
                fh.write(output)
        else:
            errors.append((f, test_type, output))

if errors:
    print("Errors were encountered!")
    for f, test_type, output in errors:
        print("{0} with {1}:".format(f, test_type))
        print("#" * 40)
        print(output)
    print("{0} tests failed!".format(len(errors)))
    exit(1)
else:
    print("{0} files passed!".format(tests))
