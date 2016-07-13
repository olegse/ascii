# ascii
characters manipulation

simplenote, notetaker and notesearch programs dimonstrates very interesting example of 
sticky bit usage

 First note is writen by notetaker, that has a sticky bit set. 
 Storing the real user id on the beginning of the string.
 Then new line is writen.
 Then a user note.

 Note search in turn reads firs four bytes of user id and comparing with real user id of
 the program.

 If match is found note is read till new line is found.

 All character manipulations can be debugged with 

[TODO]
	
	line number : white spaces count
