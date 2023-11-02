import os
import re

# Define the directory where you want to perform the replacements
directory = "."  # Use "." for the current directory

# Regular expression pattern to match lines with #include "folder/file.h"
pattern = re.compile(r'#include\s+"\w+/(\w+\.h)"')

# Function to process and modify a single file
def process_file(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()

    modified_lines = []
    for line in lines:
        modified_line = pattern.sub(r'#include "\1"', line)
        modified_lines.append(modified_line)

    with open(file_path, 'w') as file:
        file.writelines(modified_lines)

# check that directory is correct
dirname = os.path.basename(os.getcwd())
if dirname != "loader-binary":
    print("wrong cwd! exiting.")
    exit()

# Traverse the directory and process .c and .h files
for root, dirs, files in os.walk(directory):
    for file in files:
        if file.endswith(('.c', '.h')):
            file_path = os.path.join(root, file)
            process_file(file_path)
            print(f"Processed: {file_path}")

print("Replacement complete.")
