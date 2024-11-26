# Template for C projects

## Get started
- Clone the repo
- Run `make compile_commands` to generate compile_commands.json (See [LSP](#lsp))
- Run `make` to compile the project

## Commands
```bash
make # compiles the project
make run # runs the executable
make clean # cleans the project
make compile_commands # updates the compile_commands.json file
```

## LSP
If you use clangd lsp, you must install the [bear cli](https://github.com/rizsotto/Bear). You'll need to
run `make compile_commands` when your build process has changed.

The `compile_commands.json` helps the LSP find some files such as the headers.

