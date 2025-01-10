; L2
; 10.
; Tree representation: (node (list-subtree-1) (list-subtree-2) ...) 
; e.g. (A (B) (C (D) (E))) A has children B, C and C has children D, E
; Return the level of a node X in a tree. The level of the root element is 0.

; find-level((root, L, R), node, level) = NIL, n = 0
;				                                = level, node = root
;				                                = find-level(L, node, level + 1) or find-level(R, node, level + 1), node != root


(defun find-level (tree node &optional (level 0))
  "Find the level of NODE in TREE, where TREE is represented as a nested list."
  (cond
    
    ; Tree is empty, return NIL (node not found).
    ((null tree) nil)
    
    ; Root of the current tree matches the node, return the level.
    ((eq (first tree) node) level)
    
    ; Oth. search recursively in the left and right subtrees.
    (t
     (or
      ; Search left subtree
      (find-level (second tree) node (+ level 1))
      
      ; Search right subtree
      (find-level (third tree) node (+ level 1))))))


(format t "~a~%" (find-level '(A (B) (C (D) (E))) 'E)) ; Returns 2
(format t "~a~%" (find-level '(A (B) (C (D) (E))) 'B)) ; Returns 1
(format t "~a~%" (find-level '(A (B) (C (D) (E))) 'A)) ; Returns 0
(format t "~a~%" (find-level '(A (B) (C (D) (E))) 'F)) ; Returns NIL
