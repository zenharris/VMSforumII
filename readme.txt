

                     VMSforumII
 		  BBS style Forum
	    For Openvms on VAX and Alpha

Author: Michael Brown, Newcastle, Australia

Email : vmslib@b5.net

Latest Versions available at 
https://eisner.decus.org/~brown_mi/

BUILDING EXECUTABLE:

edit LOCALITY.H where you can specify the path to the data
file (default is current directory) and the privileged user
(can delete any post).

To build FORUM executable change to the EXTOOLS directory
and then use the following DCL command

	$ mms

This will build EXTOOLS.OLB.

Change directory back to the forum source and execute mms
again.

The result of this operation is FORUM.EXE.

Forum will run at user level though a system administrator
could install FORUM with greater privilege to protect the
message database file which must have write permission to
users.


