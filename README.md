# Multi-threaded Element Queue component

Everything works as per requested exercise document.
Do note the following:
- Built with MinGW compiler
- Built against C++23 standard

Running tests:
```bash
mkdir build
cd build
cmake ..
cmake --build .
ctest
```

### Notes:
     
If the users can input a maximum capacity for the queue, 
then the capacity of the queue should be compile time determined.
This is far safer. Here is an example:
```c++
template <typename T, size_t N>
class Queue<T> {
    T data[N];
}
```

The underlying data is still dynamically allocated. 
However, I have not opted for a data structure that grows and
shrinks.