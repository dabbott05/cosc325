
#lang racket
(define pi 3.14159)
(define pi_p1 (+ pi 1))
(define test_data '(1 2 3 4 5 6 7 8 9 10))

(foldl + 0 test_data)
;total = 0
; for num in test_data:
; total += num
(foldl * 1 test_data)
;total = 1
; for num in test_data:
; total *= num

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

