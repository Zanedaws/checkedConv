# checkedConv
This is code that I have converted to the Checked C language from C. The folders with the "-conv" appended to them contain the converted code while the others have the original code. 

# Notes on the Process
This is a section about how hurdles that were seen during the process of converting.

-PA1
	This program is a shell sorting algorithm meant to display the difference in efficiency between using arrays and using linked lists to store the data. As such the linked list method is far slower than the array method and will take quite a while to complete the 100K.b sorting. On the other hand the array methods will be able to complete the 1M.b sorting (1 million values) quite quickly.

	While converting this code by hand I had to modify the array functions to get the size of the array needed prior to sorting like in the original C program. This allowed the definition of the bounds of the array apon creation. Another interesting issue that I encountered was that when using int values to declare the bounds of the array, errors and warnings were produced. Thus, I switched all bounds defining values to be stored as size_t values and all errors and warnings went away.
