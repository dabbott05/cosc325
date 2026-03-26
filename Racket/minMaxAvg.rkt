#lang racket
(define test_data '(1 2 3 4 5 6 7 8 9 10))
; recursive function to add up all the numbers in the list

(define (suml lis)
  (if (= (length lis) 1)
      ; car is the function that gives you the first item in a list
      (car lis)
      (+ (car lis) (suml (cdr lis)))))

; no error checking
(define (avgl lis)
  (/ (suml lis) (length lis)))

; with  error checking
(define (avglE lis)
  (if (empty? lis)
      (print "error")
      (/ (suml lis) (length lis))))

; min element in list
;base case - 1 item lest, that item is the min
; recursive step - trciky!
(define (minl lis)
  (if (= (length lis) 1)
      (car lis)
      (if (< (car lis) (minl (cdr lis)))
             (car lis)
             (minl (cdr lis)))))

(define (maxl lis)
  (if (= (length lis) 1)
      (car lis)
      (if (> (car lis) (maxl (cdr lis)))
             (car lis)
             (maxl (cdr lis)))))

(define (min_using_let lis)
  (if (= (length lis) 1)
      (car lis)
      (let (
          (headitem (car lis))
          (mintail (min_using_let (cdr lis)))
          )
        (if (< headitem mintail)
        headitem
        mintail))))

; reversing a list is tricky recursion
; base case - list is empty ==> empty list
; recursive step - list is not empty ==> append the head item to the "reversed" tail
(define (reversel lis)
  (if (empty? lis)
      lis
      (append (reversel (cdr lis)) (list (car lis)))))

; review of how filter works
(define (is_less_than_5 a)
  (< a 5))

; our own implementation of filter
; func needs to be a function that returns true or false and takes in a single parameter
; base case empty lis ==> return empty list
; recursive step ..  nested if based on func
(define (myfilter func lis)
  (if (empty? lis)
      lis
      (if (func (car lis))
          (cons (car lis)
                (myfilter func (cdr lis)))
          (myfilter func (cdr lis)))))
