# abnet (ASIO-Based Networking Library)

`abnet` is an [ASIO](https://github.com/chriskohlhoff/asio/tree/asio-1-32-0)-based networking library built with **NO** boost dependency. <br>
Its primary goal is to provide a minimal, cross-platform BSD socket-like API.

## Installation
To start using this lib, you only need to include `abnet.hpp` header to your project.

## IMPORTANT!
Although `abnet` is simply a modified version of internal `ASIO` sockets, you cannot (and should not) use them interchangeably or attempt to combine both in your project.

## Issues and Improvements
If you encounter any issues while using abnet or have suggestions for improvements, feel free to open an issue on this GitHub repository. When reporting an issue, please include:

* A clear description of the problem.
* Steps to reproduce the issue, if applicable.
* Relevant code snippets or logs.
* Your platform and compiler details.
#### For improvement suggestions, describe the feature or change you'd like to see, including its potential use case and benefits.