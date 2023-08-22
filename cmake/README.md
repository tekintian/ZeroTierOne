This PoC works on mac, but it will take a ton of work to add all the ARM flags, add linux, add windows, etc...
So you can't use it to make releases, but it's already helpful for doing development. 

Using "ninja" instead of the default `make` makes it rebuild faster and detect changes properly.


``` sh
# once, to set up
cmake -S . -Bbuild  -G"Ninja Multi-Config" 

# while hacking
cmake --build build --config Debug 
or
cmake --build build --config Release 

```
