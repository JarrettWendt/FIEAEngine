# FIEA Engine

This is a custom game engine that I made from scratch during my graduate course at the Florida Interactive Entertainment Academy. This is naturally a derivation of an academic library crated by my professor [Dr. Paul Varcholik](https://www.varcholik.org/). I have taken lengths to improve on the original design with modern C++20 features.

## Dependencies
### Universal
I recommend using `vcpkg` to install the following dependencies:
- `jsoncpp`
- `ms-gsl`
- `catch2`

Python 3.8 or newer. This project may very well work with earlier versions of Python, perhaps even Python 2, but they are untested. Make sure you install the developer tools as well. On Windows, this can be done with the Python interactive installer under "advanced options". On Linux, this can be done by installing `python3-dev` through your preferred package manager.

With the python dev tools installed, create two environment variables to them: `PYTHON_INCLUDE_DIR` to the directory container Python's C header files and `PYTHON_LIBRARY_DIR` to where the object files are. You can discover where they are on your system with this script:
```python
from sysconfig import get_paths
from pprint import pprint
pprint(get_paths())
```

The Matrix demo uses [asciimatics](https://github.com/peterbrittain/asciimatics).

### Windows
Install the latest Clang via the Visual Studio installer

### Linux
Clang 10 or newer.
```bash
wget https://apt.llvm.org/llvm.sh
chmod +x llvm.sh
sudo ./llvm.sh 10
```

Verify you have Clang 10 installed with
```bash
clang++-10 --version
```
