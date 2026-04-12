#lang racket
(define (geti lis i)
  (cond
    ((= i (length lis)) (print "error, indexoutodboundsException") #f)
    ((= i 0) (car lis))
    (else (geti (cdr lis) (- i 1)))))

(define (findv lis val)
  (findvHelper lis val 0))

(define (findvHelper lis val i)
  (cond
    ((empty? lis) -1)
    ((= (car lis) val) i)
    (else (findvHelper (cdr lis) val (+ i 1)))))

(define (maxl lis)
  (if (= (length lis) 1)
      (car lis)
      (if (> (car lis) (maxl (cdr lis)))
          (car lis)
          (maxl (cdr lis)))))

(define (countElement lis element)
  (cond
    ((empty? lis) 0)
    ((= (car lis) element) (+ 1 (countElement (cdr lis) element)))
    (else (countElement (cdr lis) element))))

(define (countAll lis)
  (map (lambda (a) (countElement lis a)) lis))

(define (mean lis)
  (/ (apply + lis) (length lis)))

(define (min-val lis)
  (apply min lis))

(define (max-val lis)
  (apply max lis))

(define (median lis)
  (if (odd? (length lis))
      ; If odd, just get the middle element of the sorted list
      (geti (sort lis <) (quotient (length lis) 2))
      ; If even, add the two middle elements and divide by 2
      (/ (+ (geti (sort lis <) (quotient (length lis) 2))
            (geti (sort lis <) (- (quotient (length lis) 2) 1)))
         2)))

(define (mode lis)
  (geti lis (findv (countAll lis) (maxl (countAll lis)))))