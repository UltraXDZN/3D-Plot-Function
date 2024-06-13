# Dokumentacija za  3D Grafički Prikaz Funkcije ***f(x, y)***

----

[TOC]

# plot.c

----

## Instalacija

1. **Potrebni library-i**:
    
    - [OpenGL](https://www.mingw-w64.org/downloads/)
    
      - Instalacija OpenGL-a pomocu mingw-64 konzole:
    
        ```bash
        pacman -S mingw-w64-x86_64-freeglut
        pacman -S mingw-w64-x86_64-opengl
        ```
    
    - [TinyExpr](https://github.com/codeplea/tinyexpr)
    
2. **Kompajliranje** *plot.c*:
    
    ```sh
    gcc plot.c TE\tinyexpr.c -o plot -lfreeglut -lopengl32 -lglu32
    ```

## Korištenje

Program se može pokrenuti na više mogućih načina: 

1. ### Windows:

```sh
.\plot.exe "funkcija"
```

1. ### Linux:

```sh
./plot "funkcija"
```

Ako izraz nije zadan, koristi se `default`-ni izraz `3 - (x*x + y*y)`.

**Napomena: `funkcija` mora biti zanada u tinyexpr obliku ako se program koristi direktno iz konzole**



## Objašnjenje koda:

### Globalne varijable

- `float angleX, angleZ`: kutevi rotacije oko X i Z osi koordinatnog sustava.
- `const char *expr`: Funkcija koju je potrebno nacrtati (zadana kao parametar).
- `te_expr *compiled_expr`: Izčitavanje funkcije iz `*expr`.
- `double variables[2]`: Pohranjivanje vrijednosti `x` i `y`.

### Funkcije crtanja:

1. #### Inicijalizacija i postavljanje OpenGL-a

```c
void initOpenGL();
```

Postavlja osnovne postavke OpenGL-a kao što su boja pozadine, i sjenčenje.

2. #### Promijena Veličine Prozor

```c
void initOpenGL();
```

Postavlja osnovne postavke OpenGL-a kao što su boja pozadine, i sjenčenje.

```c
void reshape(int width, int height);
```
Postavlja viewport i projekciju na osnovu veličine prozora.

3. #### Rotacija

```c
void rotate(int key, int x, int z);
```
Rotira kameru oko koordinatnog sustava ovisno o pritisnutoj stelici na tipkovnici (lijeva i desna strelica mijenja x-os, a gornja i donja strelica mijenjaju z-os)

4. #### Renderiranje Osi

```c
void renderBitmapString(float x, float y, float z, void *font, const char *string);
```
Renderira x, y i z na kraju njihovih osi.

5. #### Evaluacija Izraza

```c
double evaluateExpression(double x, double y);
```
Dohvaća vrijednost matematičkog izraza unesenog pomoću `tinyexpr` te ga izračunava na temelju danih x i y vrijednosti (koristi se pri crtanju grafa).

6. #### Prikaz

```c
void display();
```
Glavna funkcija za crtanje 3D koordinatnog sustava, grafa te prikaz i rotaciju kamere.
