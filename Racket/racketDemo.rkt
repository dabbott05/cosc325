#lang racket
(define pi 3.14159)
(define pi_p1 (+ pi 1))
(define test_data '(1 2 3 4 5 6 7 8 9 10))

(define (sum2 a b)
  (define thresh 100)
  ; if a + b is less than thresh return a + b if not retunr 0
  (if (< (+ a b) thresh) (+ a b) 0))

(define (factorial x)
  (if (<= x 1)
      1 ; then
      (* x (factorial (- x 1))))) ; else

(define (doit f a)
  (f a)
  )

(doit factorial 5)

