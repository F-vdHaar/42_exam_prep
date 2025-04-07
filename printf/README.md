 See 
 attachment/subject.en.txt    

+---------------------+
| ft_printf() called  |
+---------------------+
         |
         v
+---------------------+     (checks format string)
| print_loop()        | <----+  [Is it %s, %d, or %x?]
+---------------------+      |
         |                   |
         v                   v
+------------+    +------------------------------+     +-------------------+
| ft_putchar |    | print_in_base (for %d or %x) |     | Write regular char|
+------------+    +------------------------------+     +-------------------+
         |                         |
         v                         v
      [write to stdout]            [recursive number printing]
         |                         |
         +-------------------------+
         |
         v
   Return number of printed chars
