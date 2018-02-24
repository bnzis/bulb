Description
===========
The evaluator perfoms two tasks for evaluating an expression:  
- Eval: the expression is interpreted in its sub-expressions up until 
        there's nothing to decompose, such as numbers;
- Apply: executes the procedure to the arguments given in a 
         separate environment, using eval.  

bulbEval
--------
Evaluates the expression given int the environment passed.
```c
bulbObj *bulbEval(bulbObj *ast, bulbEnv *env);
```
bulbEvalSequence
----------------
Evaluates a sequence of expressions.
```c
bulbObj *bulbEvalSequence(bulbObj *ast, bulbEnv *env);
```
bulbEvalIf, bulbEvalDef, bulbEvalSet
------------------------------------
This functions are meant to improve readbility.
```
; <value> is optional
(def <identifier> <value>) 
(def (<identifier> <parameters>) <body>)
(set! <identifier> <value>)
(def (<identifier> <value>) <body>)
; <to do if false> is optional
(if <condition> <to do if true> <to do if false>)
```
```c
bulbObj *bulbEvalIf(bulbObj *ast, bulbEnv *env);

bulbObj *bulbEvalDefine(bulbObj *ast, bulbEnv *env);

bulbObj *bulbEvalSet(bulbObj *ast, bulbEnv *env);
```
bulbExpandEnv
------------- 
Expand the environment binding the arguments passed to the formal arguments. 
```
(def f (lambda (x y) ...))
(f 10 20) ; x => 10, y => 20 
```
```c
bulbEnv *bulbExpandEnv(bulbObj *ast, bulbObj *proc, bulbObj *args, bulbEnv *env);
```
Exceptions 
----------
```c
bool bulbNotKeyword(char *sym);

void bulb_err_non_procedure(bulbObj *proc);

void bulb_err_invalid_syntax(bulbObj *tree);

void bulb_err_invalid_parameter_bool(bulbObj *tree);

void bulb_err_invalid_len(bulbObj *ast, unsigned expected, unsigned given);
```
