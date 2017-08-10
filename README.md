Before building make sure that gcc, flex and bison are installed. For example on Ubuntu:

    # apt-get install flex bison build-essential

Then to build:

    $ cd interpreter
    $ make

To run built-in tests:

    $ make test
    
This is simple testsuite which compares interpreter output for predefined files with
reference output. If there is ane difference, there will be warning message frm `cmp`
and make will exit with non-zero status.

To check source code with Google's CPPLINT:

    $ make lint
