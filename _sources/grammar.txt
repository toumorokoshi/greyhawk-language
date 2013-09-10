Grammar
=======

The grammar is expressed in EBNF (Exended Backus-Naur Form)::

    digit_excluding_zero = "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;
    digit = "0" | digit_excluding_zero ;
    integer = "0" | [ "-" ] digit_excluding_zero { digit }
