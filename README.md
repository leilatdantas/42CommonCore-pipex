# Pipex 🔄

## Table of Contents
- [Description](#description)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Examples](#examples)
- [Contributing](#contributing)
- [License](#license)

## Description
The Pipex project is focused on handling pipes in a Unix-like shell environment. The program is designed to execute a sequence of shell commands, similar to the pipeline operator (|) in a shell, by taking input from one file, processing it through two shell commands, and then writing the result to another file. The goal is to implement a program that simulates the behavior of the shell command line, redirecting the output of one command as input to another.

## Installation
1. Clone the repository and build the executable:
```bash
git clone https://github.com/your-username/pipex.git
cd pipex
make
```
## Usage
The pipex program takes four arguments:
  ```bash
    ./pipex file1 cmd1 cmd2 file2
  ```
**file1:** Input file whose content will be processed by cmd1.

**cmd1:** First command to be executed.

**cmd2:** Second command to be executed.

**file2:** Output file where the result will be saved.


## Example

```bash
  ./pipex infile "ls -l" "wc -l" outfile
```
This example executes the commands ls -l and wc -l in sequence, where the output of the first command is used as the input for the second command.
