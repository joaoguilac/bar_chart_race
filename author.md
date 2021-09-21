# Introduction

In the data folder you can get some input files to run the bar chart race.

# Author(s)

- Aluno: João Guilherme Lopes Alves da Costa (joguicosta@hotmail.com)
- Turma: DIM0120 - LINGUAGEM DE PROGRAMAÇÃO I - T01 (2021.1)
- Professor: Selan Rodrigues dos Santos

# Problems found or limitations

Quando o valor da barra máxima é maior que 0 e menor que 10 o eixo do bar chart acaba ficando muito grande. De certa forma é um problema visual do programa. Isso pode ser visualizado nos primeiros bar charts do arquivo `endgame.txt`.

# Grading

Preencha os itens que você implementou, indicando qual a nota esperada para cada item.


Item     | Valor máximo   | Valor esperado
-------- | :-----: | :-----:
Read, validate, and process command line arguments | 5 | 5
Program runs with default values, if no options are provided | 5 | 5
Read the input data file and store the information in memory    | 10 | 10
Display a summary of the data read from the input file _before_ the animation starts | 5 | 5
Display a summary of the running options (set by the user or default values) _before_ the animation starts | 5 | 5
The project contains at least two classes | 10 | 10
Create individual bar charts containing all data items for a single date | 10 | 10
Run the bar chart animation at the correct speed | 10  | 10
The bar chart anatomy is complete and correct | 30 | 30
Program handles errors properly | 10 | 10

# Compiling and Runnig

## Manualmente com g++

Para compilar e executar o programa manualmente basta digitar os seguintes comandos em seu terminal:

```
$ mkdir bin (caso não tenha uma pasta para os executáveis, você deve criá-la com esse comando)
$ cd bin/ (vá para a pasta dos executáveis criada para compilar e executar seu programa)
$ g++ -Wall -std=c++11 -g source/src/bcr.cpp source/src/animation_mgr.cpp source/src/bar_chart.cpp -I source/include -o bin/bcr (compilar)
$ ./bin/bcr [<options>] <input_data_file> (executar)
    Bar Chart Race options:
      -h  Print this help text.
      -b  <num> Max # of bars in a single char.
                Valid range is [1,15]. Default values is 5.
      -f  <num> Animation speed in fps (frames per second).
                Valid range is [1,24]. Default value is 24.
```

## Cmake

Para compilar e executar o programa com o Cmake você precisa ter instalado pelo menos a versão 3.2. Em seguida, abra seu terminal e execute os seguintes comandos:

```
$ cmake -S source -B build (pede ao cmake para criar o diretório de compilação)
$ cmake --build build (compilar)
$ ./build/bcr [<options>] <input_data_file> (executar)
    Bar Chart Race options:
      -h  Print this help text.
      -b  <num> Max # of bars in a single char.
                Valid range is [1,15]. Default values is 5.
      -f  <num> Animation speed in fps (frames per second).
                Valid range is [1,24]. Default value is 24.
```
