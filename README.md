# Compile cubiomes

```
cd cubiomes
make libcubiomes
```

You can then move the generated file `libcubiomes.a` where you want, in this case in the parent directory.

# Compile and run

```
gcc <path/to/file.c> -L. -lcubiomes -lm && ./a.out
```
In this case, replace `<path/to/file>.c` with `main.c`