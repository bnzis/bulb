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
    (if (null? l) (exit))
    (if (list? l)
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
Now, you need a c compiler and make installed, for Linux and BSD users it probably 
comes pre-installed, if not, refer to the manual of the package manager of your
distro and install it. For Windows users, I suggest to install MinGW.
Finally, compile and install: 
```
cd /path/of/bulb
make all
sudo make install # only works for Linux and BSD users.
```
