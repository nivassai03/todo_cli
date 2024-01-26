# TODO CLI
A simple terminal based todo-list application.

## Features
- Easy to backup and transfer data as everything is stored in a single json file.
- Low memory usage.

## Screenshots
![todo_cli1](https://github.com/nivassai03/todo_cli/assets/116424621/8fc66097-0ecf-48ce-9d4b-62dc20dd6708)
![todo_cli2](https://github.com/nivassai03/todo_cli/assets/116424621/2ab62e15-f77d-47e4-9f7d-f12db96fdec7)

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
