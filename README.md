<p align="center"><img src="https://github.com/JustWhit3/temp-cleaner/blob/main/img/logo.png" height=400></p>

<p align="center">
  <img title="v0.0.1" alt="v0.0.1" src="https://img.shields.io/badge/version-v0.0.1-informational?style=flat-square">
  <img title="MIT License" alt="license" src="https://img.shields.io/badge/license-MIT-informational?style=flat-square">
	<img title="C++20" alt="C++20" src="https://img.shields.io/badge/c++-20-informational?style=flat-square"><br/>
	<img title="Code size" alt="code size" src="https://img.shields.io/github/languages/code-size/JustWhit3/temp-cleaner?color=red">
	<img title="Repo size" alt="repo size" src="https://img.shields.io/github/repo-size/JustWhit3/temp-cleaner?color=red">
	<img title="Lines of code" alt="total lines" src="https://img.shields.io/tokei/lines/github/JustWhit3/temp-cleaner?color=red">
</p>

***

## Table of contents

- [Introduction](#introduction)
- [Architectures support](#architectures-support)
  - [Operating systems](#operating-systems)
  - [Compilers](#compilers)
- [Install and use](#install-and-use)
  - [Install](#install)
  - [Debug mode and tests](#debug-mode-and-tests)
  - [How to use](#how-to-use)
- [Todo](#todo)
- [Credits](#credits)
  - [Project leaders](#project-leaders)
- [Stargazers over time](#stargazers-over-time)

## Introduction

`temp-cleaner` is a an application which allows you to automatically clean up temporary files and ignored items from git repositories in your system by analyzing .gitignore files.

<img src="https://github.com/JustWhit3/temp-cleaner/blob/main/img/rec.gif">

For an overview of the functions and classes please look at the [Doxygen](https://justwhit3.github.io/temp-cleaner/) page.

## Architecture support

### Operating systems

- **Linux**
  - *Ubuntu* (tested)
  - *WSL* (tested)
- **Windows**
  - *Windows 10* or higher (not tested)
- **MacOS** (not tested)

### Compilers

- **gcc**: v. 9/10/11/12/13
- **clang**: v. 11/12/13/14/15/16/17
- **MSVC**: (not directly tested, but should work)

## Install and use

### Install

**1)** Download one of the [releases](https://github.com/JustWhit3/temp-cleaner/releases) of the repository.

**2)** Unzip and enter the downloaded repository directory.

**3)** Install and compile the app and its dependencies:

```bash
cmake -B build
sudo cmake --build build --target install
```

> :warning: `sudo` is not required on Windows.

Mandatory prerequisites:

- C++20 standard
- g++ compiler (library developed on Ubuntu v22.04.1 with g++ v11.4.0)
- [CMake](https://cmake.org/) (at least v3.15)

### Debug mode and tests

Debug mode is used from developers to test the app. To build the app in debug mode:

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
sudo cmake --build build
```

This will automatically compile the source code with debug tools.

Prerequisites for debug mode:

- [cppcheck](https://cppcheck.sourceforge.io/)

> To install on ubuntu:
> `sudo apt install cppcheck`

To run [IWYU](https://github.com/include-what-you-use/include-what-you-use) checks:

```bash
./test/IWYU.sh
```

### How to use

Once the application is installed you can easily run it from your terminal in this way:

```bash
temp-cleaner [path/to/analyze] [config_file]
```

Where `[path/to/analyze]` represents the path through which the app will search for `.gitignore` files through repositories, while `[config_file]` can be a configuration file containing a list of paths to ignore. Both these arguments are optional and if left empty the search will proceed through the user home directory.

## Credits

<table>
  <tr>
    <td align="center"><a href="https://justwhit3.github.io/"><img src="https://avatars.githubusercontent.com/u/48323961?v=4" width="100px;" alt=""/><br /><sub><b>Gianluca Bianco</b></sub></a></td>
  </tr>
</table>

<!-- ALL-CONTRIBUTORS-LIST:START - Do not remove or modify this section -->
<!-- prettier-ignore-start -->
<!-- markdownlint-disable -->

<!-- markdownlint-restore -->
<!-- prettier-ignore-end -->

<!-- ALL-CONTRIBUTORS-LIST:END -->

## Stargazers over time

[![Stargazers over time](https://starchart.cc/JustWhit3/temp-cleaner.svg)](https://starchart.cc/JustWhit3/temp-cleaner)
