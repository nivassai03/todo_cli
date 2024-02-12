# TODO CLI
A simple terminal based todo-list application.

## Features
- Easy to backup and transfer data as everything is stored in a single json file.
- Low memory usage.

## Screenshots
![todo_cli1](https://github.com/nivassai03/todo_cli/assets/116424621/c85a2cf3-87e9-4884-81b9-240c5484e30e)


![todo_cli2](https://github.com/nivassai03/todo_cli/assets/116424621/ed526d4e-9974-4a26-aa97-2d7dcbe7ae5c)


## Installation

1. Clone the repository.
```sh
git clone https://github.com/nivassai03/todo_cli.git
```

2. Build the application.
```sh
mkdir build && cd build
cmake ..
make
```
3. Run tha application.
```sh
./todo
```

## Credits
todo_cli is written in c++20 and uses the following open-source packages.
* [ftxui](https://github.com/ArthurSonzogni/FTXUI)
* [nlohmann-json](https://github.com/nlohmann/json)
