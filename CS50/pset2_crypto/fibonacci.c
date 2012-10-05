/****************************************************************************
 * fibonacci.c
 *
 * Computer Science 50
 * Daniel E. Williams
 *
 * A program that prints a requested sequence of fibonacci numbers.
 *
 * Created:  7/10/12
 * Modified: 7/10/12
 ***************************************************************************/
 
 #include <stdio.h>
 #include <stdlib.h>
 
 // fibonacci function declaration
 void fibonacci(int fib_sequence[], int number);
 
 int
 main(int argc, char *argv[])
 {
    // ensure user entered only one number
    if(argc != 2)
    {
        printf("Usage: ./fibonacci <sequence length>\n");
        return 1;
    }
  
    // ensure input is a number
    int num = atoi(argv[1]);  
    if(num <= 0)
    {
        printf("Please enter a positive integer.\n");
        return 2;
    }
  
    // get the sequence
    int seq[num];
    fibonacci(seq, num);  
    
    // print out the sequence
    for(int i = 0; i < num; i++)
    {
        printf("%d", seq[i]);
        if(i < num - i)
            printf(", ");
        else
            printf("\n");
    }
    
    // successful return
    return 0;
 }
 
 void
 fibonacci(int fib_sequence[], int number)
 {
    // set first element to 0
    fib_sequence[0] = 0;
    
    // set second element to 1
    if(number >= 2)
    {
        fib_sequence[1] = 1;
        
        // if only two elements needed, return
        if(number == 2)
            return;
    }
    
    // if more than two elements needed, calculate new elements
    for(int i = 2; i < number; i++)
        fib_sequence[i] = fib_sequence[i - 1] + fib_sequence[i - 2];
        
    return;
 }
