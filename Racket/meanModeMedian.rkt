#lang racket
(define lis '(1 2 3 4 5))

; get a specific element in a list
(define (geti lis i)
      (cond
        ((= i (length lis)) (print "error, indexoutodboundsException") #f)
        ((= i 0) (car lis))
        (else
          (geti (cdr lis) (- i 1)))))

; get the first occurence of val in the list
;-1 if not found
(define (findv lis val)
  (findvHelper lis val 0))

; recursive helper function for findv
; lis - the list we are searching
; val - the value we are searching for
; i - where we are starting from (or thinking recursively , how far we have made it so far)
; basecase1 - i == len(lis) - means we didnt find it so return -1
; basecase2 - (car list)==val means we found it!, so return i
; recursive step - (findvHelper (cdr lis) val (+ i 1))
(define (findvHelper lis val i)
  (cond
    ((empty? lis) -1)
    (( = (car lis) val) i)
    (else (findvHelper (cdr lis) val (+ i 1)))))

; returns a shortened list if the elements with the even indeces in the original list
; (evenPositionsOnly '(1 2 3 4 5) ==> list containing '(1 3 5)
(define (evenPositionsOnly lis)
  (evenPositionsOnlyH lis 0))

; base case - empty list ... return empty list
(define (evenPositionsOnlyH lis i)
  (cond
    ((empty? lis) '())
    ((even? i) (cons (car lis) (evenPositionsOnlyH (cdr lis) (+ i 1))))
    (else (evenPositionsOnlyH (cdr lis) (+ i 1)))))

; reminder of how filter works
; applies a fucntion that has a single parameter to each element in the list
; if the function returns true, that element is kept .. if false , the element is discarded

(define (evenVals lis)
  (filter even? lis))

;doesnt work with duplicates
; but here is how you use filter to get the eve positions
; you have to search for the element and then see if the index was odd or even

(define (evenPos lis)
  (filter
   (lambda (element)
     (even? (findv lis element))) lis))

; sort (the really inefficient way)
; base case - list is empty
; 2 recursive steps ... if first item is the smallest item then just cons it onto the sorted tail of the list
; if its not the smallest item then just return the sorted entire list but with the first item moved to the end
(define (sortl lis)
  (cond
    ((empty? lis) lis)
    ((= (car lis) (apply min lis)) (cons (car lis) (sortl (cdr lis))))
    (else
     (sortl (append (cdr lis) (list (car lis)))))))


; returns how manu times an element appears in the list (mode)
; gonna need many helper functions for mode
; need to do at home as well as median and mean
(define (countElement lis element)
   (cond
     ((empty? lis) 0)
     ((= (car lis) element) (+1 ( countElement (cdr lis) element)))
     (else (countElement (cdr lis) element))))