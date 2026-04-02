#lang racket
(define (calculatePlus a b)
  (+ a b))

(define (calculateSomething fun a b)
  (fun a  b))

(define (buildAdder x)
  (lambda (a) (+ a x)))

(define add3 (buildAdder 3))
(define add7 (buildAdder 7))

; a higher order function is finction that creates a function or takes in a function
; special higher order functions that apply to lists of data
; map - repeatedly apply a function to the values in a list creating a new list of results


(map add7 '(1 2 3 4 5))
(map + '(1 2 3 4 5) '(7 7 7 7 7))

; filter - filter a list with a function elements must "pass" in order to stay in the list

(filter (lambda (a) (< a 10)) '(7 8 9 10 11 12 13))


; fold - better known as reduce in most languages
; reduces a list to a single number using a function

(foldl - 0 '(1 2 3 4 5 6 7 8 9 10))