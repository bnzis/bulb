## Bulb
Bulb is a simple **Lisp** dialect very similar to **Scheme** on which 
I work in my free time. The ultimate goal of the project is to create 
a lightweight interpreter, easy to integrate on other projects.
Here are some examples of the syntax:
## Basics
```scheme
; this is a comment
(def integer 1) 
(def floating 3.1415926535)
(def flag #f) ; #f stands for false, #t for true
(def str "potatoes") ; this a string

; functions can be defined like this 
(def (abs x) (if (> x 0) x (* x -1))) 

; or like this 
(def abs (lambda (x) (if (> x 0) x (* x -1))))

; more complex example of code
(def (print-list l)
    (if (and (list? l) (not (null? l)))
        (begin 
            (print (car l) " ")
            (print-list (cdr l)))))

(print-list (list 1 2 3 4 5)) ;=> 1 2 3 4 5
(print "\n")
```

## How to install
First of all, you need the source code.
Download the tarball or clone the repository: 
```
git clone https://github.com/bnzis/bulb
```
You need **make** and a **C compiler**. If you're a 
**Linux** or **BSD** user, you probably already have both; 
if not, consult the manual of your distribution to install 
them. If you use Windows instead, I recommend installing **MinGW**.
Compile and install: 
```
cd /path/of/bulb
make all # please check config.mk before building
sudo make install # works only on Linux and BSD.
```

## How to use it
```
bulb # open the repl
bulb /path/to/file # execute the code in the file
```
