# Y86-64 Simulator CLI

A Y86-64 processor simulator written in C++11.

## Getting Started

To set up the environment, you need to have the following dependencies installed.

### 0. Prerequisites

- [CMake](https://cmake.org/download) 2.8 or above
- [GNU make](https://www.gnu.org/software/make) 4.0 or above
- [GCC](https://gcc.gnu.org/releases.html) 4.8 or above

For Windows, try [MinGW-w64](https://sourceforge.net/projects/mingw-w64).

### 1. Installation

#### 1.1 Windows

```bash
./install.ps1
```

#### 1.2 Linux

```bash
./install.sh
```

You may need to grant yourself the execute permission using the command `chmod`.

```bash
chmod +x install.sh
```

### 2. Usage

```bash
./sim input_file [output_dir]
```

#### 2.1 Arguments

##### 2.1.1 input_file

`input_file` is the path to your input file. Currently only .yo files are accepted.

##### 2.1.2 output_dir

*Optional.* `output_dir` is the directory for all outputs, which is set to the root folder of executable `sim` by default. Please make sure the directory already exists.

#### 2.2 Examples

```bash
./sim "../test/objects/prog1.yo"
```

```bash
./sim "../test/objects/asum.yo" "./temp"
```

#### 2.3 Manual

Check the manual simply by passing no argument.

```bash
./sim
```

### 3. Clean

#### 3.1 Windows

```bash
./clean.ps1
```

#### 3.2 Linux

```bash
./clean.sh
```

## Contributors

- [**Hakula Chen**](https://github.com/hakula139)<[i@hakula.xyz](mailto:i@hakula.xyz)> - Fudan University
- [**Jiaqi Dai**](https://github.com/jqdai) - Fudan University

## License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](https://github.com/hakula139/Y86-64-Simulator/blob/master/LICENSE) file for details.
