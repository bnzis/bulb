## Bulb
Bulb is a simple **Lisp** dialect very similar to **Scheme** on which I work in my free time. 
The ultimate goal of the project is to create a modular interpreter with **statically linked components** (modules)
in order to reduce the amount of bloat. Here are some examples of the syntax:
## Basics
```scheme
; this is a comment
(def integer 1) 
(def floating 3.1415926535)
(def flag #f) ; #f stands for false, #t for true
(def str "potatoes") ; this a string

; functions can be defined like this 
(def (abs x) (if (> x 0) x (- x))) 

; or like this 
(def abs (lambda (x) (if (> x 0)...
```
