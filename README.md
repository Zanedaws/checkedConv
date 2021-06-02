# checkedConv
This is code that I have converted to the Checked C language from C. The folders with the "-conv" appended to them contain the converted code while the others have the original code. 

# Notes on the Process
This is a section about how hurdles that were seen during the process of converting.

Often times I would come across errors where the compiler could not determine if the bounds for a given array were valid after a call to a function such as fwrite or free despite these functions having a bounds safe versions within the libraries stdlib_checked.h and stdio_checked.h.
