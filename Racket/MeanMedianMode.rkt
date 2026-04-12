#lang racket

; mode - could i add each element to a new list ?? and then prompt for if the next element is equal to
; the first element in the list and if it is add it to the list if not add it to a new list
; return the biggest list

; or just do this for mode - just find your helper methods in the other files
(define (geti lis i)
      (cond
        ((= i (length lis)) (print "error, indexoutodboundsException") #f)
        ((= i 0) (car lis))
        (else
          (geti (cdr lis) (- i 1)))))

(define (findv lis val)
  (findvHelper lis val 0))

(define (findvHelper lis val i)
  (cond
    ((empty? lis) -1)
    (( = (car lis) val) i)
    (else (findvHelper (cdr lis) val (+ i 1)))))

(define (countAll lis)
  (map (lambda (a) (countElement lis a ))))

(define (maxl lis)
  (if (= (length lis) 1)
      (car lis)
      (if (> (car lis) (maxl (cdr lis)))
             (car lis)
             (maxl (cdr lis)))))

(define (countElement lis element)
   (cond
     ((empty? lis) 0)
     ((= (car lis) element) (+1 ( countElement (cdr lis) element)))
     (else (countElement (cdr lis) element))))

(define lisg '(1 2 2 4 3 4 4 4))
(define (mode lis)
  (geti lis (findv (countAll lis) (maxl (countAll lis)))))

(define (mean lis)
  (/ (apply + lis) (length lis)))

(define (min-val lis)
  (apply min lis))

(define (max-val lis)
  (apply max lis))

(define (median lis)
  (let* ([sorted (sort lis <)]
         [len (length lis)]
         [mid (quotient len 2)])
    (if (odd? len)
        (geti sorted mid)
        (/ (+ (geti sorted mid) (geti sorted (- mid 1))) 2))))

(define (mode lis)
  (geti lis (findv (countAll lis) (maxl (countAll lis)))))